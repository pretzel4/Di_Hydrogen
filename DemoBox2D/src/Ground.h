#pragma once
#include "oxygine-framework.h"
#include "Box2D\Box2D.h"
#include "random2.h"
using namespace oxygine;
DECLARE_SMART(Ground, spGround);
class Ground : public Box9Sprite {
private:
	//This points to the bottom_camera boundaries.
	float &b_c_b;
	//This is the type of the ground.
	unsigned int _type;
	//This is the position that the ground needs to get to.
	b2Vec2 new_pos;
	//This is the position at which the ground started.
	Vector2 start_pos;
	

	//This should point to the world object; otherwise, behavior is undefined.
	void* _world_ptr;
	//This is a vector that holds the grass and flower objects.
	std::vector<spActor> props;
	//This will create the props across the ground.
	void create_props();
	
	
public:
	//This is the box2d body of the object.
	b2Body *body;
	Ground(const Vector2 &pos, void* world_ptr, unsigned int type = 0);
	
	//This will clean everything up.
	void destroy();
};
