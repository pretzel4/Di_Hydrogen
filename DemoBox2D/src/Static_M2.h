#pragma once
#include "Tile_M.h"
#include "Box2D\Box2D.h"

class Static_M2 : public Tile_M {
protected:
	
	//Default constructor
	Static_M2();
	
	//This sets up the body to the given parameters.
	b2Fixture* setup_body(RectF &rect, b2BodyDef *overwrite_body = NULL, b2Shape *overwrite_shape = NULL, b2FixtureDef *fixture_def = NULL);
	//This sets up the body to the given parameters, with vertices.
	void setup_body_adv(std::vector<Vector2> &vertices);
};
