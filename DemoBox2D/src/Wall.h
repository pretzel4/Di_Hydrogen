#pragma once
#include "Box2D/Box2D.h"
#include "oxygine-framework.h"
#include "World.h"
#include "GF.h"
#include "S_Visible_M3.h"
#include "S_Invisible_M3.h"
#include "Col.h"

using namespace oxygine;
class Wall : public S_Visible_M3, public Sprite, public Col {

public:
	
	Wall(World* world, RectF &rect, unsigned int type);
	~Wall();

private:
	
	const float SCALE = 100.0f;
	

	void doUpdate(UpdateState &us);

private:


};
//typedef oxygine::intrusive_ptr<Wall> spWall;