#pragma once
#include "Static_M2.h"
#include "oxygine-framework.h"

using namespace oxygine;

class S_Visible_M3 : public Static_M2 {
protected:
	//This sets up the sprite to be drawn.
	void setup(Actor *sprite, RectF &size, const spActor &attachment_obj = getStage()) const;
	/*Make a function here that sets up the sprite, according to the given parameters.*/
};
