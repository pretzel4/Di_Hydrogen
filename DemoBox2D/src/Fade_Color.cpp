#include "Fade_Color.h"
#define max_color_value 255

Color Fade_Color::update(const UpdateState &us) {
	unsigned int set_cb_color[4];
	if (tick >= how_long) {
		_is_fading = false;
		
		if (is_lambda == true) {
			//Calling lambda
			lambda();
		}
		
	}
	else {
		tick += us.dt;
	}
	//Updating the colors according to percentage of tick to how_long.
	for (unsigned int i = 0; i < 4; i++) {
		if (cb_change[i] < 0) {	
			set_cb_color[i] = old_cb_color[i] + (-cb_change[i] * (tick / how_long));
		}
		else {
			set_cb_color[i] = old_cb_color[i] - (cb_change[i] * (tick / how_long));
		}	
	}
	//Checking to make sure that none of values are above the max_color_value; otherwise, it would loop around and give weird colors.
	for (unsigned int i = 0; i < 4; i++) {
		if (set_cb_color[i] > max_color_value) {
			set_cb_color[i] = max_color_value;
		}
		else if(set_cb_color[i] < 0) {
			set_cb_color[i] = 0;
		}
	}
	return Color(set_cb_color[0], set_cb_color[1], set_cb_color[2], set_cb_color[3]);;
}

bool Fade_Color::is_active() {
	return _is_fading;
}

void Fade_Color::setup(const Vector4 &current_color, const Vector4 &new_color, float fade_time) {
	tick = 0;
	how_long = fade_time;
	if (did_call_setup2 == false) {
		is_lambda = false;
		
	}
	else {
		did_call_setup2 = false;
	}
	this;
	_is_fading = true;
	//WHAT THE COLOR STARTS AS.
	for (unsigned int i = 0; i < 4; i++) {
		old_cb_color[i] = current_color[i];
	}

	//FINDING DIFFERENCE BETWEEN THE TWO; NEW COLOR AND OLD COLOR.
	for (unsigned int i = 0; i < 4; i++) {
		cb_change[i] = int(current_color[i] - new_color[i]);
	}
	
}

void Fade_Color::setup(const Vector4 &current_color, const Vector4 &new_color, std::function<void()> callback, float fade_time) {
	did_call_setup2 = true;
	is_lambda = true;
	lambda = callback;
	setup(current_color, new_color, fade_time);
}
