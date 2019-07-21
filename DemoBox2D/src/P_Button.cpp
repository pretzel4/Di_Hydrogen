#include "P_Button.h"
#include "ImageRes.h"
#include "Debug.h"
P_Button::P_Button(const Vector2 &position, std::function<void()> lamb, std::function<void()> force_lamb) {
	lambda = lamb;
	_force_lamb = force_lamb;
	setResAnim(res::resources.getResAnim("Pause_Button"));
	setPosition(position);
	setAnchor(0.5f, 0.5f);

}

void P_Button::s_toggle() {
	if (state) {
		state = false;
	}
	else {
		state = true;
	}
}

void P_Button::pause() {
	frame = 2;
	set_frame();
	lambda();
}

void P_Button::set_frame() {
	if (state) {
		setAnimFrame(getResAnim(), frame + 3);
	}
	else {
		setAnimFrame(getResAnim(), frame);
	}
}

void P_Button::clicked(Event* ev) {
	pause();
}

void P_Button::hovered(Event* ev) {
	is_hovering = true;
	frame = 1;
	set_frame();
}

void P_Button::hovered_off(Event* ev) {
	is_hovering = false;
	if (frame != 2) {
		frame = 0;
		set_frame();
	}
	

}

void P_Button::reset() {
	state = 0;
	_force_lamb();
	set_frame();
}

void P_Button::unclicked(Event* ev) {
	
	s_toggle();
	if (!is_hovering) {
		frame = 0;
		set_frame();
	}
	else {
		if (frame == 0) {
			frame = 0;
			set_frame();
		}

		else {
			frame = 1;
			set_frame();
		}
	}
	
	
}
