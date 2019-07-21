#pragma once
#include "Box2D\Box2D.h"
#include "oxygine-framework.h"
using namespace oxygine;
class Camera : public Actor {
private:
	//This is the offset from the origin of the world.
	b2Vec2 _offset;
	
public:
	
	//This will set the given object to a child of the camera.
	void add(spActor &actor);
	Camera();
	//This will update the camera to the given offset.
	void update();
};