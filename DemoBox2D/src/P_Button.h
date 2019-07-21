#pragma once
#include "Button_MOTHER.h"
#include "oxygine-framework.h"
using namespace oxygine;
DECLARE_SMART(P_Button, spP_Button);
class P_Button : public Button_MOTHER {
private:
	//This is the force lambda, it is called when the pause button is forced into a command, not clicked!
	//This lamb is being forced to do chores.
	std::function<void()> _force_lamb;
	//This determines if the mouse if hovering over the button or not.
	bool is_hovering = false;
	//This will set the animation according the frame variable.
	inline void set_frame();
	//This will toggle the state.
	void s_toggle();
	//This is the state, whereby the button displays. e.g pause symbol, play symbol
	//0 = pause symbol-DEFAULT
	//1 = play symbol
	bool state = 0;
	void clicked(Event* ev);
	
	void hovered(Event* ev);
	
	void hovered_off(Event* ev);
	
	void unclicked(Event* ev);
public:
	//This will essentially just click the button for you.
	void pause();
	P_Button(const Vector2 &position, std::function<void()> lamb, std::function<void()> force_lamb);
	//This will set the button back to its original unpaused state.
	//This will not call the lambda.
	void reset();
};