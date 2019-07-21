#include "Button_MOTHER.h"

Button_MOTHER::Button_MOTHER() {
	addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &Button_MOTHER::clicked));
	addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &Button_MOTHER::unclicked));
	addEventListener(TouchEvent::OVER, CLOSURE(this, &Button_MOTHER::hovered));
	addEventListener(TouchEvent::OUTX, CLOSURE(this, &Button_MOTHER::hovered_off));
}

void Button_MOTHER::clicked(Event* ev) {
	if (!_is_locked) {
		frame = 2;
		setAnimFrame(getResAnim(), frame);
		lambda();
	}
	
}

void Button_MOTHER::hovered(Event* ev) {
	frame = 1;
	setAnimFrame(getResAnim(), frame);
}

void Button_MOTHER::hovered_off(Event* ev) {
	frame = 0;
	setAnimFrame(getResAnim(), frame);

}

void Button_MOTHER::unclicked(Event* ev) {
	if (frame == 0) {
		frame = 0;
		setAnimFrame(getResAnim(), frame);
	}
	else {
		frame = 1;
		setAnimFrame(getResAnim(), frame);
	}

}

void Button_MOTHER::lock() {
	_is_locked = true;
}

void Button_MOTHER::unlock() {
	_is_locked = false;
}

bool Button_MOTHER::is_locked() {
	return _is_locked;
}