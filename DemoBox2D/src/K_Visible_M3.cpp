#include "K_Visible_M3.h"

void K_Visible_M3::setup(Actor *sprite, RectF &rect, const spActor &attachment_obj) const {
	sprite->setPosition(rect.getLeftTop() + (rect.getSize() / 2));
	sprite->setAnchor(0.5f, 0.5f);
	sprite->setSize(rect.size);
	//sprite->attachTo(getStage());
	

}