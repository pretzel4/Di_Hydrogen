#pragma once
#include "oxygine-framework.h"
#include "Box2D\Box2D.h"
using namespace oxygine;
DECLARE_SMART(T_Sign, spT_Sign);
class T_Sign : public Sprite {
private:

public:
	T_Sign(const Vector2 &position);

};
DECLARE_SMART(Jaw, spJaw);
class Jaw : public Sprite {
private:
	b2Body* body;

	
public:
	//This function will make the jaw close.
	void close();
	Jaw(const Vector2 &position, void* world_ptr);
	//This will get the jaw's body.
	b2Body* getBody();
	//This properly cleans up the object.
	void destroy(void* world_ptr);
};
DECLARE_SMART(Eye_Ball, spEye_Ball);
class Eye_Ball : public Sprite {
private:
public:
	Eye_Ball(const Vector2 &position);

};

DECLARE_SMART(Main_Body, spMain_Body);
class Main_Body : public Sprite {
private:
	
	//These are ticks that are used for determing the next time the jaw should close.
	float ticks = 0;
	//This is the max number of ticks needed to close the jaw.
	float max_ticks = 0;
	//This is a pointer to the joint at which the jaw is attached.
	b2RevoluteJoint* rev_jnt;
	//This function will make the bird's jaw close.
	void close_jaw();
	//This is the eyeball on the bird.
	spEye_Ball eb;
	//This is the jaw of the bird.
	spJaw jaw;
	//This is the body of the bird.
	b2Body *body;
	//This is called every frame.
	void doUpdate(const UpdateState &us);
public:
	Main_Body(const Vector2 &position, void* world_ptr);
	//This properly cleans up the object.
	void destroy(void* world_ptr);
};

DECLARE_SMART(Final_Bird, spFinal_Bird);
class Final_Bird : public Actor {
private:
	//This is the sign that thanks the player for returning the egg.
	spT_Sign ts;
	//This should point to the world object.
	void* _world_ptr;
	//This is the main body.
	spMain_Body mb;
	//This is called when the bird is detached from the world.
	void onRemovedFromStage();
	

public:
	Final_Bird(const Vector2 &position, void* world_ptr);
};