#pragma once
#include "Box2d/Box2d.h"
#include "oxygine-framework.h"
#include "GF.h"
#include "Box2DDebugDraw.h"
#include "Camera.h"
#include "GUI.h"
#include "Background.h"
#include "Ground.h"
#include "Wind.h"

using namespace oxygine;

class World : public Actor {
public:
	//This is a vector full of the actors that need to be shifted when a new level is loaded.
	static std::vector<Actor*> World::to_shift;
	//This is called when the gui is done fading out when you win.
	void done_fading_win(Event *ev);
	//This determines if the winning function has been called.
	bool has_win_function_called = false;
	//This tells you whether or not you are a winner.
	bool _did_win_game = false;
	bool did_win_game();
	void win_game();
	//This will get a constant pointer to the _did_win_game boolean.
	bool &get_dwg_ptr();
	//This determines if the world is paused or not.
	bool is_paused = false;
	//This will tell you if the world is paused or not.
	bool get_is_paused();
	//This pauses the world.
	void pause();
	//This unpauses the world.
	void unpause();
	//This will toggle the pause function of the world.
	void pause_toggle();
	//This is how far down the camera will go.
	float cam_bottom_ylimits = 0;
	//This is a pointer to the platform that shouldn't be deleted when you call ready_to_iterate();
	void* platform_ptr = NULL;
	//This is the ground that you start on.
	spGround ground;
	//This is the background of the game-Clouds,color, etc.
	spBackground bg;
	//If this is true, then it will reset the world.
	bool should_reset_all = false;
	//This is the height the new origin should be at when iterating.
	float new_height = 0;
	//This tells what level you are on.
	unsigned int level = 1;
	//This keeps track of the wind.
	Wind wind_controller;
	//This holds all the bodies that are to be destroyed.
	std::vector<b2Body *> d_cache;
	enum objects {
		chainsaw = 0
	};
	World();
	~World();
	spBox2DDraw _debugDraw;
	
	void doUpdate(const UpdateState&);
	b2World * getb2World(); //Will return the b2World pointer.
	spGUI gui;
	//This will make it so that you are no longer winning the game.
	void stop_winning();
	//This will reset all the platforms.
	void reset_platforms();
	//This will check to see if the position given is within the boundaries of the world.
	//Will return true if it is in the boundary; otherwise, it will return false.
	//The threshold decreases the boundary size depending on what you input.
	bool is_within_boundary(const Vector2 &check_pos, const Vector2 &threshold);
	//This will set the camera's position. As long as it is within the boundaries.
	void set_cam_pos(const Vector2 &pos);
	
	//This will set up a new world.
	void setup_newworld(unsigned int world);
	//This will get the left boundary position; the furthest left the camera is willing to go.
	static float get_boundary_left();
	//This will get the right boundary position; the furthest right the camera is willing to go.
	static float get_boundary_right();
	//This will get the size of the boundary area; where the camera is willing to go.
	static float get_boundary_size();
	//This will increase the level by one, and load up the new world.
	//WARNING: INPUT NEW_HEIGHT AS OXY VECTOR TYPE, NOT B2.
	//If a platform pointer is given, then it will calculate it for all the platforms except the one given;
	//Otherwise, it will calculate all of them.
	void ready_to_iterate(float New_Height, void* pltfrm_ptr = NULL);
	//This will increase the platform counter in the GUI by the given amount.
	void increase_platform_counter(int addend);
	//This will return the level for you.
	unsigned int getLevel();
	//This will tell the world that is ready to reset the next frame.
	void set_RESET_ALL();
	//This will set the theme of the background.
	void setTheme(unsigned int new_level, unsigned int world);
	//This will give you the worldNum.
	unsigned int getworldNum();
	//This will iterate the worldNum.
	void iterate_to_next_world();
	//This will shift the position from the last world origin shift.
	void shift_pos(Vector2 &pos);
	//This will tell you if this is the last world.
	bool is_final_world();
	//This will put the object given into a vector, which in turn will shift the position of the actor when the world is shifted.
	void insert_shifter(Actor* to_insert);
	//This will return true if the world is on the final level.(space).
	bool is_final_level();
private:
	//This function is called whenever you win the game.
	void if_game_win();
	//This is how many levels you have gone through.
	//This is the world(how many iterations you have gone through the three levels.).
	unsigned int worldNum = 4;
	//This will reset the game and put everything back to where it should be.
	void RESET_ALL();
	//This function will move all the children of the world in the y axis.
	void shift_children(float y_axis_amount);
	//This tells if the world is ready to load up a new theme.
	//If the rti == -1, then it is not looking to iterate to the next level.
	//If the rti == 0, then it will iterate to the next level without exceptions of updates.
	//If the rti == 1, then it will iterate to the next level with the exception of not deleting the platform pointer.
	int rti = -1;
	void iterate_to_next_level();
	b2World* world;
	const float SCALE = 100.0f;
	const float FIXED_TIMESTEP = 1.0f / 60.0f;
	
private://Convert and Common

	void create_blocks();//This will create the entire world of blocks.
	void create_platform(const Vector2 &pos, int widt);
	

private: //Tile creations
	void create_wall(RectF &rect);
};

typedef oxygine::intrusive_ptr<World> spWorld;
