#pragma once
#include "oxygine-framework.h"
#include "GUI_Decor_M.h"
#include "random2.h"
using namespace oxygine;
enum type {
	same = -1,
	rain = 0,
	snow = 1,
	ash = 2
};
DECLARE_SMART(Raindrop, spRaindrop);

class Raindrop : public Sprite {
private:
	
	
	//This is a pointer to the randomStorages in the Rain class.
	random2::RandomStorage_int *_rand_store_pos;
	random2::RandomStorage_float *_rand_store_vel;
	
	//This determines if it has put itself in the _to_delete vector yet.
	bool has_inserted = false;
	

	std::vector<spRaindrop> *_to_delete;
	//This will set the given parameters to the raindrop.
	inline void set(const Vector2 &position, const Vector2 &velocity);
	//This will give the raindrop and a new velocity and a new position.
	void reset();
	//This is the velocity of the raindrop.(added to the position every frame).
	Vector2 vel;
	//This is called every frame.
	void doUpdate(const UpdateState &us);
public:
	bool _is_active = false;
	Raindrop(type t, random2::RandomStorage_float *sf_ptr, random2::RandomStorage_int *si_ptr, std::vector<spRaindrop> *to_delete_ptr);
};

DECLARE_SMART(Rain, spRain);
class Rain : public Actor {
public:
	

	//This activates the rain object.
	void set_activity(bool is_active, type t = same);
	//This sets the position of the sprite.
	//Use this instead of setPosition();
	void setPos(const Vector2 &new_pos);
	

private:
	
	//This is the type of rain droplet you should use. snow, rain, etc
	type _type;
	//This is a vector full of objects that need to be deleted.
	static std::vector<spRaindrop> Rain::_to_delete;
	//This tells if the rain is active or not.
	bool _is_active = 0;
	//This is how many are left to create after activating.
	unsigned int init_how_many_left_to_create = 0;
	//This is the last position of the camera.
	Vector2 last_pos;
	//This is how many drops of water there should be in total.
	unsigned int how_many_left_to_create = 0;
	//This stores pointers to all the raindrops.
	static std::vector<spRaindrop> Rain::_objects;
	//These are used for determining where the raindrops are suppost to spawn.
	random2::RandomStorage_int _rand_store_pos;
	//These are used for determining the raindrops velocities.
	random2::RandomStorage_float _rand_store_vel;
	//This will create a raindrop at the given level.
	void createDrop(unsigned int fromTop);
	//This keeps track of the time since it has last dropped a raindrop.
	float d_ticks = 0;
	//This is how long it needs to wati before dropping a raindrop.
	float max_d_ticks = 2;
	//This is called every frame.
	void doUpdate(const UpdateState &us);
public:
	//By default, it is a normal raindrop.
	Rain(const Vector2 &position, void* world_ptr, bool should_start_active, type t = same);
	
};