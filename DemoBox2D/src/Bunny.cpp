#include "Bunny.h"
#include "ImageRes.h"
#include "GF.h"
#define bunny_fade_speed 3000 //Three seconds
Bunny::Bunny(const Vector2 &position) : start_pos(position){
	setPosition(position);
	setResAnim(res::resources.getResAnim("Big_Bunny"));

}

void Bunny::onRemovedFromStage() {
}

void Bunny::doUpdate(const UpdateState &us) {
	
}