#pragma once
#include "oxygine-framework.h"
#include "GF.h"
#include <functional>
using namespace oxygine;
#define Default_CD_Time 10000 //This is the default fade time; time it takes to fade into one color from another.

class Fade_Color {
private:
	//This tells if you have called the lambda setup function.
	bool did_call_setup2 = false;
	//This tells the object if there is a lambda that should be called.
	bool is_lambda = false;
	//This is a lambda that will be called when the color is done fading.
	std::function<void()> lambda;
	//This tells if the color is still fading.
	bool _is_fading = false; 
	//This holds the color that it was before fading.
	unsigned int old_cb_color[4]; 
	//This is how much of a difference the two colors have.
	int cb_change[4]; 
	//This is the new color that it is trying to fade into.
	unsigned int cb_color[4]; 
	//This is how long it will take to change everything.
	float how_long = Default_CD_Time; 
	//This is how long it has been currently active.
	float tick = 0; 

public:
	//If this is true, then it is still changing color; otherwise it will return false.
	bool is_active();
	//This sets up the fade_color. Use this when you are telling the object to change colors.
	void setup(const Vector4 &current_color, const Vector4 &new_color, float fade_time = Default_CD_Time);
	//This sets up the fade_color. Use this when you are telling the object to change colors.
	void setup(const Vector4 &current_color, const Vector4 &new_color, std::function<void()> callback, float fade_time = Default_CD_Time);
	//This is called every frame.
	Color update(const UpdateState &us);
	
	//Default constructor
	Fade_Color() {

	}
	


};