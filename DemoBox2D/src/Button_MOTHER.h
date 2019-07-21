#pragma once
#include "oxygine-framework.h"
#include <functional>
using namespace oxygine;
class Button_MOTHER : public Sprite {
private:
	

protected:
	bool _is_locked = false;
	//This is called when the button is clicked.
	std::function<void()> lambda;
	//This is the frame that the sprite is on.
	unsigned int frame = 0;
	//This is called when the player has clicked the button.
	virtual void clicked(Event* ev);
	//This is called when the player is hover their cursor over the button.
	virtual void hovered(Event* ev);
	//This is called when the player takes their cursor off the button.
	virtual void hovered_off(Event* ev);
	//This is called when the player lets go off the button.
	virtual void unclicked(Event* ev);
public:
	//This will lock the button(making it so that you cannot click the button).
	void lock();
	//This will unlock the button(making it so you can click the button).
	void unlock();
	//This will return true if the button is currently locked.
	bool is_locked();
	Button_MOTHER();
};