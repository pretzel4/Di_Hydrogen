#include "Ship_Engine.h"
#include "ImageRes.h"
#include "GF.h"
#include "Noise.h"
#include "Ship_M.h"
#define scale 1
#define ship_fuel_add 200
#define engine_fade_out_time 2000 //This is the amount of time it takes for the engine's sounds to fade out when calling stop_sound().
Ship_Engine::Ship_Engine() {
	Exhaust->set_type(2);
}

Ship_Engine::Ship_Engine(Vector2 pos, int Playerindex, std::vector<b2Joint *> &joints, float *ship_fuel, float max_ship_fuel) {
	
	Exhaust = new P_System(pos, Vector2(0, 0), 2, _world_ptr, false);
	addChild(Exhaust);
	_fuel = ship_fuel;
	_max_fuel = max_ship_fuel;
	playerIndex = Playerindex;
	ship_joints = &joints;
	
	//_in.si->setVolume(0.2f);
	_impulse = 200 * 5;
	is_first_time_fe = true;
	
	setup("Ship_Engine", pos, SHIP_PART, DEFAULT_MASK, e_engine);
	
	//Defining the ship's sound instance.
	
}

void Ship_Engine::doUpdate(const UpdateState &us) {
	Vector2 dif = getPosition() - Ship_M::objects[0]->_torso->getPosition();
	Vector2 b332(dif / 20);
	_in.setCoord(b332);
	
	if (is_on() == true || is_on() == true) {
		if (!hfr) {
			//_in._settings.pitch(random2::randomrange_float(0.1f, 1));
			switch (random2::randomrange(0, 2)) {
			case 0:
				_in._ID = Noise::st::Rocket_Fire0;
				break;
			
			};
			
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
			//_in.si->fadeOut(600);
		}

		sound_toggle = true;
	}
	update_emitter();
	if (_is_on == true && !_world_ptr->get_is_paused()) {
		
		_impulse = (200 * 5.5f) - (ship_fuel_add * (*_fuel / _max_fuel));
		b2Vec2 forceDirection = body->GetWorldVector(b2Vec2(0, 1));
		forceDirection = b2Vec2(forceDirection.x * -_impulse, forceDirection.y * -_impulse);
		body->ApplyForceToCenter(forceDirection, true);
		if (*_fuel < _max_fuel / 2) {
			Exhaust->set_type(3); //Below half fuel
		}
		else {
			Exhaust->set_type(2); //Above half fuel
		}
		Exhaust->set_is_active(true);
		
		Exhaust->setPosition(getPosition());
	
		//_in.resume();
		Exhaust->set_direction(GF::convert(-b2Vec2(forceDirection.x / 2, forceDirection.y / 2)));
		
		//if (!_in.si->isPlaying() && !is_first_time) {
		//	_in.si->setCoord(getPosition(), 0.0f);
		//	_in.si->fadeIn(0);
			
		//}
		
	}
	else {
		//_in.pause();
	}
}

void Ship_Engine::set_state(bool new_state) {
		
		if (new_state == true && _is_on == false) {
			Exhaust->set_is_active(true);
			addTween(Sprite::TweenAnim(res::resources.getResAnim("Ship_Engine_Ani")), 500, -1);
			//addTween(Sprite::TweenAnim(res::resources.getResAnim("Ship_Engine_Ani"), 500, 1));
			/*if (!is_first_time_fe) {
				if (is_first_time) {
					_in.resume();
					GF::println_va(is_first_time);

				}
			}
			else {
				_in.play();
				is_first_time_fe = false;
			}*/
			
			//log::messageln("G");
			is_first_time = false;
			_is_on = true;
			
		}
		else if (new_state == false && _is_on == true) {
			//log::messageln("T");
			is_first_time = true;
			Exhaust->set_is_active(false);
			removeTweens();
			setResAnim(res::resources.getResAnim("Ship_Engine"));
			_is_on = false;
			//_in.pause();
		}
	
		//log::messageln("LACTOSE INTOLERANCE");
	
	
}

//This is called when the object is remove from the stage/deleted.
void Ship_Engine::onRemovedFromStage() {
	if (!_has_been_deleted) {
		delete_joints(*ship_joints);
		s_emitter->detach();
		c_sound.destroy();
		Exhaust->detach();
		//Exhaust.~intrusive_ptr();
		_in.destroy();
		_world_ptr->getb2World()->DestroyBody(body);
		_has_been_deleted = true;
	}
	
}

void Ship_Engine::stop_sound() {
	_in.si->fadeOut(engine_fade_out_time);
}