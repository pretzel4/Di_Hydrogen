#include "Hot_Balloon.h"
#include "ImageRes.h"
#include "World.h"
#define terminal_velocity 500
std::vector<spHot_Balloon> Hot_Balloon::_balloons;

void Hot_Balloon::onRemovedFromStage() {

}
	
Hot_Balloon::Hot_Balloon(const Vector2 &position, float death_line, unsigned int type) : x_speed(0.04f) {
	setPosition(position);
	setResAnim(res::resources.getResAnim("Hot_Air_Balloon"));
	//addTween(Sprite::TweenAnim(getResAnim()), 2000, -1);
	//_vel = Vector2(0, 0);
	x_death = death_line;
	setCull(true);
	/*THIS HAS SOME WEIIIRRRRDD PROPERTIES, REALLY, I HAVE NO IDEA 
	WHY THE GAME CRASHES WHEN THIS IS LOWER THAN -9000 AND THERE IS A FORCEFIELD IN THE GAME.
	KEEP IT AT -1001 OR ELSE FACE CONSEQUENCES. unless you figured out what was wrong.*/
	setPriority(-8500);
	_balloons.push_back(this);
	/*setPosition(position);
	setResAnim(res::resources.getResAnim("Balloon"));
	addTween(Sprite::TweenAnim(getResAnim()), 2000, -1);
	_vel = Vector2(0, 0);
	setCull(true);
	setPriority(-1001);
	_balloons.push_back(this);*/


	/*setPosition(position);
	setResAnim(res::resources.getResAnim("Hot_Air_Balloon"));
	x_death = death_line; 
	setCull(true);
	setPriority(-9000);
	_balloons.push_back(this);*/
}

void Hot_Balloon::doUpdate(const UpdateState &us) {
	/*if (_vel.y > terminal_velocity) {
		_vel.x += accel.x * us.dt;
		_vel.y += accel.y * us.dt;
	}

	setPosition(getPosition() + _vel);

	//Checking to make sure that the balloons are popped once they get to a certain altitude.
	if (getPosition().y <= _max_height) {
		if (has_achieved_height == false) {
			setAlpha(0);
			for (unsigned int i = 0; i < _balloons.size(); i++) {
				if (_balloons[i] == this) {

					_balloons.erase(_balloons.begin() + i);
					break;
				}

			}
			_to_delete.push_back(this);
			has_achieved_height = true;
		}


	}*/
	setPosition(Vector2(getPosition().x - (x_speed * us.dt), getPosition().y));
	if (getPosition().x < x_death) {
			//Putting itself into the delete vector, for deletion later.
			setAlpha(0);
			
			for (unsigned int i = 0; i < _balloons.size(); i++) {
				if (_balloons[i] == this) {
					_balloons.erase(_balloons.begin() + i);
					break;
				}
			}
			_to_delete.push_back(this);		
	}
}

void Hot_Balloon::clear_all() {
	for (unsigned int i = 0; i < _balloons.size(); i++) {
		_to_delete.push_back(_balloons[i]);
	}
	_balloons.clear();
	clear_cache();
	/*for (unsigned int i = 0; i < _balloons.size(); i++) {
		_to_delete.push_back(_balloons[i]);
	}
	_balloons.clear();
	clear_cache();*/
}