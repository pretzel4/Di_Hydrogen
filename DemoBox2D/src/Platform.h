#pragma once
#include "S_Visible_M3.h"
#include "oxygine-framework.h"
#include "Engine_M.h"
#include "D_Line.h"
#include "W_Line.h"
#include "Spike.h"
#include "P_System.h"
#include "Bird_Nest.h"
#include "TPIN.h"
using namespace oxygine;


DECLARE_SMART(Platform, spPlatform);
class Platform : public S_Visible_M3, public Box9Sprite {
private:
	
	//Vector2 position;
	//This is what determines if the ship is close enough to notice the platform or not.
	spTPIN _tpin;
	
	//This is the average height between two platforms.
	static float Platform::average_height;
	//This is the x coordinate that the final platform will have.
	static int Platform::final_x;
	//This will fire the two cannons.
	void fire_cannons();
	//These are two confetti cannons(pretty much).
	spP_System confetti_left;
	spP_System confetti_right;
	//This determines if the platform has been iterated yet.
	bool has_iterated = false;
	//This determines if it should be able to make more platforms.
	static bool Platform::can_create_platforms;
	//This is a vector full of objects that are special to the specific platform type that it is.
	std::vector<spActor> specials;
	enum types {
		NORMAL = 0,
		UPSIDE_DOWN_FORCEFIELD = 1,
		BIRD_NEST = 2
	};
	//This is the type of platform that it is.
	types type;
	//This is a line that is shown only if the platform is the last one of the level.
	//This will point to nothing otherwise. So please be careful when referencing this member.
	spW_Line end_line;
	//This counts how many platforms have been created since the last reset call to this class.
	static int Platform::how_many_platforms;
	//This determines if the platform should set the world into a new sector.
	bool is_final_platform = false;
	//This is the last threshold setting given.
	static int Platform::start_threshold;
	//This is the original place that the platforms were created.
	static Vector2 Platform::start_pos;
	//This is used to help the player find the next platform.
	spD_Line _dline;
	//This is the initial position at which the platform was created.
	const Vector2 _init_pos; //pos1
	//This is the second position for the dline.
	const Vector2 _init_pos2;
	//This is the angle in radians for the d_line.
	const float d_angle_rads;
	//These are the platforms that are waiting to be completely deleted.
	static std::vector<spPlatform> Platform::should_delete;
	//This is the original creation spot of the platforms.
	//This is where the first platform will start when you recreate the objects.
	static Vector2 Platform::restart_pos;
	static bool Platform::_should_delete;
	//This boolean is used to determine if the platform is ready to be deleted when you fly off of it.
	bool _has_been_landed = false;
	//This determines what should be deleted next frame.
	static spPlatform Platform::to_delete;
	//This will create a new platform in a random location relative to the given coordinates.
	static Vector2 create_new_platform(const Vector2 &rel_pos, bool is_final = false);
	//This function will make the platform fade out, instead of just instantly dissapearing.
	void fadeOut_and_destroy();
	//This is called once the platform is done fading out.
	void done_fading(Event *ev);
	//This is called every frame.
	void doUpdate(const UpdateState &us);
public:
	//typedef intrusive_ptr<Platform> spPlatform;
	static std::vector<spPlatform> Platform::objects;
	//This function will create all new platforms after the newest one and will delete the older ones except the one given.
	static void reset_platforms_except(Platform * ptr, float shift_height);

	static void create_platforms(const Vector2 &init_pos, int threshold);
	//This creates a platform, that once you land on it and take off again, it will disapear.
	Platform(const Vector2 &pos, int number_of_blocks, float angle, const Vector2 &pos2, float dist);
	//This gets a const pointer to all the platform objects.
	static const std::vector<spPlatform> *getObjects();
	//This will clean up the class of any nullptrs or objects left around.
	//Call this when you load up a new world or have to delete everything.
	static void clean();
	void check_start(b2Body *ptr);
	void check_end(b2Body *ptr);
	//This is called when the platform has been landed on by the ship.
	void has_landed();
	//This will notify the update function, so that it can delete the platform and all the ones behind it.
	void iterate();
	//This function should be called every frame. It handles deletion and creation of platform objects.
	static void tick();
	//This will clean up the platform in a special way.
	//This is specificly used for the very final platform.
	void special_destroy();
	//This will clean up everything.
	void destroy();
	//This will return true if this is the final platform.
	bool get_is_final_platform();
	//This will destroy everything, and then rebuild new platforms.
	static void reset_platforms();
	//This gets the position of the first platform created.
	//If there are no platforms in the world when this is called, it will return NULL.
	static Vector2 *get_first_platform_pos();
	//This is called to initialize everything.
	static void init(World* world_ptr);
	//This will clean up the platforms properly. Only call this when the window is closed.
	static void stop_tweens();
	//This will get the body of the platform.
	b2Body *getBody() { return body; };
};
