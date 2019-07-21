#include "Black_hole.h"
//#include "Tile_M.h"
#include "ImageRes.h"
#define back_frame_speed 0.03
#define gravity_sensor_scale 8
#define gravity_bf_size_scale 0.5
#define gravity_size_scale 0.2
#define gravity_power_scale 0.4//0.00009
#define tpin_radius 15
Vertex_Sprite::Vertex_Sprite(const Vector2 &pos, float gravity) {
	setSize(gravity * gravity_size_scale, gravity * gravity_size_scale);
	setColor(Color::Blue);
	setPosition(pos);
	setAnchor(0.5f, 0.5f);
	attachTo(getStage());
}

Back_frame::Back_frame(const Vector2 &pos, float gravity) {
	setResAnim(res::resources.getResAnim("Black_Hole"));
	setPosition(pos);
	setAnchor(0.5f, 0.5f);
	
	
	attachTo(getStage());
	setSize(Vector2(gravity * gravity_sensor_scale * gravity_bf_size_scale, gravity * gravity_sensor_scale * gravity_bf_size_scale));

	rot_speed = -gravity / 10000;
	
}

void Back_frame::doUpdate(const UpdateState &us) {
	setRotation(getRotation() + (us.dt * rot_speed));
}

void Back_frame::destroy() {
	detach();
	
}

void Vertex::doUpdate(const UpdateState &us) {
	Vector2 dif = GF::convert(body->GetPosition()) - GF::convert(Ship_M::objects[0]->_torso->body->GetPosition());
	w_sound.setCoord(dif / 2000);
	_tpin->SetPosit(getPosition());
}

void Vertex::play_sound() {
	if (_is_playing_sound == false) {
		w_sound.play();
	}
	_is_playing_sound = true;
	
}

void Vertex::stop_sound() {
	
	if (_is_playing_sound == true) {
		w_sound.si->fadeOut(1400);
	}
	_is_playing_sound = false;
	
}

bool Vertex::is_sound_playing() {
	return w_sound.si->isPlaying();
}

void Vertex::fadeOut_tpin(int fadeTime) {
	_tpin->fadeOut(fadeTime);
}

Vertex::Vertex(const Vector2 &pos, float gravity) : w_sound(Noise::st::Black_Hole_Wind, Noise::player::sfx, PlayOptions().loop(true)) {

	
	//w_sound.play();
	setName("BHV");
	
	b2FixtureDef fd;
	//fd.filter.groupIndex = -1;
	fd.filter.categoryBits = BLACK_HOLE;
	fd.filter.maskBits = DEFAULT_MASK;
	setup_body(RectF(pos, getSize()), NULL, NULL, &fd);
	_tpin = new TPIN(pos, tpin_radius, world_ptr.get(), TPIN::type::ENTITY);
	//world_ptr->addChild(this);
	body->SetUserData(this);

}

b2Body *Vertex::getBody() {
	return body;
}
void Vertex::destroy() {
	if (_tpin.get() != NULL) {
		_tpin->detach();
		//_tpin.~intrusive_ptr();
	}
	//w_sound.destroy();
	detach();
	world_ptr->getb2World()->DestroyBody(body);
	
}

void Vertex::if_contact(b2Body *other_body, Actor* other_actor) {
	
	bool should_put = true;
	//other_actor->getName() != "Pinwheel" && other_actor->getName() != "Temple"
	if (other_actor->getName() == "PM" || other_actor->getName() == "EM") {
		Noise::play_ID(Noise::st::Black_Hole_Suck, Noise::player::sfx);
		//Ship_M::objects[0]->line_controller.delete_all();
		for (unsigned int i = 0; i < world_ptr->d_cache.size(); i++) {
			if (world_ptr->d_cache[i] == other_body) {
				should_put = false;
			}
		}
		if (should_put == true) {
			world_ptr->d_cache.push_back(other_body);
		}
	}
	
}

Field::Field(const Vector2 &pos, float gravity) {
	if (world_ptr->getLevel() == 2) {
		gravity *= 2.0f;
	}
	//This is the black portal behind everything.
	bf = new Back_frame(Vector2(0, 0), gravity);
	addChild(bf);

	//This is the blue dot in the center of the black_hole.
	vtx_sprite = new Vertex_Sprite(Vector2(0, 0), gravity);
	addChild(vtx_sprite);
	//This is the acutal body of the vertex, if something collides with this it will be destroyed.
	vtx = new Vertex(pos ,gravity);
	addChild(vtx);

	_gravity = gravity;
	setName("BHS");

	//Creating fixtures for the field.
	b2FixtureDef fd;
	fd.filter.categoryBits = BLACK_HOLE; //It is a black_hole.
	fd.filter.maskBits = DEFAULT_MASK; //Collide with everything.
	b2CircleShape cs;
	cs.m_radius = GF::convert_to_b2(gravity * gravity_sensor_scale);
	b2Fixture *fx = setup_body(RectF(pos, Vector2(0,0)), NULL, &cs, &fd);
	
	//Welding the field's body to the vertex's body.
	b2WeldJointDef wd;
	wd.bodyA = body;
	wd.bodyB = vtx->getBody();
	weld_jt = world_ptr->getb2World()->CreateJoint(static_cast<b2JointDef*>(&wd));

	fx->SetSensor(true);
	world_ptr->addChild(this);
	body->SetUserData(this);
}

void Field::if_entered(b2Body *other_body, Actor* other_actor) {
	//if (other_actor->getName() == "PM" || other_actor->getName() == "EM") {
	//	if (vtx->is_sound_playing() == false) {
	//		vtx->play_sound();
	//	}
	//}
	if (other_actor->getName() == "EM" || other_actor->getName() == "PM") {
		
		vtx->play_sound();
		contained.push_back(other_body);
	}
}

void Field::if_exited(b2Body *other_body) {
	if (contained.size() == 1) {
		vtx->stop_sound();
	}
	
	for (unsigned int i = 0; i < contained.size(); i++) {
		if (contained[i] == nullptr) {
			contained.erase(contained.begin() + i);
			i--;
		}
		else {
			if (contained[i] == other_body) {
				
				contained.erase(contained.begin() + i);
				break;
			}
		}
		
	}

}

void Field::doUpdate(const UpdateState& us) {
	if (!world_ptr->get_is_paused()) {
		for (unsigned int i = 0; i < contained.size(); i++) {
			if (contained[i] != nullptr) {
				b2Body *b_ptr = contained[i];

				float dist = GF::getDistance(getPosition(), GF::convert(b_ptr->GetPosition()));
				float power = _gravity * (300 / dist);
				power *= gravity_power_scale;
				b2Vec2 vec(GF::convert(getPosition()) - (b2Vec2(b_ptr->GetPosition().x, b_ptr->GetPosition().y)));
				vec.x *= power;
				vec.y *= power;
				b_ptr->ApplyForceToCenter(vec, true);
			}

		}
	}
	
}


void Field::destroy() {
	vtx->destroy();
	bf->destroy();
	vtx_sprite->detach();
	world_ptr->getb2World()->DestroyBody(body);
	detach();
	
}

void Field::fadeOut(unsigned int time) {
	GF::fadeOut(this, time, 0);
	vtx->fadeOut_tpin(time);
}

Black_hole::Black_hole(const Vector2 &pos, float gravity, void* world_ptr, unsigned int fadeTime)
{
	setName("BH");
	World* w_ptr = static_cast<World*>(world_ptr);
	setPosition(pos);
	
	
	field = new Field(pos, gravity);
	
	GF::fadeIn(field.get(), fadeTime);
}

void Black_hole::destroy() {
	field->destroy();
	
	
	//detach();
}

void Black_hole::onRemovedFromStage() {
	destroy();
}

void Black_hole::fadeOut(unsigned int time) {
	field->fadeOut(time);
	GF::fadeOut(this, time, 0);
	
}