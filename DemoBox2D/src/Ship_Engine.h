#pragma once
#include "Engine_M.h"
#include "Ship_M.h"
#include "P_System.h"
#include "Noise.h"
class Ship_Engine : public Engine_M {
private:
	bool hfr = false;
	bool sound_toggle = true;
	//This is the ship's max fuel.
	float _max_fuel;
	//This points to the ship's fuel.
	float *_fuel;
	//This shoots out a bunch of particles out the engine.
	spP_System Exhaust;
	//This updates the engine every frame.
	void doUpdate(const UpdateState &us);
	//This is called when the object is remove from the stage/deleted.
	void onRemovedFromStage();
	//This object is used for making sound only while the engine is on.
	Noise::instance _in = Noise::instance(Noise::st::Rocket_Fire0, Noise::player::sfx, PlayOptions().loop(true));
	//This tells if it is the first time firing the engines.
	bool is_first_time = false;
	//This tells if it is the first time firing the engines.
	bool is_first_time_fe = false;
public:
	
	//This will stop the noise being played by the engines.
	void stop_sound();
	//Default constructor
	Ship_Engine();
	void set_state(bool new_state);
	//This is an engine for the ship, it provides lift.
	Ship_Engine(Vector2 pos, int Playerindex, std::vector<b2Joint *> &joints, float *ship_fuel, float max_ship_fuel);
	
};
typedef intrusive_ptr<Ship_Engine> spShip_Engine;