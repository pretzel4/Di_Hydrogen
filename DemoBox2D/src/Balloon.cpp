#include "Balloon.h"
#include "ImageRes.h"
#define terminal_velocity -30
std::vector<spBalloon> Balloon::_balloons;
Balloon::Balloon(const Vector2 &position, int max_height, unsigned int type) : _max_height(max_height), _type(type)  {
	setPosition(position);
	setResAnim(res::resources.getResAnim("Balloon"));
	addTween(Sprite::TweenAnim(getResAnim()), 2000, -1);
	_vel = Vector2(0, 0);
	setCull(true);
	setPriority(-1001);
	_balloons.push_back(this);
} 


void Balloon::done_popping(Event *ev) {
	setAlpha(0);
	for (unsigned int i = 0; i < _balloons.size(); i++) {
		if (_balloons[i] == this) {

			_balloons.erase(_balloons.begin() + i);
			break;
		}
		
	}
	_to_delete.push_back(this);
}

void Balloon::doUpdate(const UpdateState &us) {
	if (_vel.y > terminal_velocity) {
		_vel.x += accel.x * us.dt;
		_vel.y += accel.y * us.dt;
	}
	
	setPosition(getPosition() + _vel);
	
	//Checking to make sure that the balloons are popped once they get to a certain altitude.
	if (getPosition().y <= _max_height) {
		if (has_achieved_height == false) {
			spTween t = addTween(Sprite::TweenAnim(res::resources.getResAnim("Balloon_pop")), 400, 1);
			t->addDoneCallback(CLOSURE(this, &Balloon::done_popping));
			has_achieved_height = true;
		}
		

	}
}

void Balloon::onRemovedFromStage() {

}



void Balloon::clear_all() {
	for (unsigned int i = 0; i < _balloons.size(); i++) {
		_to_delete.push_back(_balloons[i]);
	}
	_balloons.clear();
	clear_cache();
}