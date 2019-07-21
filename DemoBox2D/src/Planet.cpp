#include "Planet.h"
#include "ImageRes.h"
#include "GF.h"
#define compression 4
#define extra_comp 2
void Tile::doUpdate(const UpdateState &us) {
	setX(getX() + (h_speed * us.dt));
	if (getX() >= x_limit) {
		float dif = x_limit - getX();
		setX(-96 + compression + extra_comp + dif);
	}
}

void Tile::revert(Event *ev) {
	setPosition(init_pos);
	//spTween t = addTween(Actor::TweenPosition(96 * 2, 0), 6000, 1, 0, 0);
	//t->addDoneCallback(CLOSURE(this, &Tile::revert));
}

Tile::Tile(const Vector2 &position, unsigned int type) : init_pos(position), x_limit((96 * 2) - (compression + extra_comp)), h_speed(0.01) {
	setPosition(position);
	/*switch (type) {
	case 0: {

		spTween t = addTween(Actor::TweenPosition(96 * 2, 0), 6000, 1, 0, 0);
		t->addDoneCallback(CLOSURE(this, &Tile::revert));
		break;
		}
		
		
	case 1:
	{
		spTween t = addTween(Actor::TweenPosition(96 * 2, 0), 6000 * 2, 1, 0, 0);
		t->addDoneCallback(CLOSURE(this, &Tile::revert));
		break;
	}
		
	case 2:
	{
		spTween t = addTween(Actor::TweenPosition(96 * 2, 0), 6000 * 3, 1, 0, 0);
		t->addDoneCallback(CLOSURE(this, &Tile::revert));
	}
		

	}*/

	
}

Planet::Planet(const Vector2 &position, unsigned int type) : _type(type) {
	setPosition(position);
	_water = new Sprite;
	_water->setResAnim(res::resources.getResAnim("Planet0"));
	addChild(_water);
	msk_spr = new MaskedSprite;
	msk_spr->attachTo(this);
	msk_spr->setMask(_water, false);
	//for (unsigned int i = 0; i < 3; i++) {

	//_terrain->setPosition(Vector2(-96 + (96 * i), 0));
	//_terrain->setMask(_water, false);

	//_terrain->addTween(Actor::TweenPosition(96 * 2, 0), 6000, 1, 0, 0);
	
	//}
	
	spTile _terrain = new Tile(Vector2(-96 + compression, 0), 0);
	_terrain->setAnimFrame(res::resources.getResAnim("Planet0_Features"), 0);
	msk_spr->addChild(_terrain);
	
	_terrain_tiles.push_back(_terrain);
	spTile _terrain2 = new Tile(Vector2(0, 0), 1);
	_terrain2->setAnimFrame(res::resources.getResAnim("Planet0_Features"), 1);
	addChild(_terrain2);
	msk_spr->addChild(_terrain2);
	_terrain_tiles.push_back(_terrain2);
	spTile _terrain3 = new Tile(Vector2(96 - compression, 0), 2);
	_terrain3->setAnimFrame(res::resources.getResAnim("Planet0_Features"), 2);
	addChild(_terrain3);
	msk_spr->addChild(_terrain3);
	_terrain_tiles.push_back(_terrain3);
	
	
}
