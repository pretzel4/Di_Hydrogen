#pragma once
#include "oxygine-framework.h"
#include "GUI_Decor_M.h"
using namespace oxygine;
DECLARE_SMART(C_Milk, spC_Milk);
class C_Milk : public GUI_Decor_M {
private:
	
public:
	const Vector2 start_pos;
	C_Milk(const Vector2 &position);

};