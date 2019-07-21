#include "P_System.h"
#include <cmath>
#define initial_amplitude_magnification 3
P_System::P_System(const Vector2 &pos, float angle_radians, float amplitude, unsigned int type, spWorld world_ptr, bool is_active) {
	
	_angle = angle_radians;
	_amplitude = amplitude * initial_amplitude_magnification;
	setup(pos, convert_a_to_v(angle_radians, _amplitude), type, world_ptr, is_active);

}

P_System::P_System(const Vector2 &pos, const Vector2 &direction, unsigned int type, spWorld world_ptr, bool is_active) {
	setup(pos, direction, type, world_ptr, is_active);
}

void P_System::setup(const Vector2 &pos, const Vector2 &direction, unsigned int type, spWorld world_ptr, bool is_active) {
	//Initilizing variables.
	setPosition(pos);
	_direction = direction;
	_type = type;
	_world_ptr = world_ptr;
	
	
	_is_active = is_active;
	switch (type) {
	case 0:
		_variation = 3;
		break;
	case 1:
		_variation = 3;
		break;
	case 2:
		_variation = 3;
		break;
	case 3:
		_variation = 3;
	}

	
	setAnchor(0.5f, 0.5f);
}


Vector2 P_System::convert_a_to_v(float angle, float amplitude) {
	float y = sin(angle) * amplitude;
	float x = cos(angle) * amplitude;
	return Vector2(x, y);
}

void P_System::createParticle(const Vector2 &pos, const Vector2 &direction, unsigned int type) {
	Particle* p = new Particle(pos, direction, type);
}

void P_System::doUpdate(const UpdateState &us) {
	if (_is_active) {
		ticks += us.dt;
		if (ticks >= max_ticks) {
			switch (_type) {
			case 0: //Sparks
				
				createParticle(getPosition(), _direction, _type);
				break;
			case 1: //Confetti -One shot
				for (unsigned int i = 0; i < 30; i++) {
					createParticle(getPosition(), _direction * (i + 1), _type);
				}
				set_is_active(false);
				break;
			case 2: //Exhaust -FULL
				createParticle(getPosition(), _direction, _type);
				break;
			case 3: //Exhaust -RUNNING OUT
				createParticle(getPosition(), _direction, _type);
			}

			ticks -= max_ticks;
		}
	}

}

void P_System::set_is_active(bool is_active) {
	_is_active = is_active;
}

/*Setting*/

void P_System::set_type(unsigned int new_type) {
	_type = new_type;
}

void P_System::set_angle(float new_angle) {
	_angle = new_angle;
	_direction = convert_a_to_v(new_angle, _amplitude);
}

void P_System::set_amplitude(float new_amplitude) {
	_amplitude = new_amplitude;
	_direction = convert_a_to_v(_angle, _amplitude);
}

void P_System::set_a_and_a(const Vector2 &angle_amplitude) {
	_amplitude = angle_amplitude.y;
	_angle = angle_amplitude.x;
	_direction = convert_a_to_v(_angle, _amplitude);
}

void P_System::set_direction(const Vector2 &new_direction) {
	_angle = -1;
	_amplitude = -1;
	_direction = new_direction;
}

/*Getting*/

bool P_System::get_is_active() {
	return _is_active;
}

unsigned int P_System::get_type() {
	return _type;
}

float P_System::get_angle() {
	return _angle;
}

float P_System::get_amplitude() {
	return _amplitude;
}

Vector2 P_System::get_direction() {
	return _direction;
}