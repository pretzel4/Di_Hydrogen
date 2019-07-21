#pragma once
#include "oxygine-framework.h"
#include "GUI_Decor_M.h"
#include "Fade_Color.h"
using namespace oxygine;
//Front
DECLARE_SMART(Cover, spCover);
class Cover : public Sprite {
private:
public:
	Cover(const Vector2 &position);
};


//Back
DECLARE_SMART(Volcano, spVolcano);
class Volcano : public GUI_Decor_M {
private:
	//This is the function that is used to fade in.
	std::function<void()> _fade_in_lamb;
	//This is the function that is used to fade out.
	std::function<void()> _fade_out_lamb;
	//This is used for the glowing effect.
	Fade_Color fc;
	//This is what goes over the lava of the volcano.
	spCover _cover;
	//This is called every frame.
	void doUpdate(const UpdateState &us);
public:
	Volcano(const Vector2 &position);
	
	
};