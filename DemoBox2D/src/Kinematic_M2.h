#pragma once
#include "Tile_M.h"
class Kinematic_M2 : public Tile_M {

protected:
	//Default constructor
	Kinematic_M2();
	
	b2Fixture* setup_body(RectF &rect, b2BodyDef *overwrite_body = NULL, b2Shape *overwrite_shape = NULL, b2FixtureDef *fixture_def = NULL);
	void setup_body_adv(std::vector<Vector2> &vertices);
public:
	
};