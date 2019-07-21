#pragma once
#include "Ship_Part_M.h"
#include "Box2D\Box2D.h"

/*This class is used to distinguish the engines apart from normal parts on the ship.*/
class Engine_M : public Ship_Part_M {

protected:
	
	//This is used to reference the main ship.
	//Ship_M *ship_ptr;
	//This is how strong the engine is.
	float _impulse = 2;
	//This tells if the engine is on or off.
	bool _is_on = false;
	//This checks to see what you have landed on.
	//void BeginContact(b2Contact* contact);
	
public:
	//This is called when the sprite is detached from the screen.
	Engine_M();
	//This is called whenever the engine makes contact with a platform.
	void ifcontact_platform_start();
	//This is called whenever the engine stops making contact with a platform.
	void ifcontact_platform_end();
	//This tells if the engine is settled on a platform or not.
	bool is_settled = false;
	//This holds the data to the most recent contact this engine has made.
	b2Contact *most_recent;
	//This tells if the ship's engine is on or not.
	//return true = is on.
	//return false = is off.
	bool is_on();
	//This toggles the engine.
	void toggle();
	//This sets the engine's state(on/off);
	//para = true = is on.
	//para = false = is off.
	virtual void set_state(bool new_state);

};
typedef intrusive_ptr<Engine_M> spEngine_M;