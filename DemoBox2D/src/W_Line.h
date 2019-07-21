#pragma once
#include "oxygine-framework.h"
#include "World.h"
#include "Text.h"
using namespace oxygine;
DECLARE_SMART(W_Line, spW_Line);
class W_Line : public ColorRectSprite {
private:
	//This is what is displayed over the line.
	spText print;
public:
	//This will make a line between the two max camera points of the world.
	//Used as a marker.
	W_Line(World *world_ptr, const Vector2 &ref_pos);
	//This will clean up the things inside of this class.
	void destroy();
};