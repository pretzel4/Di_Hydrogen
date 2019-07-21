#pragma once
#include "oxygine-framework.h"
#include "GUI_Decor_M.h"
using namespace oxygine;
class Spaceship : public GUI_Decor_M, public Sprite {
private:

public:
	Spaceship(const Vector2 &position, unsigned int type);
};