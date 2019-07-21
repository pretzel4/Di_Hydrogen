#pragma once
#include "oxygine-framework.h"
#include "GUI_Decor_M.h"
using namespace oxygine;
DECLARE_SMART(Bunny, spBunny);
class Bunny : public GUI_Decor_M {
private:
	void onRemovedFromStage();
	void doUpdate(const UpdateState &us);
public:
	const Vector2 start_pos;
	Bunny(const Vector2 &position);
	
};