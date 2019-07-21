#pragma once
#include "S_Invisible_M3.h"
#include "K_Visible_M3.h"
#include "K_Invisible_M3.h"
#include "oxygine-framework.h"
#include "D_Visible_M3.h"
#include "S_Visible_M3.h"
#include "Entity.h"
#include "ImageRes.h"
using namespace oxygine;
#define pin_turn_speed 500
#define circle_extention 100
//This is the circle in the background indicating where the ball is going to be throughout its circle.
DECLARE_SMART(Circle, spCircle);
class Circle : public Sprite {
private:

public:
	Circle(const Vector2 &position, float radius);
};
DECLARE_SMART(Ball, spBall);
class Ball : public D_Visible_M3, public Sprite {
public:
	Ball(const Vector2 &axis_point, float rot_speed);
	void destroy() {
		detach();
		world_ptr->getb2World()->DestroyBody(body);
	}
	b2Body * getBody() {
		return body;
	}
};
DECLARE_SMART(Stick, spStick);
class Stick : public K_Visible_M3, public Box9Sprite {
private:
	b2RevoluteJoint *rj;
	b2RevoluteJoint *rj2;
	spBall pin;
	spBall pin2;
	spCircle circ;
	void doUpdate(const UpdateState &us);
public:
	void create_joint(); 
	Stick(const Vector2 &axis_point, float dist_in_16s, float rot_speed, unsigned int fadeTime = 0);
		
	spWorld getWorld() {
		return world_ptr;
	}

	void done_fading_out(Event *ev) {
		destroy();
	}

	void destroy() {
		
		
		//circ.~intrusive_ptr();
		pin->destroy();
		pin2->destroy();
		detach();
		
		world_ptr->getb2World()->DestroyBody(body);
		circ->detach();
		//pin.~intrusive_ptr();
		//pin2.~intrusive_ptr();
	}
	b2Body* getBody() {
		return body;
	}
	void fadeOut(unsigned int time) {
		GF::fadeOut(circ.get(), time, 0);
		GF::fadeOut(pin.get(), time, 0);
		GF::fadeOut(pin2.get(), time, 0);
		GF::fadeOut(this, time, 0);
		
	}
};
DECLARE_SMART(Chainsaw, spChainsaw);
class Chainsaw : public Entity {
private:
	
	spStick pole;
	
public:
	//This determines if you have called destroy or not on this object.
	bool has_called_destroy = false;
	//This is a spinning blade on a stick, that simple.
	//The inflicted tween will call the tween on every object within this object.
	Chainsaw(const Vector2 &pos, float rot_speed, float length, unsigned int fadeTime = 0);
	
	//This will make the chainsaw fade out.
	void fadeOut(unsigned int time = 0);
	void destroy();
	//This is a virtual function that is overwritten from the actor class. It is called every time the actor is detached from the stage.
	void onRemovedFromStage();
	//void create_joints(float rot_speed);
};