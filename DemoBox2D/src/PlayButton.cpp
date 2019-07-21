#include "PlayButton.h"
#include "ImageRes.h"
PlayButton::PlayButton(spCharacter ply) {
	player = ply;
	setResAnim(res::resources.getResAnim("Skele_man"));
	setAnchor(0.5, 0.5);
	setPosition(Vector2(getStage()->getSize().x / 2, getStage()->getSize().y / 2));
	
	addEventListener(TouchEvent::CLICK, CLOSURE(this, &PlayButton::moveCharacter));
}

PlayButton::~PlayButton() {

}

void PlayButton::moveCharacter(Event* ev) {
	
}