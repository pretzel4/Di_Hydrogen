#pragma once
#include "oxygine-framework.h"
#include "GUI_Decor_M.h"
using namespace oxygine;

DECLARE_SMART(B_Flower, spB_Flower);
class B_Flower : public GUI_Decor_M {
private:
public:
	B_Flower(const Vector2 &position);
};