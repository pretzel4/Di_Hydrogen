#include "Bubble.h"
#include "World.h"
#include "ImageRes.h"
#define max_integers 200
#define rain_shift_to_right 50
std::vector<spDroplet> Bubble::_objects;
std::vector<spDroplet> Bubble::_to_delete;
void Droplet::set(const Vector2 &position, const Vector2 &velocity) {
	vel = velocity;
	vel.x = -vel.x;
	setPosition(position);
}

void Droplet::reset() {
	if (_is_active) {
		set(Vector2(getStage()->getWidth() + rain_shift_to_right, _rand_store_pos->getNum()), Vector2(_rand_store_vel->getNum(), 0));

	}
	else {

		if (!has_inserted) {
			_to_delete->push_back(this);
			has_inserted = true;
		}

	}
}

Droplet::Droplet(random2::RandomStorage_float *sf_ptr, random2::RandomStorage_int *si_ptr, std::vector<spDroplet> *to_delete_ptr, bubble_type::type type) : _is_active(true), _to_delete(to_delete_ptr) {
	_rand_store_pos = si_ptr;
	_rand_store_vel = sf_ptr;

	reset();
	switch (type) {
	case 0:
		setResAnim(res::resources.getResAnim("Bubble"));
		break;
	case 1:
		switch (static_cast<int>(random2::range(3, _rand_store_pos->getNum(), _rand_store_pos->getMinNum(), _rand_store_pos->getMaxNum()))) {
		case 0:
			setResAnim(res::resources.getResAnim("BreezeFlower_0"));
			break;
		case 1:
			setResAnim(res::resources.getResAnim("BreezeFlower_1"));
			break;
		case 2:
			setResAnim(res::resources.getResAnim("BreezeFlower_2"));
			break;
		}
		break;
	case 2:
		setResAnim(res::resources.getResAnim("Bubble"));
		setColor(random2::randomrange(0, 255), random2::randomrange(0, 255), random2::randomrange(0, 255), 255);
		break;
	}
}

void Droplet::doUpdate(const UpdateState &us) {
	setPosition(getPosition() + (vel * us.dt));
	if (getPosition().x <= -getWidth()) {
		reset();
	}
}

Bubble::Bubble(const Vector2 &position, void* world_ptr, bool should_start_active, bubble_type::type type) : last_pos(Vector2(0, 0)), init_how_many_left_to_create(60), max_d_ticks(2) {

	setPosition(position);
	_type = type;
	World* w_ptr = static_cast<World*>(world_ptr);
	
	//Setting up the random integer storage.
	_rand_store_pos.setup(max_integers, 0, getStage()->getHeight());
	_rand_store_vel.setup(max_integers, 1, 6, 10);

	set_activity(should_start_active);

}

void Bubble::doUpdate(const UpdateState &us) {

	if (_is_active) {
		if (how_many_left_to_create > 0) {
			d_ticks += us.dt;
			if (d_ticks >= max_d_ticks) {
				createDrop(0);
				how_many_left_to_create--;
				d_ticks -= max_d_ticks;
			}
		}
	}
	else {
		for (unsigned int i = 0; i < _to_delete.size(); i++) {
			//Finding and erasing the pointer in the _objects vector.
			for (unsigned int b = 0; b < _objects.size(); b++) {
				if (_to_delete[i] == _objects[b]) {
					_objects.erase(_objects.begin() + b);
					break;
				}
			}
			//Detaching
			_to_delete[i]->detach();
			//_to_delete[i].~intrusive_ptr();
		}
		_to_delete.clear();
	}


}

void Bubble::createDrop(unsigned int fromTop) {
	spDroplet rd = new Droplet(&_rand_store_vel, &_rand_store_pos, &_to_delete, _type);//Vector2(_rand_store_pos.getNum(), fromTop), Vector2(_rand_store_vel.getNum(), _rand_store_vel.getNum()));
	_objects.push_back(rd);
	addChild(rd);
}

void Bubble::setPos(const Vector2 &new_pos) {
	setPosition(new_pos);
}

void Bubble::set_activity(bool is_active, bubble_type::type new_type) {
	if (new_type != -1) {
		_type = new_type;
	}
	if (_is_active != is_active) {
		switch (is_active) {
		case 0: //Is not active
			for (unsigned int i = 0; i < _objects.size(); i++) {
				_objects[i]->_is_active = false;
			}

			break;
		case 1: //Is active
			for (unsigned int i = 0; i < _objects.size(); i++) {
				_objects[i]->detach();
				//_objects[i].~intrusive_ptr();
			}
			_objects.clear();
			_to_delete.clear();
			how_many_left_to_create = init_how_many_left_to_create;
			d_ticks = 0;

		}
		_is_active = is_active;
	}

}