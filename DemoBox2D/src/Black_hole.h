#pragma once
#include "oxygine-framework.h"
#include "Entity.h"
#include "S_Visible_M3.h"
#include "S_Invisible_M3.h"
#include "TPIN.h"
#include "Tile_M.h"
using namespace oxygine;
DECLARE_SMART(Vertex_Sprite, spVertex_Sprite);
class Vertex_Sprite : public ColorRectSprite {
public:
	Vertex_Sprite(const Vector2 &pos, float gravity);
};
DECLARE_SMART(Back_frame, spBack_frame);
class Back_frame : public Sprite {
private:
	float rot_speed = 0;
public:
	Back_frame(const Vector2 &pos, float gravity);
	void doUpdate(const UpdateState &us);
	void destroy();
};
DECLARE_SMART(Vertex, spVertex);
class Vertex : public S_Invisible_M3, public Actor {
private:
	void doUpdate(const UpdateState &us);
	spTPIN _tpin;
	bool _is_playing_sound = false;
	//This is a sound that the black hole constantly plays.
	Noise::instance w_sound;
	
public:
	Vertex(const Vector2 &pos, float gravity);
	//This is called for clean up.
	void destroy();
	//This called when something has collided with it.
	void if_contact(b2Body *other_body, Actor* other_actor);
	//This gets the vertex's body.
	b2Body *getBody();
	//This will play the sound.
	void play_sound();
	//This will stop the sound.
	void stop_sound();
	//This will return true if the sound is playing; otherwise, it will return false.
	bool is_sound_playing();
	//This will fade out the tpin in the vertex.
	void fadeOut_tpin(int fadeTime);

};

DECLARE_SMART(Field, spField);
class Field : public S_Invisible_M3, public Actor {
private:
	spVertex_Sprite vtx_sprite;

	spBack_frame bf;
	//This welds the vertex and the field together.
	b2Joint *weld_jt;
	//These are all the bodies contained inside the field.
	std::vector<b2Body *> contained;
	spVertex vtx;
	//This is the gravity of the black_hole; the amount of force applied to nearby objects.
	float _gravity = 0;
public:
	Field(const Vector2 &pos, float gravity);
	//This is called when the is an object is inside the sensor.
	void if_entered(b2Body *other_body, Actor* other_actor);
	//This is called when an object leaves the sensor.
	void if_exited(b2Body *other_body);
	//This is called every frame.
	void doUpdate(const UpdateState& us);
	//This is called whenever the field needs to clean up.
	void destroy();
	//This is called to fadeOut the field object.
	void fadeOut(unsigned int time);
};

DECLARE_SMART(Black_hole, spBlack_hole);
class Black_hole :  public Entity {
private:
	
	spField field;
	
public:
	Black_hole(const Vector2 &pos, float gravity, void* world_ptr, unsigned int fadeTime = 0);
	void destroy();
	//This is a overwritten virtual function that is called whenever the actor is detached from the stage.
	void onRemovedFromStage();
	//This is called when the black hole fades out.
	//This is a overwritten virtual function.
	void fadeOut(unsigned int time = 0);
};