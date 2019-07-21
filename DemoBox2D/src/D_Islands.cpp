#include "D_Islands.h"
#include "ImageRes.h"
#include "World.h"
#define how_many_islands 7
#define fade_time 3000
Island::Island(const Vector2 &position, unsigned int sprite_index) {
	setPosition(position);
	setResAnim(getSpri(sprite_index));
	setSize(getResAnim()->getSize());
	
}

D_Islands::D_Islands(const Vector2 &position, void* world_ptr) : start_pos(position) {
	_world_ptr = world_ptr;
	setPosition(position);
	
}

void D_Islands::create_island(const Vector2 &position, unsigned int index) {
	spIsland i = new Island(position, index);
	addChild(i);
	_objects.push_back(i);
}

void D_Islands::setup(bool should_fade) {
	if (should_fade) {
		fadeIn();
	}
	unsigned int iter = 0;
	World* w_ptr = static_cast<World*>(_world_ptr);
	while (1 < 2) {
		if (iter < how_many_islands) {
			//This is the random number that is used for finding what sprite the new island is going to use.
			unsigned int rand = random2::randomrange(0, 3);
			bool should_create = true;
			float x = random2::randomrange(0, w_ptr->get_boundary_size());
			float y = random2::randomrange(0, getStage()->getHeight());

			RectF new_rect(Vector2(x, y), getSpri(rand)->getSize());
			for (unsigned int i = 0; i < _objects.size(); i++) {
				//Checking to make sure that no islands overlap.
				if (new_rect.isIntersecting(_objects[i]->getSrcRect())) {
					should_create = false;
					break;
				}
			}

			if (should_create) {
				create_island(new_rect.pos, rand);
				iter++;
			}
		}
		else {
			break;
		}
		
		
	}
}

void D_Islands::fadeIn() {
	setAlpha(0);
	addTween(Actor::TweenAlpha(255), fade_time);

}

void D_Islands::fadeOut() {
	addTween(Actor::TweenAlpha(0), fade_time);
}

bool D_Islands::get_is_active() {
	if (getAlpha() != 0) {
		return true;
	}
	else {
		return false;
	}
}

void D_Islands::deallocate() {
	for (unsigned int i = 0; i < _objects.size(); i++) {
		_objects[i]->detach();
		//_objects[i].~intrusive_ptr();
	}
	_objects.clear();
}