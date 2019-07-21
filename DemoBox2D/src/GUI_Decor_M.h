#pragma once
#include "oxygine-framework.h"
using namespace oxygine;
#define default_fade_time 3000
class GUI_Decor_M : public Sprite {

protected:
	//This contains all the objects create by GUI_Decor_M.
	static std::vector<spActor> GUI_Decor_M::_objects;
	//This contains all the objects that should be deleted.
	static std::vector<spActor> GUI_Decor_M::_to_delete;

	//This will clear the cache of _to_delete objects.
	static void clear_cache();
protected:
	//This can be called at the end of a tween.
	void tween_d_check(Event *ev);
public:

	//This should be called every frame.
	static void tick();
	//This will check to see if the object should be detached or not.
	//If it has already been detached, then this will not crash the game, it will do nothing instead.
	void d_check();
	//This will make the prop fade out.
	//This will detach the sprite when done.
	void fadeOut(unsigned int milliseconds = default_fade_time);
	//This will make the prop fade in.
	void fadeIn(Actor* to_attach, unsigned int milliseconds = default_fade_time);
	

};