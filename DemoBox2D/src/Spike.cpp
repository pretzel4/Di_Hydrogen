#include "Spike.h"
#include "Ship_M.h"
#define grid_size 16
Spike::Spike(const Vector2 &rel_pos, Vector2 pos, unsigned int direction, int number_of_blocks, bool flip) {
	
	

	
	//Flipping the sprite and setting the resAnim accordingly.
	switch (direction) {
	case 1:
		if (flip) {
			setFlippedY(true);
		}
		setResAnim(res::resources.getResAnim("Spike"));
		break;
	case 2:
		if (flip) {
			setFlippedX(true);
		}
		setResAnim(res::resources.getResAnim("Spike_R"));
	}
	
	//Adding relative position.
	pos *= grid_size;
	number_of_blocks *= grid_size;
	//pos.x += rel_pos.x;
	//pos.y += rel_pos.y;
	
	setPosition(pos);
	//Naming it.
	setName("Spikes");

	//Setting guides
	setVerticalMode(Box9Sprite::StretchMode::TILING_FULL);
	setHorizontalMode(Box9Sprite::StretchMode::TILING_FULL);
	setGuides(0, 0, 0, 0);

	//Checking direction and making the rectangle.
	RectF r;
	switch (direction) {
	
	case 1://Direction is right
		r = RectF(rel_pos + pos, Vector2(number_of_blocks, grid_size));
		break;
	case 2://Direction is down
		r = RectF(rel_pos + pos, Vector2(grid_size, number_of_blocks));
		break;

	}
	

	//Setting up the sprite.
	setup(this, RectF(rel_pos + pos, Vector2(16, 16)));

	//Modifying the rectangle so that the position is just right.
	setSize(r.size.x, r.size.y);
	r.pos += (r.getSize() / 2);

	b2FixtureDef fd;
	fd.filter.categoryBits = SPIKES; //Is a platform.
	fd.filter.maskBits = DEFAULT_MASK; //Collides with everything.
	//Setting up the b2body.
	setup_body(r, NULL, NULL, &fd);
	setPriority(10000);

	body->SetUserData(this);
}

Vector2 Spike::convert_to_grid(const Vector2 &conversion_vector) {
	return conversion_vector / grid_size;
}

Vector2 Spike::convert_to_pixel(const Vector2 &conversion_vector) {
	return conversion_vector * grid_size;
}

void Spike::onRemovedFromStage() {
	world_ptr->getb2World()->DestroyBody(body);
}

void Spike::doneFading(Event *ev) {
	//Checking to make sure that it is still attached to something.
	if (getParent()) {
		detach();
	}
}

void Spike::if_contact(Ship_Part_M *ship_part_ptr, Actor* other_actor) {
	//You will instantly crash when you collide with spikes.
	Ship_M::objects[ship_part_ptr->playerIndex]->if_crashed(other_actor);
}

void Spike::fadeOut_and_destroy(unsigned int time) {
	addTween(Actor::TweenAlpha(0), time)->setDoneCallback(CLOSURE(this, &Spike::doneFading));
}

void Spike::fadeOut(unsigned int time) {
	GF::fadeOut(this, time, 0);
}