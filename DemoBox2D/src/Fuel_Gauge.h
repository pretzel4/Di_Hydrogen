#pragma once
#include "oxygine-framework.h"
#include "ImageRes.h"
#include "World.h"
using namespace oxygine;
class fra{ //Fuel rising animation
private:
	//This is how long it has been since starting the animation.
	float _ticks = 0;
	//This tells you how long it needs to be before reaching the desired fuel level.
	const float _max_ticks = 0;
	//This is how much fuel needs to be added.
	float _fuel_to_add_total = 0;
	//This is what was previously(last frame) returned from the tick function.
	float prev_return = 0;
public:
	//This is used for controlling the refuelling animation.
	fra(float max_ticks) : _max_ticks(max_ticks) {};
	
	void start(float current_fuel, float new_fuel);
	//This will update the animation every time you call this function.
	//This will return the fuel that should be added every frame.
	float tick(const UpdateState &us);

};
DECLARE_SMART(L_fuel, spL_fuel);
class L_fuel : public Sprite {
private:
public:
	L_fuel(const Vector2 &position);
};
class back_plate : public Sprite {
public:
	back_plate(const Vector2 &pos) {
		setPosition(pos);
		setAnchor(0.5f, 0.5f);
		setResAnim(res::resources.getResAnim("Ship_Empty"));
	}
};
typedef intrusive_ptr<back_plate> spback_plate;
class Fuel_Gauge : public Actor {
private:
	//This is used for fuel animation.
	fra _fra;
	//This is the percent of fuel that the gauge has.
	float _f_per;
	//This is the initial height of the sprites.
	const float init_height;
	//This is the initial position at which you created the object.
	const Vector2 init_pos;
	//This is the current global position of the Torso object.
	Vector2 global_pos;
	inline void update_fuel(float new_fuel);
	//This is the max amount of fuel the gauge can display.
	//This is used for percentages.
	float _max_fuel;
	//This is the part of the fuel gauge that is empty.
	spMaskedSprite mask_rect;
	//spColorRectSprite debug_rect;
	spClipRectActor clip_rect;
	spSprite alpha_rect;
	spL_fuel fuel_sprite;
	spback_plate bp;
	//This is the fuel that was last given to setFuel.
	//This is not entirely reliable to give the actual fuel in the tank.
	//Use at your own risk.
	float _last_fuel = 0;
public:
	//This will update the animation.
	//This will return the fuel adding constant.
	float update_animation(const UpdateState &us);
	//This will update the rotation of the fuel gauge.
	void update_rotation(Sprite* sprite);
	//void update();
	Fuel_Gauge(const Vector2 &global_pos, const Vector2 &pos, float max_fuel, spWorld world_ptr);
	//This will update the progress bar.
	void setFuel(float new_fuel, float rotation);
	void destroy();
	//This will set the fuel to the desired amount; however, it will do so in an animation. i.e: slowly fill up.
	void refill(float new_fuel);
	
};
typedef intrusive_ptr<Fuel_Gauge> spFuel_Gauge;