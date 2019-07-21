#include "ForceField.h"
#include "ImageRes.h"
#define max_force_color 80 / 5
ForceField::ForceField(RectF box, int direction, float force, unsigned int fadeTime, bool should_add_to_world) {
	//RECTANGLE SPRITE
	force *= 5;
	unsigned int alpha = 255 * (force / max_force_color);
	red_plate = new ColorRectSprite();
	//ACTOR STUFF
	setPosition(box.pos);
	setup(this, box);
	red_plate->setColor(Color(255, 0, 0, alpha));
	//red_plate->setAlpha(100);
	red_plate->setPosition(Vector2(0,0));
	red_plate->setSize(box.size);
	red_plate->setPriority(10000);
	addChild(red_plate);
	setName("ForceField");
	b2FixtureDef fd;
	fd.filter.categoryBits = FORCE_FIELD; //It is a black_hole.
	fd.filter.maskBits = DEFAULT_MASK; //Collide with everything.
	b2Fixture *f_ptr = setup_body(box, NULL, NULL, &fd);
	f_ptr->SetSensor(true);
	
	
	setPriority(-5);
	//CLIP STUFF
	clip = new ClipRectActor();
	addChild(clip);
	clip->setPosition(Vector2(0, 0));//Vector2(box.pos - (box.size / 2)));
	clip->setSize(box.size);
	clip->setPriority(-4);
	//ARROW SPRITE
	arrow = new Sprite();
	//Setting up the arrow.
	//arrow->setMask(this, true);
	//arrow->setMask(c_rect, true);
	arrow->setAnchor(0.5f, 0.5f);
	arrow->setAnimFrame(res::resources.getResAnim("ForceField_Arrow"), direction);
	arrow->setSize(box.size);
	clip->addChild(arrow);

	Vector2 ref_pos = Vector2(0,0);//getPosition() - getSize();
	float h_h = getHeight() / 2;
	float h_w = getWidth() / 2;
	//Determining what vector should be applied to all objects within the field.
	switch (direction) {
	case 0:
		arrow->setPosition(ref_pos + Vector2(getWidth() / 2, getHeight() + h_h));
		arrow->addTween(Actor::TweenPosition(ref_pos + Vector2(getWidth() / 2, -h_h)), 2000, -1);
		pre_vector = b2Vec2(0, -force);
		break;
	case 1:
		arrow->setPosition(ref_pos + Vector2(-h_w, getHeight() / 2));
		arrow->addTween(Actor::TweenPosition(ref_pos + Vector2(getWidth() + h_w, getHeight() / 2)), 2000, -1);
		pre_vector = b2Vec2(force, 0);
		break;
	case 2:
		arrow->setPosition(ref_pos + Vector2(getWidth() / 2, -h_h));
		arrow->addTween(Actor::TweenPosition(ref_pos + Vector2(getWidth() / 2, box.getHeight() + h_h)), 2000, -1);
		pre_vector = b2Vec2(0, force);
		break;
	case 3:
		arrow->setPosition(ref_pos + Vector2(getWidth() + h_w, getHeight() / 2));
		arrow->addTween(Actor::TweenPosition(ref_pos + Vector2(-h_w, getHeight() / 2)), 2000, -1);
		pre_vector = b2Vec2(-force, 0);
	}
	GF::fadeIn(this, fadeTime, alpha);
	body->SetUserData(this);
	if (should_add_to_world) {
		world_ptr->addChild(this);
	}
}

void ForceField::destroy() {
	
	world_ptr->getb2World()->DestroyBody(body);
	
	
}

void ForceField::if_entered(b2Body *entering_body, Actor *entering_actor) {
	if (entering_actor->getName() != "Pinwheel" && entering_actor->getName() != "ForceField" && entering_actor->getName() != "T_Block" && entering_actor->getName() != "T_Block_K") {
		within.push_back(entering_body);
	}
	
}

void ForceField::if_left(b2Body *leaving_body) {
	//Checking to see where the other pointer is that is identical to the one that just left, so that it can delete it out of the within vector.
	for (unsigned int i = 0; i < within.size(); i++) {
		if (within[i] == leaving_body) {
			within.erase(within.begin() + i);
			break;
		}
	}
}

void ForceField::doUpdate(const UpdateState &us) {
	if (!world_ptr->get_is_paused()) {
		for (unsigned int i = 0; i < within.size(); i++) {
			if (within[i] != NULL) {
				within[i]->ApplyForceToCenter(pre_vector, true);
			}

		}
	}
	
	
}

void ForceField::onRemovedFromStage() {
	if (!has_deleted) {
		destroy();
		has_deleted = true;
	}
	
		
}

void ForceField::doneFading(Event *ev) {
	if (getParent()) {
		detach();
	}
}

void ForceField::fadeOut(unsigned int time) {
	GF::fadeOut(this, time, 0);
}

void ForceField::fadeOut_and_destroy(unsigned int time) {
	addTween(Actor::TweenAlpha(0), time)->setDoneCallback(CLOSURE(this, &ForceField::doneFading));
}