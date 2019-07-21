#include "B_Flower.h"

B_Flower::B_Flower(const Vector2 &position) {
	setPosition(position);
	_objects.push_back(this);
}