#include "Wind.h"

Wind::Wind() {

}

Wind::Wind(const b2Vec2 &init_current, bool is_active) : _is_active(is_active), _current(init_current) {
	_current = init_current;
	_is_active = is_active;
}

void Wind::setCurrent(const b2Vec2 &current) {
	_current = current;
}

void Wind::set_activity(bool is_active) {
	_is_active = is_active;
}

void Wind::simulate(b2Body *to_change) {
	if (_is_active) {
		to_change->ApplyForceToCenter(_current, true);

	}
}

bool Wind::get_is_active() {
	return _is_active;
}

b2Vec2 Wind::get_current() {
	return _current;
}

void Wind::set(const b2Vec2 &current, bool is_active) {
	
	_current = current;
	_is_active = is_active;
}