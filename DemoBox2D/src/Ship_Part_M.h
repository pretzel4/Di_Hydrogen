#pragma once
#include "World.h"
#include "Box2D\Box2D.h"
#include "oxygine-framework.h"
#include "Tile_M.h"
#include "Filters.h"
#include "P_System.h"
#include "Noise.h"
using namespace oxygine;
/*This defines a ship part.*/
enum e_types : int{
	e_box = 0, e_l_leg = 1, e_r_leg = 2, e_engine = 3, e_torso = 4
};
class Ship_Part_M : public Sprite, public Filters {
private:
	
protected:
	//This is used for identifying parts from one another.
	std::string identifier;
	//This is a sound that plays when you collide into something.
	Noise::instance c_sound;
	//This is the contact by which the particles are emitted.
	//This will be NULL if there is no contact. Make sure you don't call a null pointer.
	b2Contact *contact_ptr;
	
	//This is the particle emitting system that enables sparks to come out of the contacts.
	spP_System s_emitter;
	//This is a vector used for storing the joint data from the ship_M class.
	std::vector<b2Joint *> *ship_joints;
	//This function will loop through the given vector of joints and delete any joints that were attached to this object.
	void delete_joints(std::vector<b2Joint *> &joints);
	bool has_crashed = false;
	//This is a pointer to the world, used primarily for scrolling in the y axis.
	static spWorld Ship_Part_M::_world_ptr;
	//This sets up the part.
	void setup(const char* sprite, const Vector2 &pos, filters categoryBits = DEFAULT_CAT, filters maskBits = DEFAULT_MASK, e_types type = e_types::e_box, float scale = 1);
	bool _has_been_deleted = false;
	//This will update the s_emitter's position, amplitude, etc.
	void update_emitter();
	void doUpdate(const UpdateState &us);

	
	//void destroy(){};
public:
	//This points to whatever you want.
	void* abstract_ptr;
	
	//This will determine what index each part is at.
	int playerIndex;
	Ship_Part_M();
	virtual ~Ship_Part_M();
	void if_contact(b2Contact* contact, Ship_Part_M *s_part, Actor* other_actor);
	void end_contact(Ship_Part_M *s_part);
	//This erases the body&sprite from the world.
	//It will still be a valid pointer, so make sure you make it NULL.
	void erase();
	//This is the body.
	b2Body *body;
	static void init(World *world);
};
typedef intrusive_ptr<Ship_Part_M> spShip_Part_M;