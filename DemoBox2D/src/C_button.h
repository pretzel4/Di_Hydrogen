#pragma once
#include "oxygine-framework.h"
#include "Text.h"
#include <functional>
#include "Button_MOTHER.h"
using namespace oxygine;
DECLARE_SMART(C_button, spC_button);
class C_button : public Button_MOTHER {
private:
	
protected:
	
	const bool _play_sound;
	//This is the text this is displayed in the center of the button.
	spText _text;
	void clicked(Event* ev);
public:
	C_button(const Vector2 &pos, char* words_to_display, std::function<void()> &callback, bool should_play_sound = true);
	
	//This will set the text object.
	void setText(char* string);

};