#pragma once
//#include "Ship_M.h"
#include "oxygine-framework.h"
using namespace oxygine;
//This is the class that is used for controlling the ship by touch.
//Invisible_button
DECLARE_SMART(I_control, spI_control);
class I_control : public Actor {
private:
	
public:
	enum types {
		up = 0, right = 1, down = 2, left = 3
	};
	I_control(types type, void* player);
};
