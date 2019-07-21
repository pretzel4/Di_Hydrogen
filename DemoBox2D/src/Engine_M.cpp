#include "Engine_M.h"
Engine_M::Engine_M() {
	
	
	setName("EM");
}
bool Engine_M::is_on() {
	return _is_on;
}

void Engine_M::toggle() {
	if (_is_on == false) {
		set_state(true);
	}
	else {
		set_state(false);
	}
}

void Engine_M::set_state(bool new_state) {
	if (new_state) {
		if (!_world_ptr->get_is_paused()) {
			_is_on = true;
		}
	}
	else {
		_is_on = false;
	}
	
}

void Engine_M::ifcontact_platform_start() {
	is_settled = true;

	//ship_ptr->engine_check(platform);
}

void Engine_M::ifcontact_platform_end() {
	is_settled = false;
//	ship_ptr->engine_check(platform);
}
/*void Engine_M::BeginContact(b2Contact* contact) {
	log::messageln("Made contact");
	b2Vec2 vel1 = contact->GetFixtureA()->GetBody()->GetLinearVelocity();
	b2Vec2 vel2 = contact->GetFixtureB()->GetBody()->GetLinearVelocity();
	b2Vec2 total_force = vel1 + vel2;
	//has_crashed = true;
	if (total_force.x >= max_vel || total_force.x <= -max_vel) {
		log::messageln("Crashed, X_axis");
		has_crashed = true;
	}
	else if (total_force.y >= max_vel || total_force.y <= -max_vel) {
		log::messageln("Crashed, Y_axis");
		has_crashed = true;
	}
	void* fixA = contact->GetFixtureA()->GetBody()->GetUserData();
	if (fixA == static_cast<Platform *>(fixA)) {
		log::messageln("Has landed on platform");
		is_landed_on_platform = true;
	}
	else {
		log::messag
		is_landed_on_platform = false;
	}
}*/