#pragma once
#include "Box2D/Box2D.h"
#include "oxygine-framework.h"
#include "World.h"
#include "GF.h"
using namespace oxygine;

class Character : public Sprite {
public:
	Character(World* world, const Vector2& pos,float scale = 1);
	~Character();
	void doUpdate(const UpdateState&);
	
private:
	b2Body* body;
	//This is a pointer to the world, used primarily for scrolling in the y axis.
	World *_world_ptr;
	b2Vec2 vel;
	const float SCALE = 100.0f;
	//bool _has_jumped = false; //When true, you are not allowed to jump.
private:

	
};
typedef oxygine::intrusive_ptr<Character>spCharacter;