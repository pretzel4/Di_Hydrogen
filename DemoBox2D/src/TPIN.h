#pragma once
#include "Box2D\Box2D.h"
#include "oxygine-framework.h"
#include "World.h"
#include "Ship_Part_M.h"
#include "Ship_M.h"
#include "TPI_Line.h"
using namespace oxygine;
DECLARE_SMART(TPIN, spTPIN);
/*This class is used for telling the Ship_M class that the object inheriting this one, will be pointed at by TPI_Lines.*/
class TPIN : public Actor {
private:
	//This is the type of the TPIN.
	unsigned int _type;
	//This is the radius of the circle.
	const float _radius;
	//This is a pointer to the world.
	spWorld _world_ptr;
	//This is the position at which the TPIN will reside.
	Vector2 _pos;
	//This is a IC_Line pointer. Used for destroying itself after leaving the sensor.
	spTPI_Line _i_ptr;
	//This tells you how many ship parts are within the circle sensor.
	unsigned int num_of_parts_within;
	//This is used for sensing if the ship has come within a certain radius of the TPIN object.
	b2Body *body;
	
	void onRemovedFromStage();
public:
	//This will set the position of the tpin.
	void SetPosit(const Vector2 &new_pos);
	//This will return the type of tpin that the object is.
	unsigned int getType();
	enum type {
		PLATFORM = 0,
		ENTITY = 1
	};
	void when_entering(Ship_Part_M *part_ptr);
	void when_leaving(Ship_Part_M *part_ptr);
	TPIN(const Vector2 &position, float radius, World* world_ptr, type Type);
	//This will make the TPIN line fade out then detach and destroy itself.
	void fadeOut(int fadeTime);
};