#include "Ship_Part_M.h"
#include "ImageRes.h"
#include "GF.h"
#include "Ship_M.h"
#define max_friction_amplitude 4000
#define max_vel 160
#define collision_sound_volume 0.3f
spWorld Ship_Part_M::_world_ptr;

Ship_Part_M::Ship_Part_M() : c_sound(Noise::st::Collision, Noise::player::sfx) {
	//c_sound.si->setVolume(collision_sound_volume);
	c_sound.setCoord(Vector2(50000000, 0));
	//Making the s_emitter point to a P_System object.
	//This does not need to have a direction or position, as it will be overwritten later.
	s_emitter = new P_System(Vector2(0, 0), Vector2(0, 0), 0, _world_ptr, false);
	_world_ptr->addChild(s_emitter);
	contact_ptr = NULL;
	setPriority(4000);
	setName("PM");
}

Ship_Part_M::~Ship_Part_M() {

}

void Ship_Part_M::setup(const char* sprite, const Vector2 &pos, filters categoryBits, filters maskBits ,e_types type, float scale) {
	identifier = sprite;
	setResAnim(res::resources.getResAnim(sprite));
	setPosition(pos);
	setAnchor(0.5f, 0.5f);
	setScale(scale);
	setSize(getSize());
	b2BodyDef def;
	def.type = b2_dynamicBody;
	def.position = GF::convert(pos, GF::get_G_Scale());
	def.angle = 0;
	body = _world_ptr->getb2World()->CreateBody(&def);
	b2FixtureDef fd;
	fd.filter.maskBits = maskBits;
	fd.filter.categoryBits = categoryBits;
	fd.filter.groupIndex = -1;
	fd.density = 1.0f;
	b2PolygonShape poly_shape;
	b2CircleShape circle_shape;
	b2Vec2 sz;
	b2Vec2 vertices[6];
	float G = GF::get_G_Scale();
	switch (type) {
	case 0: //box
		sz = GF::convert(getSize() / 2, GF::get_G_Scale());
		poly_shape.SetAsBox(sz.x, sz.y);
		fd.shape = &poly_shape;
		body->CreateFixture(&fd);
		break;
	case 1: //e_l_leg
		vertices[0].Set(GF::convert_to_b2(31, G), GF::convert_to_b2(4, G));
		vertices[1].Set(GF::convert_to_b2(25, G), GF::convert_to_b2(4, G));
		vertices[2].Set(GF::convert_to_b2(10, G), GF::convert_to_b2(9, G));
		vertices[3].Set(GF::convert_to_b2(0, G), GF::convert_to_b2(31, G));
		vertices[4].Set(GF::convert_to_b2(5, G), GF::convert_to_b2(31, G));
		vertices[5].Set(GF::convert_to_b2(31, G), GF::convert_to_b2(8, G));
		for (unsigned int i = 0; i < 6; i++) {
			vertices[i].Set(vertices[i].x - GF::convert_to_b2(getSize().x / 2, G), vertices[i].y - GF::convert_to_b2(getSize().y / 2, G));
		}
		poly_shape.Set(vertices, 6);
		fd.shape = &poly_shape;
		body->CreateFixture(&fd);
		break;
	case 2: //e_r_leg
		
		vertices[0].Set(GF::convert_to_b2(0, G), GF::convert_to_b2(4, G));
		vertices[1].Set(GF::convert_to_b2(0, G), GF::convert_to_b2(8, G));
		vertices[2].Set(GF::convert_to_b2(26, G), GF::convert_to_b2(31, G));
		vertices[3].Set(GF::convert_to_b2(31, G), GF::convert_to_b2(31, G));
		vertices[4].Set(GF::convert_to_b2(21, G), GF::convert_to_b2(9, G));
		vertices[5].Set(GF::convert_to_b2(6, G), GF::convert_to_b2(4, G));
		for (unsigned int i = 0; i < 6; i++) {
			vertices[i].Set(vertices[i].x - GF::convert_to_b2(getSize().x / 2, G), vertices[i].y - GF::convert_to_b2(getSize().y / 2, G));
		}
		poly_shape.Set(vertices, 6);
		fd.shape = &poly_shape;
		body->CreateFixture(&fd);
		break;
	case 3: //engine
		vertices[0].Set(GF::convert_to_b2(5, G), GF::convert_to_b2(0, G));
		vertices[1].Set(GF::convert_to_b2(0, G), GF::convert_to_b2(9, G));
		vertices[2].Set(GF::convert_to_b2(0, G), GF::convert_to_b2(19, G));
		vertices[3].Set(GF::convert_to_b2(15, G), GF::convert_to_b2(19, G));
		vertices[4].Set(GF::convert_to_b2(15, G), GF::convert_to_b2(9, G));
		vertices[5].Set(GF::convert_to_b2(10, G), GF::convert_to_b2(0, G));
		for (unsigned int i = 0; i < 6; i++) {
			vertices[i].Set(vertices[i].x - GF::convert_to_b2(getSize().x / 2, G), vertices[i].y - GF::convert_to_b2(getSize().y / 2, G));
		}
		poly_shape.Set(vertices, 6);
		fd.shape = &poly_shape;
		fd.restitution = 0;
		body->CreateFixture(&fd);
		break;
	case 4: //Torso
		circle_shape.m_radius = GF::convert_to_b2(16, G);
		
		vertices[0].Set(GF::convert_to_b2(0, G), GF::convert_to_b2(10, G));
		vertices[1].Set(GF::convert_to_b2(0, G), GF::convert_to_b2(18, G));
		vertices[2].Set(GF::convert_to_b2(43, G), GF::convert_to_b2(18, G));
		vertices[3].Set(GF::convert_to_b2(43, G), GF::convert_to_b2(10, G));
		for (unsigned int i = 0; i < 4; i++) {
			vertices[i].Set(vertices[i].x - GF::convert_to_b2(getSize().x / 2, G), vertices[i].y - GF::convert_to_b2(getSize().y / 2, G));
		}
		poly_shape.Set(vertices, 4);
		fd.shape = &circle_shape;
		body->CreateFixture(&fd);
		fd.shape = &poly_shape;
		body->CreateFixture(&fd);
		break;
	default:
		sz = GF::convert(getSize() / 2, GF::get_G_Scale());
		poly_shape.SetAsBox(sz.x, sz.y);
		fd.shape = &poly_shape;
		body->CreateFixture(&fd);
	}
	/*switch (type) {
	case 0:
		log::messageln("0Test");
		break;
	case 2:
		log::messageln("2TTest");
	}*/
	//sz = GF::convert(getSize() / 2, GF::get_G_Scale());
	//shape.SetAsBox(sz.x, sz.y);
	/*switch (type) {
	case 0: //box
		
		break;
	case 1: //e_l_leg

	}*/
	
	
	body->SetAngularDamping(40);
	body->SetUserData(this);
}

void Ship_Part_M::init(World *world) {
	_world_ptr = world;
	
}

void Ship_Part_M::update_emitter() {
	if (contact_ptr != NULL) {
		b2WorldManifold mani;
		contact_ptr->GetWorldManifold(&mani);
		unsigned int pc = contact_ptr->GetManifold()->pointCount;
		if (pc == 1) {
			
			b2Body *body_A = contact_ptr->GetFixtureA()->GetBody();
			b2Body *body_B = contact_ptr->GetFixtureB()->GetBody();
			float amplitude = abs((body_A->GetLinearVelocity().x * body_A->GetLinearVelocity().y) - (body_B->GetLinearVelocity().x * body_B->GetLinearVelocity().y));
			if (amplitude >= 0.4f) {
				amplitude *= 2000;
				if (amplitude > max_friction_amplitude) {
					amplitude = max_friction_amplitude;
				}
				b2Vec2 pos = mani.points[0];
				s_emitter->setPosition(GF::convert(pos));
				s_emitter->set_is_active(true); 
				s_emitter->set_amplitude(amplitude);
				s_emitter->set_angle(30);
			}
			else {
				s_emitter->set_is_active(false);
			}
			
		}
		else if (pc >= 2) {
			b2Body *body_A = contact_ptr->GetFixtureA()->GetBody();
			b2Body *body_B = contact_ptr->GetFixtureB()->GetBody();
			float amplitude = abs(((body_A->GetLinearVelocity().x + 1) * (body_A->GetLinearVelocity().y + 1)) - ((body_B->GetLinearVelocity().x + 1) * (body_B->GetLinearVelocity().y + 1)));
			if (amplitude >= 20.0f) {
				
				amplitude *= 1500;
				if (amplitude > max_friction_amplitude) {
					amplitude = max_friction_amplitude;
				}
				b2Vec2 pos = mani.points[0];
				s_emitter->setPosition(GF::convert(pos));
				s_emitter->set_is_active(true);
				s_emitter->set_amplitude(amplitude);
				s_emitter->set_angle(30);
			}
			else {
				s_emitter->set_is_active(false);
			}
		}
	}
}

void Ship_Part_M::doUpdate(const UpdateState &us) {
	update_emitter();
}

void Ship_Part_M::if_contact(b2Contact* contact, Ship_Part_M *s_part, Actor* other_actor) {
	
	contact_ptr = contact;
	s_part->contact_ptr = contact;
	
	b2WorldManifold wm;
	contact->GetWorldManifold(&wm);


	b2Vec2 vel1 = contact->GetFixtureA()->GetBody()->GetLinearVelocity();
	b2Vec2 vel2 = contact->GetFixtureB()->GetBody()->GetLinearVelocity();
	Vector2 dif = GF::convert(b2Vec2(wm.points[0].x, wm.points[0].y)) - GF::convert(Ship_M::objects[0]->_torso->body->GetPosition());
	
	b2Vec2 total_force = vel1 - vel2;
	if (s_part->identifier != "Ship_Left_Leg" && s_part->identifier != "Ship_Right_Leg") {
		c_sound.setCoord(dif / 7);
		c_sound.play();
		
	}
	
	

	if (total_force.x >= max_vel || total_force.x <= -max_vel) {
		
		Ship_M::objects[playerIndex]->if_crashed(other_actor);
	}
	else if (total_force.y >= max_vel || total_force.y <= -max_vel) {
		
		Ship_M::objects[playerIndex]->if_crashed(other_actor);
	}
	
	//has_crashed = true;
	

}

void Ship_Part_M::end_contact(Ship_Part_M *s_part) {
	s_part->contact_ptr = nullptr;
	s_emitter->set_is_active(false);
}

void Ship_Part_M::erase() {
	
	detach();
	//body = NULL;
	
	
}

void Ship_Part_M::delete_joints(std::vector<b2Joint *> &joints) {
	if (joints.size() > 0) {
		if (body != NULL) {
			//b2JointEdge *je = body->GetJointList();

			//if (&je != NULL) {
			//while (1 < 2) {

			//if (joint != nullptr) {
			std::vector<b2Joint *> to_be_deleted;
			for (b2JointEdge *jointEdge = body->GetJointList(); jointEdge; jointEdge = jointEdge->next) {
				b2Joint *joint = jointEdge->joint;
				for (unsigned int i = 0; i < joints.size(); i++) {
					if (joints[i] == joint) {
						to_be_deleted.push_back(joint);
						joints.erase(joints.begin() + i);
						break;
					}
				}
			}

			for (unsigned int i = 0; i < to_be_deleted.size(); i++) {
				_world_ptr->getb2World()->DestroyJoint(to_be_deleted[i]);
			}
			to_be_deleted.clear();

			//	je = je->next;
			//if (je == NULL) {
			//		break;
			//}
			//}
			//else {
			//	break;
			//}

			//}
			//	}


		}
	}
	
	
}