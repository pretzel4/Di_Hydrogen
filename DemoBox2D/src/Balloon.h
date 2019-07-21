#pragma once
#include "oxygine-framework.h"
using namespace oxygine;
#include "GUI_Decor_M.h"
DECLARE_SMART(Balloon, spBalloon);
class Balloon : public GUI_Decor_M {

private:
	static std::vector<spBalloon> Balloon::_balloons;
	//This tells if the balloon has achieved its desired height.
	bool has_achieved_height = false;
	//This is the type of balloon that it is.
	unsigned int _type;
	//This is what should be added to the balloon every frame.
	Vector2 _vel;
	//This is how it goes before popping.
	int _max_height;
	//This is called every frame.
	void doUpdate(const UpdateState &us);
	//This is the acceleration of a balloon.
	Vector2 accel = Vector2(0, -0.0004);
	
	//This is called after the balloon is done animating its popping animation.
	void done_popping(Event *ev);

	void onRemovedFromStage();
public:
	//This will make a decoration that looks like a balloon.
	//This does not use box2d, it also does not add its own parent.
	Balloon(const Vector2 &position, int max_height, unsigned int type);
	//This will clear all the balloons on the stage.
	static void clear_all();

};