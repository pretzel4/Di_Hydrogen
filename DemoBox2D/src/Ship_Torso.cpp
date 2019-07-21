#include "Ship_Torso.h"
#include "ImageRes.h"
#include "Platform.h"
#include <cmath>
#include "Ship_M.h"
Compass::Compass(const Vector2 &pos, const Vector2 *global_pos) {
	setResAnim(res::resources.getResAnim("compass"));
	setPosition(pos);
	setAnchor(0.5f, 0.5f);
	ship_pos = global_pos;
	
}

void Compass::destroy() {
	detach();
	
}

void Compass::doUpdate(const UpdateState &us) {
	//if (should_make_new_tween == true) {
		
		Vector2 *new_pos = Platform::get_first_platform_pos();
		if (new_pos != NULL) {
			float angle = atan2(ship_pos->y - new_pos->y, ship_pos->x - new_pos->x);
			float ship_angle = getParent()->getRotation();
			setRotation((angle - 90) - ship_angle);
		}
		
		//spTween t = addTween(Actor::TweenRotation(), 100, 1, 0, 0);
		//t->addDoneCallback(CLOSURE(this, &Compass::make_new_tween));
		//should_make_new_tween = false;
	//}
		
	
}

void Compass::make_new_tween(Event *ev) {
	/*Vector2 &new_pos = Platform::get_first_platform_pos();
	float angle = atan2(new_pos.y - ship_pos->y, new_pos.x - ship_pos->x);
	spTween t = addTween(Actor::TweenRotation(angle), 100, 1, 0, 0);
	t->addDoneCallback(CLOSURE(this, &Compass::make_new_tween));*/
	should_make_new_tween = true;
}

void Ship_Torso::doUpdate(const UpdateState &us) {
	fg->update_rotation(this);
}

Ship_Torso::Ship_Torso(const Vector2 &pos, int Playerindex, float max_fuel, std::vector<b2Joint *> &joints) {
	ship_joints = &joints;
	playerIndex = Playerindex;
	setup("Ship_Torso", pos + (getSize() / 2), SHIP_PART, DEFAULT_MASK, e_torso);
	spFuel_Gauge f = new Fuel_Gauge(pos, getSize() / 2, max_fuel, _world_ptr);
	fg = f;
	addChild(fg);
	fg->attachTo(this);
	fg->setFuel(max_fuel, getRotation());
	
	
	
	_world_ptr->addChild(this);
	
	//cp->attachTo(this);
	
	//this->insertSiblingBefore(fg);
	
	
	
	//insertSiblingBefore(fg, this);
	//_world_ptr->addChild(fg);
	//fg->setPriority(500);
	//setPriority(-500);
	//fg->setFuel(100);
}

void Ship_Torso::onRemovedFromStage() {
	if (!_has_been_deleted) {
		Ship_M::objects[0]->line_controller.delete_all();
		fg->destroy();
		s_emitter->detach();
		c_sound.destroy();
		//s_emitter = NULL;
		delete_joints(*ship_joints);
		_world_ptr->getb2World()->DestroyBody(body);
		_has_been_deleted = true;
	}
	
}

void Ship_Torso::setFuel(float new_fuel) {
	//fg->setRotation(getRotation());
	fg->setFuel(new_fuel, getRotation());
	
}

void Ship_Torso::refill(float new_fuel) {
	fg->refill(new_fuel);
}

float Ship_Torso::update_animation(const UpdateState &us) {
	return fg->update_animation(us);
}