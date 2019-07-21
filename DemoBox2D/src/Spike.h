#pragma once
#include "oxygine-framework.h"
#include "S_Visible_M3.h"
#include "Ship_Part_M.h"

/*This class is a little messy, sorry if you need to modify this.
This a block that whenver the ship touches it, the ship will crash.*/
using namespace oxygine;

DECLARE_SMART(Spike, spSpike);
class Spike : public S_Visible_M3, public Box9Sprite {
private:
	void doneFading(Event *ev);
public:

	Spike(const Vector2 &rel_pos, Vector2 pos, unsigned int direction, int number_of_blocks, bool flip);
		
	void onRemovedFromStage();
	//This will convert the position given into blocks. Call this before giving it to the constructor if you are giving it in pixels.
	static Vector2 convert_to_grid(const Vector2 &conversion_vector);
	//This will convert the position given into pixels.
	static Vector2 convert_to_pixel(const Vector2 &conversion_vector);
	void if_contact(Ship_Part_M *ship_part_ptr, Actor* other_actor);
	void fadeOut_and_destroy(unsigned int time);
	void fadeOut(unsigned int time);
};
