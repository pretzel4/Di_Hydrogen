#include "Rain.h"
#include "World.h"
#include "ImageRes.h"
#define max_integers 200
#define rain_shift_to_right 50
std::vector<spRaindrop> Rain::_objects;
std::vector<spRaindrop> Rain::_to_delete;
void Raindrop::set(const Vector2 &position, const Vector2 &velocity) {
	vel = velocity;
	vel.x = -vel.x;
	setPosition(position);
}

void Raindrop::reset() {
	if (_is_active) {
		set(Vector2(_rand_store_pos->getNum(), 0), Vector2(_rand_store_vel->getNum() / 2, _rand_store_vel->getNum()));

	}
	else {
		
		if (!has_inserted) {
			_to_delete->push_back(this);
			has_inserted = true;
		}
		
	}
}

Raindrop::Raindrop(type t, random2::RandomStorage_float *sf_ptr, random2::RandomStorage_int *si_ptr, std::vector<spRaindrop> *to_delete_ptr) : _is_active(true), _to_delete(to_delete_ptr) {
	_rand_store_pos = si_ptr;
	_rand_store_vel = sf_ptr;
	reset();
	switch (t) {
	case 0: //Raindrop
		setResAnim(res::resources.getResAnim("Raindrop"));
		break;
	case 1: //Snow
		setResAnim(res::resources.getResAnim("Snow"));
		break;
	case 2: //ash
		setResAnim(res::resources.getResAnim("ash"));
	}
	
}

void Raindrop::doUpdate(const UpdateState &us) {
	setPosition(getPosition() + (vel * us.dt));
	if (getPosition().y >= getStage()->getHeight()) {
		reset();
	}
}

Rain::Rain(const Vector2 &position, void* world_ptr, bool should_start_active, type t) : last_pos(Vector2(0,0)), init_how_many_left_to_create(40) ,max_d_ticks(2), _type(t){

	setPosition(position);
	World* w_ptr = static_cast<World*>(world_ptr);
	unsigned int b_size = w_ptr->get_boundary_size();
	//Setting up the random integer storage.
	_rand_store_pos.setup(max_integers, 0, b_size * 2 + rain_shift_to_right);
	_rand_store_vel.setup(max_integers, 1,4,5);

	set_activity(should_start_active);
	
}

void Rain::doUpdate(const UpdateState &us) {
	
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

void Rain::createDrop(unsigned int fromTop) {
	spRaindrop rd = new Raindrop(_type, &_rand_store_vel, &_rand_store_pos, &_to_delete);
	_objects.push_back(rd);
	addChild(rd);
}

void Rain::setPos(const Vector2 &new_pos) {
	setPosition(new_pos);
}

void Rain::set_activity(bool is_active, type t) {
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
			if (t != same) {
				_type = t;
				
			}
			
		}
		_is_active = is_active;
	}
	
}