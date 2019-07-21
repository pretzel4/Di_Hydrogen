#include "Character.h"
#include "ImageRes.h"
#include "oxygine/core/log.h"

Character::Character(World* world, const Vector2& pos, float scale) {
	setResAnim(res::resources.getResAnim("anim"));
	setPosition(Vector2(getStage()->getSize().x / 2, getStage()->getSize().y / 2));
	setAnchor(0.5f, 0.5f);
	//setPosition(pos);
	//create the body properties
	b2BodyDef bodydef;
	//bodydef.type = b2_dynamicBody;
	bodydef.type = b2_dynamicBody;
	bodydef.position = GF::convert(pos, SCALE);

	body = world->getb2World()->CreateBody(&bodydef);

	_world_ptr = world; //Keeping a pointer to the world to be used later.

	setScale(scale);

	b2PolygonShape box;
	b2Vec2 sz = GF::convert(getSize() / 2, SCALE);
	box.SetAsBox(sz.x, sz.y);
	body->CreateFixture(&box, 0.0f);
	body->SetUserData(this);

	vel = body->GetLinearVelocity();
}

Character::~Character() {

}

void Character::doUpdate(const UpdateState& us) {
	const uint8* data = SDL_GetKeyboardState(NULL);
	vel = body->GetLinearVelocity();
	
	float speed = 300.f;
	float jumpspeed = 1000.f;
	//_world_ptr->scroll_coords.y = getPosition().y - (getStage()->getHeight() / 2);
	//_world_ptr->set_Y_origin(getPosition().y);
	if (data[SDL_GetScancodeFromKey(SDLK_d)]) {
		addTween(Sprite::TweenAnim(getResAnim()), 500, 1);
		vel.x += speed *(us.dt / 1000.0f);
		body->SetLinearVelocity(vel);
	}
	if (data[SDL_GetScancodeFromKey(SDLK_a)]) {
		addTween(Sprite::TweenAnim(getResAnim()), 500, 1);
		vel.x -= speed *(us.dt / 1000.0f);
		body->SetLinearVelocity(vel);
	}
	if (data[SDL_GetScancodeFromKey(SDLK_w)]) {
		if(vel.y == 0){
			vel.y = -(jumpspeed *(us.dt / 1000.0f));
			body->SetLinearVelocity(vel);
			
		}
		
	}
	
	_world_ptr->setAnchor(Vector2(getPosition().x - (getStage()->getSize().x / 2), getPosition().y - (getStage()->getSize().y / 2)));
	//_world_ptr->set_origin();
	
	
	//getStage()->setPosition(Vector2(getPosition().x - (getStage()->getSize().x / 2), getPosition().y - (getStage()->getSize().y / 2)));
}

