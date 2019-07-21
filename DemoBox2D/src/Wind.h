#pragma once
#include "oxygine-framework.h"
#include "Box2D\Box2D.h"
using namespace oxygine;
/*This is used to simulate wind.*/
class Wind {
private:
	//This is added to the bodies every frame.
	b2Vec2 _current;
	//This tells if the wind is active or not.
	bool _is_active;

public:
	//This will set the current of the wind.
	void setCurrent(const b2Vec2 &current);
	//This will set it active or non_active.
	void set_activity(bool is_active);
	//This will set the current and activity.
	void set(const b2Vec2 &current, bool is_active);
	Wind();
	//This is used for simulating wind.
	Wind(const b2Vec2 &init_current, bool is_active = false);
	//This takes in a body and will change it according to the wind.
	void simulate(b2Body *to_change);
	
	//This will return true if the wind is active; otherwise, it will return false.
	bool get_is_active();
	//This will return the current.
	b2Vec2 get_current();
};