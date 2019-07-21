#pragma once
#include "oxygine-framework.h"
#include "Character.h"
using namespace oxygine;

class PlayButton : public Sprite 
{
public:
	PlayButton(spCharacter ply);
	~PlayButton();
	void moveCharacter(Event*);
private:
	spCharacter player;
};
typedef oxygine::intrusive_ptr<PlayButton>spPlayButton;