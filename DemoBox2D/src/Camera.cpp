#include "Camera.h"
#include "GF.h"
Camera::Camera() {
	_offset = b2Vec2(0, -30);
	setSize(getStage()->getSize());
}

void Camera::update() {
	setPosition(GF::convert(b2Vec2(50,-100), GF::get_G_Scale()));
	
}

void Camera::add(spActor &actor) {
	addChild(actor);
}