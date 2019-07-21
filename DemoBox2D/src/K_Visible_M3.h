#pragma once
#include "Kinematic_M2.h"
class K_Visible_M3 : public Kinematic_M2 {
public:
	//This sets up the sprite to be drawn.
	void setup(Actor *sprite, RectF &size, const spActor &attachment_obj = getStage()) const;
};