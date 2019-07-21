#include "Bird_Nest.h"
#include "ImageRes.h"
#include "World.h"
Bird_Nest::Bird_Nest(const Vector2 &position, void* world_ptr) : _world_ptr(world_ptr) {
	World* w_ptr = static_cast<World*>(world_ptr);
	//Setting up sprite.
	setPosition(position);
	setResAnim(res::resources.getResAnim("Bird_Nest"));
	w_ptr->addChild(this);


	//Setting up body.
	b2BodyDef bd;
	bd.position = GF::convert(position);
	bd.type = b2_staticBody;
	body = w_ptr->getb2World()->CreateBody(&bd);
	//b2PolygonShape ps;
	//b2Vec2 sz = GF::convert(Vector2(getWidth(), 1));
	//ps.SetAsBox(sz.x / 2,sz.y, GF::convert(Vector2(getWidth() / 2, getHeight() - 1)), 0);
	b2PolygonShape ps_L;
	b2Vec2 sz2 = GF::convert(Vector2(4, getHeight()));
	ps_L.SetAsBox(sz2.x / 2, sz2.y / 2, GF::convert(Vector2(0,getHeight() / 2)), 0);
	b2PolygonShape ps_R;
	b2Vec2 sz3 = GF::convert(Vector2(4, getHeight()));
	ps_R.SetAsBox(sz3.x / 2, sz3.y / 2, GF::convert(Vector2(getWidth() - 4, getHeight() / 2)), 0);
	//b2FixtureDef fd;
	//fd.shape = &ps;
	b2FixtureDef fd_R;
	fd_R.shape = &ps_R;
	b2FixtureDef fd_L;
	fd_L.shape = &ps_L;
	
	//body->CreateFixture(&fd);
	body->CreateFixture(&fd_R);
	body->CreateFixture(&fd_L);

	//Etc
	body->SetUserData(this);
}

void Bird_Nest::onRemovedFromStage() {
	World* w_ptr = static_cast<World*>(_world_ptr);
	w_ptr->getb2World()->DestroyBody(body);
}