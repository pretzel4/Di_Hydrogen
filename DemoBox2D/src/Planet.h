#pragma once
#include "oxygine-framework.h"
using namespace oxygine;
DECLARE_SMART(Tile, spTile);
class Tile : public Sprite {
private:
	const float h_speed;
	const int x_limit;
	const Vector2 init_pos;
	void revert(Event *ev);
	void doUpdate(const UpdateState &us);
public:
	Tile(const Vector2 &position, unsigned int type);
};
DECLARE_SMART(Planet, spPlanet);
class Planet : public Actor {
private:
	
	//This is a vector full of terrain tiles.
	std::vector<spTile> _terrain_tiles;
	//This is the water on the planet.
	spSprite _water;
	//Everything attached to this is masked.
	spMaskedSprite msk_spr;
	
	//This is the type of planet that it is.
	const unsigned int _type;

public:
	Planet(const Vector2 &position, unsigned int type);
};