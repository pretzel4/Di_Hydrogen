#pragma once
#include "Box2D\Box2D.h"
#include "oxygine-framework.h"
#include "S_Visible_M3.h"
using namespace oxygine;
DECLARE_SMART(Bird_Nest, spBird_Nest);
class Bird_Nest :  public Sprite {
private:
	//This is a pointer to the body.
	b2Body *body;
	//This is a pointer to the world object.
	void* _world_ptr;
	void onRemovedFromStage();
	
public:
	//This is what is on the final platform.
	Bird_Nest(const Vector2 &position, void* world_ptr);
};