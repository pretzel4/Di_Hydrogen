#include "Ship_M.h"
#include "ImageRes.h"
#include "GF.h"
#include "Ship_R_Leg.h"
#include "Ship_L_Leg.h"
#include "Ship_Engine.h"
#include "Text.h"
#include "Fuel_Gauge.h"
#include "Noise.h"
#include "Platform.h"
#define max_fuel 100
#define engine_consumption 0.09
#define atmosphere_density 0.01//0.009
#define death_wait_time 5000
#define IC_Line_fadeTime 2000//3000
#define engine_sound_volume 50 //100 is max
std::vector<spShip_M> Ship_M::objects;
Vector2 test(200, -500);

void IC_Line::shift(float shift_amount) {
	for (unsigned int i = 0; i < _lines.size(); i++) {
		TPI_Line* ptr = _lines[i].get();
		ptr->setP_Transform(Vector2(ptr->getP_Transform().x, ptr->getP_Transform().y + shift_amount));
	}
}

void IC_Line::destroy(spTPI_Line to_delete) {
	for (unsigned int i = 0; i < _lines.size(); i++) {
		if (_lines[i] == to_delete) {
			_lines[i]->detach();
			_lines.erase(_lines.begin() + i);
			break;
		}
	}
}

void IC_Line::tick() {
	for (unsigned int i = 0; i < _to_destroy.size(); i++) {
		if (_to_destroy[i]->has_faded_out()) {
			destroy(_to_destroy[i]);
		}
	}
	_to_destroy.clear();
}


void IC_Line::fadeOut_and_delete(spTPI_Line line) {
	try {
		line->fadeOut(IC_Line_fadeTime);
	}
	catch (int e) {
		std::cout << "Excetion occured at IC_Line fadeOut_and_delete: " + e << std::endl;
	}
	
	//line->setAlpha(255);
	_to_destroy.push_back(line);
}

spTPI_Line IC_Line::create(const Vector2 &position, float radius, unsigned int type) {
	spTPI_Line e = new TPI_Line(_position, position, radius, type);
	_lines.push_back(e);
	_world_ptr->addChild(e);
	return e;
}

void IC_Line::delete_all() {
	for (unsigned int i = 0; i < _lines.size(); i++) {
		_lines[i]->detach();
	}
	_lines.clear();
	_to_destroy.clear();
}



IC_Line::IC_Line(const Vector2 &position, World* world_ptr) : _position(position), _world_ptr(world_ptr){
	
}

Ship_M::Ship_M(World* world, const Vector2& pos, control_types c_type, float scale) : line_controller(IC_Line(c_pos, world)){
	setPosition(pos);
	setAnchor(0.5f, 0.5f);
	setScale(scale);
	

	setSize(getSize());
	
	_world_ptr = world;
	playerIndex = objects.size();
	objects.push_back(this);
	switch (c_type) {
	case 0: //a_d
		controls[0] = 97;//97
		controls[1] = 100;
		break;
	case 1:
		controls[0] = SDLK_LEFT;
		controls[1] = SDLK_RIGHT;
		break;
	}
	start_pos = pos;

	//attachTo(getStage());
	_world_ptr->addChild(this);
	
	
	fuel = max_fuel;


	create_parts();
	//_r_engine->is_settled = true;
	//_l_engine->is_settled = true;
	

	vel = body->GetLinearVelocity();
}

void Ship_M::update_aerodynamics() {
	float total_area = 0;
	for (unsigned int i = 0; i < _parts.size(); i++) {
		total_area += _parts[i]->getWidth() * _parts[i]->getHeight();
	}
	total_area += _r_engine->getWidth() * _r_engine->getHeight();
	total_area += _l_engine->getWidth() * _l_engine->getHeight();
	total_area += _torso->getWidth() * _torso->getHeight();
	b2Vec2 l_vel = body->GetLinearVelocity();
	float vel2 = l_vel.Normalize();
	float drag_mag = atmosphere_density * vel2 * vel2;
	b2Vec2 dragForce = drag_mag * -l_vel;
	body->ApplyForceToCenter(dragForce, true);
	float angular_drag = 0.02 * -body->GetAngularVelocity();
	body->ApplyTorque(angular_drag, true);
}

void Ship_M::doUpdate(const UpdateState& us) {
	line_controller.tick();
	c_pos = GF::convert(body->GetPosition());
	/*if (_r_engine->is_on() == true || _l_engine->is_on() == true) {
		if (!hfr) {
			_in._settings.pitch(random2::randomrange_float(0.1, 2));
			
			sound_toggle = false;
			_in.play();
			hfr = true;
		}
		else {
			//if (_in.si->isPlaying() == true) {
				//log::messageln("Playing");
				//if (sound_toggle == true) {
				//	_in.si->resume();
				//	sound_toggle = false;
				//}
				
			//}
			
		}
		
	}
	else {
		hfr = false;
		
		if (!sound_toggle) {
			_in.si->stop();
		}
		
		sound_toggle = true;
	}*/

	if (is_waiting == false) {
		//If any of the parts have been destroyed, then it should reset the world within a certain amount of time.
		if (_torso->getParent() == NULL || _r_engine->getParent() == NULL || _l_engine->getParent() == NULL || _r_engine->getParent() == NULL || _l_engine->getParent() == NULL || _parts[0]->getParent() == NULL || _parts[1]->getParent() == NULL
			|| has_crashed == true || fuel <= 0) {
			_is_in_working_order = false;
			if (should_destroy_joints == true) {
				for (unsigned int i = 0; i < _joints.size(); i++) {
					
					if (_joints[i]->IsActive() == true) {
						_world_ptr->getb2World()->DestroyJoint(_joints[i]);
					}

				}
				_joints.clear();
				should_destroy_joints = false;
			}
			//update_aerodynamics();
			
			_world_ptr->gui->open_restart_menu(_world_ptr.get());
			
			//spTween t = addTween(TweenDummy(), death_wait_time);
			//t->addDoneCallback(CLOSURE(this, &Ship_M::tween_callback));
			
		}
		else {
			if (_world_ptr->gui->is_restart_menu_open() == true) {
				_is_in_working_order = true;
				_world_ptr->gui->close_restart_menu();
			}
		}
		
		const uint8* data = SDL_GetKeyboardState(NULL);
		//if (has_reset == false) {
		
		if (_torso->getParent() != NULL) {
			
			if (!_world_ptr->get_is_paused()) {
				
				//setPosition(GF::convert(body->GetPosition()) - Vector2(12, 12));
				//GF::print_va(getPosition().x);
				//Applying aerodynamic drag

				update_aerodynamics();
				_world_ptr->set_cam_pos(-_torso->getPosition() + (getStage()->getSize() / 2));
				//_world_ptr->setPosition(Vector2(-GF::convert_to_Oxy(body->GetPosition().x) + (getStage()->getWidth() / 2), -GF::convert_to_Oxy(body->GetPosition().y) + (getStage()->getHeight() / 2)));
				//_world_ptr->set_cam_pos(Vector2(-GF::convert_to_Oxy(body->GetPosition().x) + (getStage()->getWidth() / 2), -GF::convert_to_Oxy(body->GetPosition().y) + (getStage()->getHeight() / 2)));
				//_world_ptr->set_cam_pos(Vector2(-_torso->getPosition().x, -_torso->getPosition().y), start_pos);
				//}
				vel = body->GetLinearVelocity();

				if (fuel > 0) {
					if (has_crashed == false) {
						//-=-=-=-=-=-=-=-=-=-
						//KEY_BOARD CONTROLS -- uncomment this if you are using win32, linux or mac etc.
						//-=-=-=-=-=-=-=-=-=-

						if (data[SDL_GetScancodeFromKey(controls[1])]) {
							//addTween(Sprite::TweenAnim(getResAnim()), 500, 1);
							//vel.x += speed *(us.dt / 1000.0f);

							if (_r_engine->getParent() != NULL) {


								_r_engine->set_state(true);
								fuel -= engine_consumption;
								if (fuel <= 0) {
									Noise::play_ID(Noise::st::Run_Out_Of_Fuel, Noise::player::sfx);
								}
								

							}

						}
						else {
							if (_r_engine->getParent() != NULL) {
								_r_engine->set_state(false);
							}
						}
						if (data[SDL_GetScancodeFromKey(controls[0])]) {
							if (_l_engine->getParent() != NULL) {
								_l_engine->set_state(true);
								fuel -= engine_consumption;
								if (fuel <= 0) {
									Noise::play_ID(Noise::st::Run_Out_Of_Fuel, Noise::player::sfx);
								}
							}

						}
						else {
							if (_l_engine->getParent() != NULL) {
								_l_engine->set_state(false);
							}
						}
						
						//-=-=-=-=-=-=-=-
						//CLICK_CONTROLS --Uncomment this if you are using ios or android.
						//-=-=-=-=-=-=-=-
						/*if (_r_engine->getParent() != NULL) {
						if (_r_engine_switch_is_on == true) {
						_r_engine->set_state(true);
						fuel -= (engine_consumption / 2);
						_torso->fg->setFuel(fuel);
						}
						else {
						_r_engine->set_state(false);
						}
						}
						if (_l_engine->getParent() != NULL) {
						if (_l_engine_switch_is_on == true) {
						_l_engine->set_state(true);
						fuel -= (engine_consumption / 2);
						_torso->fg->setFuel(fuel);
						}
						else {
						_l_engine->set_state(false);
						}
						}*/


					}
					else {
						if (should_destroy_joints == true) {
							for (unsigned int i = 0; i < _joints.size(); i++) {
								if (_joints[i]->IsActive() == true) {
									_world_ptr->getb2World()->DestroyJoint(_joints[i]);
								}

							}
							_joints.clear();
							should_destroy_joints = false;
						}
					}


			}
			else {
				_r_engine->set_state(false);
				_l_engine->set_state(false);


			}
			
			fuel += _torso->update_animation(us);
			if (fuel > max_fuel) {
				fuel = max_fuel;
			}
			_torso->setFuel(fuel);

			}
			
		}
		
		//_world_ptr->setPosition(Vector2(getPosition().x + (getStage()->getSize().x / 2), getPosition().y + (getStage()->getSize().y / 2)));
		
		//_world_ptr->setPosition(getPosition().x + vel.x, getPosition().y + vel.y);
		//body->SetAngularVelocity(body->GetAngularVelocity());
		
		
		
		//_world_ptr->scroll_coords.y = getPosition().y - (getStage()->getHeight() / 2);
		//_world_ptr->set_Y_origin(getPosition().y);
		
		
		
		if (data[SDL_GetScancodeFromKey(SDLK_0)]) {
			_world_ptr->set_RESET_ALL();
		}
		if (data[SDL_GetScancodeFromKey(SDLK_1)]) {
			GF::println_va(Platform::objects[0]->getBody()->GetPosition().y);
		}
		if (data[SDL_GetScancodeFromKey(SDLK_3)]) {
			ObjectBase::dumpCreatedObjects();
		}
	}
	else {
		if (_torso->getParent() != NULL) {
			_world_ptr->set_cam_pos(-_torso->getPosition() + (getStage()->getSize() / 2));

		}
	}
	
		
	
	//log::messageln(std::to_string(getPosition().y).c_str());

	//_world_ptr->set_origin();


	//getStage()->setPosition(Vector2(getPosition().x - (getStage()->getSize().x / 2), getPosition().y - (getStage()->getSize().y / 2)));
}

void Ship_M::fade_out_engine_sound() {
	/*if (_r_engine->getParent() != NULL) {
		_r_engine->stop_sound();
	}
	if (_l_engine->getParent() != NULL) {
		_l_engine->stop_sound();
	}*/
}

void Ship_M::deallocate_parts() {
	if (_torso->getParent() != NULL) {
		_torso->erase();
		_torso = NULL;
	}
	for (unsigned int i = 0; i < _parts.size(); i++) {
		if (_parts[i]->getParent() != NULL) {
			_parts[i]->erase();
			_parts[i] = NULL;
		}

	}
	_parts.clear();
	if (_r_engine->getParent() != NULL) {
		_r_engine->erase();
		_r_engine = NULL;
	}
	if (_l_engine->getParent() != NULL) {
		_l_engine->erase();
		_l_engine = NULL;
	}
}

void Ship_M::reset() {
		
		_is_in_working_order = true;
	
		
		fuel = max_fuel;
		//fg->setFuel(fuel);
		is_waiting = true;
		//body->SetLinearVelocity(b2Vec2(0, 0));
		//body->SetAngularVelocity(0);
		removeTweens();
		spTween tween = _world_ptr->addTween(Actor::TweenPosition(-start_pos + (getStage()->getSize() / 2)), 300, 1, false, 0, Tween::ease_inBack);
		tween->setDoneCallback(CLOSURE(this, &Ship_M::end_reset));
		
		has_crashed = false;
		
		//body->SetTransform(GF::convert(start_pos, GF::get_G_Scale()), 0);
		
		/*Erasing all parts::START*/
		/*for (unsigned int i = 0; i < _joints.size(); i++) {
			if (_joints[i] != NULL) {
				_world_ptr->getb2World()->DestroyJoint(_joints[i]);
			}
		}*/
		//_joints.clear();
		deallocate_parts();
		
		create_parts();
		set_active(false);
		//Vector4 jd_joints(30, 16, 0, 10);
		//Vector4 jd1_joints(2, 16, 32, 10);
		//Vector4 jd2_joints(26, 30, 16, 16);
		//Vector4 jd3_joints(6, 30, 16, 16);
		
	
		

}

void Ship_M::end_reset(Event *event) {
	
	is_waiting = false;
	set_active(true);
}

void Ship_M::set_active(bool is_active) {
	
	for (unsigned int i = 0; i < _parts.size(); i++) {
		_parts[i]->body->SetActive(is_active);
	}
	_r_engine->body->SetActive(is_active);
	_l_engine->body->SetActive(is_active);
	body->SetActive(is_active);
	
}

inline void Ship_M::create_parts() {

	
	_torso = new Ship_Torso(start_pos, playerIndex, max_fuel, _joints);
	body = _torso->body;
	//_torso->attachTo(getStage());
	//_world_ptr->addChild(_torso);
	
	Vector4 jd_joints(30 + 2, 16 + 2, 0, 10); //Right leg joints
	Vector4 jd1_joints(2 - 2, 16 + 2, 32, 10); //Left leg joints
	Vector4 jd2_joints(26, 30, 16, 16); //Right engine joints
	Vector4 jd3_joints(6, 30, 16, 16); //Left engine joints
	spShip_R_Leg R_leg = new Ship_R_Leg(Vector2(start_pos.x + jd_joints[0] - jd_joints[2], start_pos.y + jd_joints[1] - jd_joints[3]), playerIndex, _joints);
	_parts.push_back(R_leg.get());
	spShip_L_Leg L_leg = new Ship_L_Leg(Vector2(start_pos.x - jd1_joints[2] + jd1_joints[0], start_pos.y + jd1_joints[1] - jd1_joints[3]), playerIndex, _joints);
	_parts.push_back(L_leg.get());
	spShip_Engine R_engine = new Ship_Engine(Vector2(R_leg->getPosition().x + jd2_joints[0] - jd2_joints[2], R_leg->getPosition().y + jd2_joints[1] - jd2_joints[3]), playerIndex, _joints, &fuel, max_fuel);
	_r_engine = R_engine.get();
	spShip_Engine L_engine = new Ship_Engine(Vector2(L_leg->getPosition().x + jd3_joints[0] - jd3_joints[2], L_leg->getPosition().y + jd3_joints[1] - jd3_joints[3]), playerIndex, _joints, &fuel, max_fuel); // 36
	_l_engine = L_engine.get();
	//(((jd_joints[1] - jd_joints[3]) + jd2_joints[1]) - jd2_joints[3]))
	//start_pos.y + (((jd1_joints[1] - jd1_joints[3]) + jd3_joints[1]) - jd3_joints[3])
	//start_pos.x + jd_joints[0] + jd2_joints[0] - jd2_joints[2]
	//start_pos.x - (26 + 16)
	//R_leg->attachTo(getStage());
	//L_leg->attachTo(getStage());
	//_r_engine->attachTo(getStage());
	//_l_engine->attachTo(getStage());
	R_leg->abstract_ptr = this;
	L_leg->abstract_ptr = this;
	_r_engine->abstract_ptr = this;
	_l_engine->abstract_ptr = this;
	_world_ptr->addChild(R_leg);
	_world_ptr->addChild(L_leg);
	_world_ptr->addChild(_r_engine);
	_world_ptr->addChild(_l_engine);
	//Vector2 r_leg_pos(30,);
	//R_Leg to Main
	b2WeldJointDef jd;
	jd.collideConnected = false;
	jd.type = e_weldJoint;

	jd.localAnchorA = b2Vec2(GF::convert(Vector2(jd_joints[0], jd_joints[1]), GF::get_G_Scale()));
	jd.localAnchorB = b2Vec2(GF::convert(Vector2(jd_joints[2], jd_joints[3]), GF::get_G_Scale()));
	jd.bodyA = body;
	jd.bodyB = R_leg->body;
	b2WeldJoint *wj = (b2WeldJoint*)_world_ptr->getb2World()->CreateJoint(&jd);
	_joints.push_back(wj);
	//L_Leg to Main
	b2WeldJointDef jd2;
	jd2.collideConnected = false;
	jd2.type = e_weldJoint;
	jd2.localAnchorA = b2Vec2(GF::convert(Vector2(jd1_joints[0], jd1_joints[1]), GF::get_G_Scale()));
	jd2.localAnchorB = b2Vec2(GF::convert(Vector2(jd1_joints[2], jd1_joints[3]), GF::get_G_Scale()));
	jd2.bodyA = body;
	jd2.bodyB = L_leg->body;
	b2WeldJoint *wj2 = (b2WeldJoint*)_world_ptr->getb2World()->CreateJoint(&jd2);
	_joints.push_back(wj2);
	//R_Engine to R_Leg
	b2WeldJointDef jd3;
	jd3.collideConnected = false;
	jd3.type = e_weldJoint;
	jd3.localAnchorA = b2Vec2(GF::convert(Vector2(jd2_joints[0], jd2_joints[1]), GF::get_G_Scale()));
	jd3.localAnchorB = b2Vec2(GF::convert(Vector2(jd2_joints[2], jd2_joints[3]), GF::get_G_Scale()));
	jd3.bodyA = R_leg->body;
	jd3.bodyB = R_engine->body;
	b2WeldJoint *wj3 = (b2WeldJoint*)_world_ptr->getb2World()->CreateJoint(&jd3);
	_joints.push_back(wj3);
	//L_Engine to L_Leg
	b2WeldJointDef jd4;
	jd4.collideConnected = false;
	jd4.type = e_weldJoint;
	jd4.localAnchorA = b2Vec2(GF::convert(Vector2(jd3_joints[0], jd3_joints[1]), GF::get_G_Scale()));
	jd4.localAnchorB = b2Vec2(GF::convert(Vector2(jd3_joints[2], jd3_joints[3]), GF::get_G_Scale()));
	jd4.bodyA = L_leg->body;
	jd4.bodyB = L_engine->body;
	b2WeldJoint *wj4 = (b2WeldJoint*)_world_ptr->getb2World()->CreateJoint(&jd4);
	_joints.push_back(wj4);
}

bool Ship_M::engine_check() {
	
	
	//log::messageln("Engine checking!");
	if (_l_engine->is_settled == true && _r_engine->is_settled == true) {
		return true;
	}
	return false;
}

void Ship_M::refuel() {
	_torso->fg->refill(max_fuel);
}

Ship_M::~Ship_M() {
	/*log::messageln("Calling Ship_M deconstructor!");
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (objects[i] == nullptr) {
			objects.erase(objects.begin() + i);
		}
	}*/
}

bool Ship_M::are_both_engines_not_settled() {
	if (_r_engine->is_settled == false && _l_engine->is_settled == false) {
		return true;
	}
	return false;
}

void Ship_M::if_crashed(Actor* other_actor) {
	if (other_actor->getName() != "BHV") {
		Noise::play_ID(Noise::st(random2::randomrange(1, 3)), Noise::player::sfx);
	}
	
	line_controller.delete_all();
	should_destroy_joints = true;
	has_crashed = true;
}

Vector2 Ship_M::get_dif_from_start() {
	return _torso->getPosition() - start_pos;
}

void Ship_M::click_engine_right(Event *event) {
	_r_engine_switch_is_on = true;
}

void Ship_M::click_engine_left(Event *event) {
	_l_engine_switch_is_on = true;
}

void Ship_M::unclick_engine_right(Event* event) {
	_r_engine_switch_is_on = false;
}

void Ship_M::unclick_engine_left(Event* event) {
	_l_engine_switch_is_on = false;
}

bool Ship_M::is_in_working_order() {
	return _is_in_working_order;
}

void Ship_M::set_is_waiting(bool new_state) {
	is_waiting = new_state;
}

void Ship_M::deallocate() {
	for (unsigned int i = 0; i < objects.size(); i++) {
		objects[i]->destroy();
		objects[i]->detach();
	}
	
	
	objects.clear();
	
}

spShip_M Ship_M::allocate(World* world_ptr) {
	return new Ship_M(world_ptr, Vector2(0, -60), Ship_M::control_types::a_d);
	
}

void Ship_M::destroy() {
	for (unsigned int i = 0; i < _parts.size(); i++) {
		_parts[i]->detach();
	}
	_parts.clear();
	_r_engine->detach();
	_l_engine->detach();
	_torso->detach();
}