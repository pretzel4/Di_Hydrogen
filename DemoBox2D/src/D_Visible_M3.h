#pragma once
#include "Dynamic_M2.h"
class D_Visible_M3 : public Dynamic_M2 {
public:
	//This sets up the sprite to be drawn.
	void setup(Actor *sprite, RectF &size, const spActor &attachment_obj = getStage()) const;
};