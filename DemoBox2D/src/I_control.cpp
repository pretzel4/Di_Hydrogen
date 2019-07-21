#include "I_control.h"
#include "Ship_M.h"
I_control::I_control(types type, void* player) {
	
	switch (type) {
	case 0: //Up
		addEventListener(TouchEvent::TOUCH_DOWN ,CLOSURE(static_cast<Ship_M*>(player), &Ship_M::click_engine_left));
		addEventListener(TouchEvent::TOUCH_UP, CLOSURE(static_cast<Ship_M*>(player), &Ship_M::unclick_engine_left));
		setPosition(Vector2(0, 0));
		setSize(Vector2(getStage()->getWidth(), getStage()->getHeight() / 2));
		break;
	case 1: //Right
		addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(static_cast<Ship_M*>(player), &Ship_M::click_engine_right));
		addEventListener(TouchEvent::TOUCH_UP, CLOSURE(static_cast<Ship_M*>(player), &Ship_M::unclick_engine_right));
		setPosition(Vector2(getStage()->getWidth() / 2, 0));
		setSize(Vector2(getStage()->getWidth() / 2, getStage()->getHeight()));
		break;
	case 2: //Down
		addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(static_cast<Ship_M*>(player), &Ship_M::click_engine_right));
		addEventListener(TouchEvent::TOUCH_UP, CLOSURE(static_cast<Ship_M*>(player), &Ship_M::unclick_engine_right));
		setPosition(Vector2(0, getStage()->getHeight() / 2));
		setSize(Vector2(getStage()->getWidth(), getStage()->getHeight() / 2));
		break;
	case 3: //Left
		addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(static_cast<Ship_M*>(player), &Ship_M::click_engine_left));
		addEventListener(TouchEvent::TOUCH_UP, CLOSURE(static_cast<Ship_M*>(player), &Ship_M::unclick_engine_left));
		setPosition(Vector2(0, 0));
		setSize(Vector2(getStage()->getWidth() / 2, getStage()->getHeight()));
		break;
	}
	attachTo(getStage());

}