#pragma once
#include "oxygine-framework.h"
#include "GUI_Decor_M.h"
using namespace oxygine;
DECLARE_SMART(Hot_Balloon, spHot_Balloon);
class Hot_Balloon : public GUI_Decor_M {
private:
	
	//This keeps track of all the Hot_Balloon objects.
	static std::vector<spHot_Balloon> Hot_Balloon::_balloons;
	//This is the coordinate at which the balloon gets destroyed.
	float x_death;
	//This is how fast the balloon should be moving to the left.
	float x_speed;
	
	//This is called every frame.
	void doUpdate(const UpdateState &us);
	void onRemovedFromStage();
	
public:
	Hot_Balloon(const Vector2 &position, float death_line, unsigned int type);
	
	//This will clear all the hot_air_balloons.
	static void clear_all();
};