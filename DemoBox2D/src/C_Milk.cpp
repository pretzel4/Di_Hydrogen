#include "C_Milk.h"
#include "ImageRes.h"
C_Milk::C_Milk(const Vector2 &position) : start_pos(position) {
	setPosition(position);
	setResAnim(res::resources.getResAnim("C_M"));
}
