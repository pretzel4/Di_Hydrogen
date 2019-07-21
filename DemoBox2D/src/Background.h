#pragma once
#include "oxygine-framework.h"
#include "Fade_Color.h"
#include "random2.h"
#include "Balloon.h"
#include "Rain.h"
#include "Bubble.h"
#include "Bunny.h"
#include "Volcano.h"
#include "C_Milk.h"
#include "D_Islands.h"
//#include "World.h"
#define Default_CD_Time 10000
using namespace oxygine;
/*This holds the objects for two volcanos*/
DECLARE_SMART(I_Hand, spI_Hand);
class I_Hand : public Sprite {
private:
	const bool _is_reversed = false;
public:
	//This should be called when the click animation is done.
	void click_done(Event *ev);
	I_Hand(const Vector2 &position, bool is_reversed = false);
};


DECLARE_SMART(Volcano_controller, spVolcano_controller);
class Volcano_controller : public Actor{
private:
	spVolcano v1;
	spVolcano v2;
	 
public:
	//This is the starting position of the object.
	const Vector2 start_pos;
	//This will make the volcanos fade in.
	void fade_in();
	//This will make the volcanos fade out.
	void fade_out();
	//This will get rid of all the volcanoes.
	void remove();
	Volcano_controller(const Vector2 &position);
	//This will return true if the volcanoes have an alpha above 0.
	bool get_is_active();
};
DECLARE_SMART(Sun, spSun);
class Sun : public Sprite {
private:
	//This is the position of the sun denoted in the constructor.
	Vector2 init_sun_pos;
	//This in the initial color of the sun.
	Color init_color;
public:
	//This will reset the sun.
	void reset();
	Vector2 start_pos;
	Sun(const Vector2 &position, unsigned int level);

};
DECLARE_SMART(Cloud_Top, spCloud_Top);
class Cloud_Top : public Sprite {
	
public:
	Vector2 start_pos;
	Cloud_Top(const Vector2 &pos);
	void move(float how_much);
};
DECLARE_SMART(Cloud_Bottom, spCloud_Bottom);
class Cloud_Bottom : public Sprite {

public:
	Vector2 start_pos;
	Cloud_Bottom(const Vector2 &pos);
	void move(float how_much);
};
DECLARE_SMART(Background, spBackground);
class Background : public Actor {
private:
	//These are used to tell you to click on the screen.
	spI_Hand _I_Hand_L;
	spI_Hand _I_Hand_R;
	//This will tell you if the player has won the game or not.
	bool * const has_won_game;
	//This is the boundary size of the world.
	float w_bound = 0;
	//This is a set of two volcanos.
	spVolcano_controller volcano_controller;
	//This is used for having a background of cereal and milk.
	spC_Milk cm;
	//This is used for the dark islands.
	spD_Islands island_controller;
	//These are the two volcanos
	//This is the big bunny that appears.
	spBunny big_bunny;
	//This is the rain actor.
	//It handles all the raindrops.
	spRain rain_controller;
	//This is the bubble actor.
	//It handles all the bubbles.
	spBubble bubble_controller;
	//This is the sun's starting position.
	Vector2 sun_s_p;
	//This is a world_ptr.
	void* _world_ptr;
	//This vector holds the integers for the velocity of the balloons.
	random2::RandomStorage_int Background::rand_store_int_vel;
	//This is a vector full of random numbers that represent the location of balloon spawning.
	random2::RandomStorage_int Background::rand_store_int;
	//This is how long it has been waiting since it last spawned a balloon.
	float wait_ticks = 0;
	//This determines how long it should wait before spawning a balloon.
	float max_wait_ticks = 1000;
	//This is a point to the fade color.
	Fade_Color *cb_fade;
	Fade_Color *stage_fade;
	Fade_Color *sun_fade;
	
	//This is the sun in the background.
	spSun sun;
	//Pointer to the world object.
	//World *_world_ptr;
	//Fade_Color *ct_fade;
	//Fade_Color *stage_fade;
	unsigned int current_level;
	//This will create a hot air balloon at a certain altitude.
	void createHotBalloon();
	//This will create a balloon at a random location on the ground.
	void createBalloon();
	//This contains all the theme data.
	//Each index is relative to the level.
	//e.g. level 0, should have index 0 of the vector.
	//std::vector<change_data> themes;
	//change_data *cd; //This is the data used for determining the theme when changing.
	spCloud_Bottom cb;
	spCloud_Top ct;
	static Color Background::stage_color;
public:
	static void pre_init();
	static Color *getStage_Color();
	void init(void *world_ptr);
	Background(void* world_ptr);
	void doUpdate(const UpdateState &us);
	//This will set the theme depedning on what level you input.
	void setTheme(unsigned int level, unsigned int world);
	//This gets the current theme.
	unsigned int getTheme();
};