#pragma once
#include "oxygine-framework.h"
#include "Ship_Part_M.h"
#include "Fuel_Gauge.h"
using namespace oxygine;
DECLARE_SMART(Compass, spCompass);
class Compass : public Sprite {
private:
	const Vector2 *ship_pos;
	
	bool should_make_new_tween = true;
public:
	Compass(const Vector2 &pos, const Vector2 *global_pos);
	void doUpdate(const UpdateState &us);
	void make_new_tween(Event *ev);
	void destroy();
};
DECLARE_SMART(Ship_Torso, spShip_Torso);
class Ship_Torso : public Ship_Part_M {
private:
	
	void doUpdate(const UpdateState &us);
	//spCompass cp;
public:
	spFuel_Gauge fg;
	//This is called when the object is remove from the stage/deleted.
	void onRemovedFromStage();
	Ship_Torso(const Vector2 &pos, int Playerindex, float max_fuel, std::vector<b2Joint *> &joints);
	void setFuel(float new_fuel);
	//This will update changes in the fuel.
	float update_animation(const UpdateState &us);
	//This will show a refill animation instead of just instantly being that fuel.
	void refill(float new_fuel);
};