#pragma once
#include "Ship_Part_M.h"
#include "oxygine-framework.h"
using namespace oxygine;
class Ship_R_Leg : public Ship_Part_M {
public:
	Ship_R_Leg();
	Ship_R_Leg(Vector2 pos, int Playerindex, std::vector<b2Joint *> &joints);
	
	void onRemovedFromStage();
};
typedef intrusive_ptr<Ship_R_Leg> spShip_R_Leg;