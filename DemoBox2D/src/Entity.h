#pragma once
#include "oxygine-framework.h"
#include "World.h"
#include "ox/Actor.hpp"
using namespace oxygine;
DECLARE_SMART(Entity, spEntity);
class Entity : public Actor {

public:
	
	int ID = 0; //Identification For Deconstruction
	virtual void fadeOut(unsigned int time = 0){};
	
};