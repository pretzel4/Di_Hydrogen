#pragma once
#include <vector>
#include "Box2D\Box2D.h"
#include "World.h"
#include "oxygine-framework.h"
#include "Filters.h"
using namespace oxygine;
/*This is a class that contains all the tiles in the world.
It holds pointers to all the objects that should be saved.*/
DECLARE_SMART(Tile_M, spTile_M);

class Tile_M : public Filters {
private:
	//static std::vector<spTile_M> Tile_M::_Cache;
	//typedef intrusive_ptr<Tile_M> spTile_M;
	//This contains pointers to all the objects created from this class.
	
	
	

protected:
	
	//This is the scale of the object.
	const float SCALE = 100.0f;
	//This contains a pointer to the world.
	static spWorld Tile_M::world_ptr;
	//This is the body that is placed in the world Box2D world.
	b2Body *body;
	Tile_M();
	
	

	

public:
	//This is used to determine if the body is ready to be deleted or not.
	bool is_ready_to_delete = false;
	
	//This is used to initialize the class.
	static void init(World *World_Ptr);
};

