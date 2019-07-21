#pragma once
#include "Ship_Part_M.h"
class Ship_L_Leg : public Ship_Part_M {
public:
	Ship_L_Leg();
	Ship_L_Leg(Vector2 pos, int Playerindex, std::vector<b2Joint *> &joints);
	
	void onRemovedFromStage();
};
typedef intrusive_ptr<Ship_L_Leg> spShip_L_Leg;