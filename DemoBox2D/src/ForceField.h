#pragma once
#include "oxygine-framework.h"
#include "S_Visible_M3.h"
#include "Entity.h"
using namespace oxygine;
DECLARE_SMART(ForceField, spForceField);
class ForceField : public S_Visible_M3, public Entity  {

private:
	spColorRectSprite red_plate;
	//This determines if you have deleted the forcefield yet.
	bool has_deleted = false;
	//This is the rectangle that all the sprites need to be in.
	spClipRectActor clip;
	//This is the colored rectangle that represents the forcefield.
	spColorRectSprite c_rect;
	//This holds pointers to all the objects that are within the field.
	std::vector<b2Body *> within;
	//This is the amount of force it should apply to all objects inside of the field.
	float _force = 0;
	//This is the direction by which the field will push the objects.
	int _direction = -1;
	//This is a vector that should be applied to all objects within the field.
	//This is determined on first creation of the object.
	b2Vec2 pre_vector;
	//This is the arrow that will show what direction that field applies it's force in.
	spSprite arrow;
	void doneFading(Event *ev);

public:
	//This is called when something has entered the field.
	void if_entered(b2Body *entering_body, Actor *entering_actor);
	//This is called when something has left the field.
	void if_left(b2Body *leaving_body);
	//This is called every frame.
	void doUpdate(const UpdateState &us);
	//This makes a box that will push whatever is inside of it in the direction given, with the force given.
	ForceField(RectF box, int direction, float force, unsigned int fadeTime, bool should_add_to_world = true);
	//This will clean up everything within this class.
	void destroy();
	//This is called when the actor is detached from the stage.
	void onRemovedFromStage();
	//This is called when the forcefield should be fading out.
	void fadeOut(unsigned int time = 0);
	//This will make the forcefield fade out and detach itself after fading.
	void fadeOut_and_destroy(unsigned int time);
};