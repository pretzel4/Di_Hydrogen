#pragma once
#include "oxygine-framework.h"
#include "Box2D/Box2D.h"
#include "World.h"
#include "Engine_M.h"
#include "Text.h"
#include "Fuel_Gauge.h"
#include "I_control.h"
#include <vector>
#include "Ship_Torso.h"
#include "TPI_Line.h"
#include "Noise.h"
//#include "Ship_Engine.h"
using namespace oxygine;
class IC_Line {
private:
	//These are all the lines that are pending their destruction.
	std::vector<spTPI_Line> _to_destroy;
	//This is a point to the world.
	World *_world_ptr;
	//This holds all the TPI lines asociated with the ship.
	std::vector<spTPI_Line> _lines;
	//This is the position that was given. It will change depending out external factors.
	const Vector2 &_position;
	//This is called when the line is done fading.
	void done_fading(Event *ev);
	
public:
	//This is called every frame.
	void tick();
	IC_Line(const Vector2 &position, World* world_ptr);
	//This will create a line to the position given.
	spTPI_Line create(const Vector2 &position, float radius, unsigned int type);
	//This will properly delete the I_Line.
	void destroy(spTPI_Line to_delete);
	//This will make the line fade out and then delete itself.
	void fadeOut_and_delete(spTPI_Line line);
	//This will clear all the lines. //THIS WILL NOT FADE THEM OUT.
	void delete_all();
	//This will shift all the line's second positions down a given amount.
	void shift(float shift_amount);
	
};
DECLARE_SMART(Ship_M, spShip_M);
class Ship_M : public Actor {
private:
	bool sound_toggle = true;
	//This tells if it is the first time playing the rocket sounds or not.
	bool hfr = false; //Has Fired Rocket
	//This object is used for making sound only while the engine is on.
	Noise::instance _in = Noise::instance(Noise::st::Rocket_Fire1, Noise::player::sfx, PlayOptions().loop(true));
	//This is the current position of the ship.
	Vector2 c_pos;
	
	//This determines if the ship has allocated parts or not.
	bool is_allocated = true;
	//This tells if the ship_M is still in working order, all criteria must be met for it to be true:
	//1-> Still has more then zero fuel.
	//2-> has all the parts attached
	//3-> all parts must exist in the world.
	bool _is_in_working_order = true;
	//This boolean tells if the right engine should turn on or not.
	bool _r_engine_switch_is_on = false;
	//This boolean tells if the left engine should turn on or not.
	bool _l_engine_switch_is_on = false;
	
	//This is the ship's body.
	b2Body *body;
	//This tells all the parts what player it belongs to.
	int playerIndex = 0;
	//These contain integers to the controls that you want to use for the player.
	//Control[0] = Left engine
	//Control[1] = Right engine
	unsigned int controls[2];
	
	//This is how much fuel you have left in the ship.
	float fuel = 100;
	//This will create the parts that are attached to the main object of the ship.
	inline void create_parts();
	//This tells if the ship is waiting to gain control again.
	//With this true, the ship cannot be controlled, or moved.
	bool is_waiting = true;
	//This is the original position where the ship started.
	Vector2 start_pos;
	//This is called every frame.
	void doUpdate(const UpdateState& us);
	//This contains all the parts to the ship, excluding the engines.
	std::vector<spShip_Part_M> _parts;
	//This contains all the joints.
	std::vector<b2Joint *> _joints;
	//This is the right engine.
	spEngine_M _r_engine;
	//This is the left engine.
	spEngine_M _l_engine;
	
	//This is the velocity of the ship.
	b2Vec2 vel;
	
	//This is called when the camera is done moving back to the starting position, when resetting.
	void end_reset(Event *event);
	//This will set all the ship's parts to inactive.
	void set_active(bool is_active);
	//This is a pointer to the world.
	spWorld _world_ptr;
	//This tells if the ship should destroy the joints or not.
	bool should_destroy_joints = false;
	//This tells if the ship has crashed or not.
	bool has_crashed = false;
	friend I_control;
	//This function will apply aerodynamic properties to the ship.
	//This should be called every frame you want aerodynamics to be calculated.
	void update_aerodynamics();
	//This function will properly deallocate the memory for all the parts.
	void deallocate_parts();
	//This will properly clean the object up.
	void destroy();
protected:
	
public:
	//This is the torso of the ship.
	spShip_Torso _torso;
	//This will fade out the engine's sounds.
	void fade_out_engine_sound();
	//This controls all the TPI_Lines.
	IC_Line line_controller;
	void set_is_waiting(bool new_state);
	//This tells if the ship_M is still in working order, all criteria must be met for it to be true:
	//1-> Still has more then zero fuel.
	//2-> has all the parts attached
	//3-> all parts must exist in the world.
	bool is_in_working_order();
	//This will turn on the engine when you have clicked on the gui.
	void click_engine_right(Event* event);
	//This will turn on the engine when you have clicked on the gui.
	void click_engine_left(Event* event);
	//This will turn off the engine when you have released the mouse off the button.
	void unclick_engine_right(Event* event);
	//This will turn off the engine when you have released the mouse off the button.
	void unclick_engine_left(Event* event);

	//This resets the ship back to it's starting point and builds it back together.
	void reset();
	//This is called when the ship has crashed.
	void if_crashed(Actor* other_actor);
	enum control_types {
		a_d = 0,
		arrow_l_r = 1
	};
	//This is used to store player pointers.
	static std::vector<spShip_M> Ship_M::objects;

	//This will return true if both engines are not currently settled.
	bool are_both_engines_not_settled();
	//This gets the fuel variable.
	float getFuel() {
		return fuel;
	}
	//This gets the has_crashed variable.
	bool get_has_crashed() {
		return has_crashed;
	}
	//This will refuel the fuel of the ship.
	void refuel();
	//This will check the engines to see if the ship has landed.
	bool engine_check();
	//This will hand information down. Will not modify rocket.
	unsigned int get_engine_info();
	//This makes the ship.
	//This is the object(the character) you control in the world.
	Ship_M(World* world, const Vector2& pos, control_types c_type, float scale = 1);
	//This is the destructor used to clear the vector to make sure that there is no dangling pointers.
	~Ship_M();
	//This will get the difference in the start_pos and the current position of the ship.
	Vector2 get_dif_from_start();
	//This will deallocate the all the ships' memory.
	static void deallocate();
	//This will allocate the memory for the ships.
	//You should only call this when you have deallocated the memory. void deallocate();
	static spShip_M allocate(World* world_ptr);
	
};
