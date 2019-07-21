#include "Wall.h"
#include "ImageRes.h"
#include <iostream>
#include <fstream>

Wall::Wall(World* world, RectF &rect, unsigned int type) {
	//setPosition(Vector2(5, 8));
	//
	ColType = 2;
	setName("Wall");

	
	//setPosition(rect.getLeftTop() + (rect.getSize() / 2));
	//setAnchor(0.5f, 0.5f);
	//setSize(rect.size);
	//attachTo(getStage());
	//sprite->addRef();
	
	/*setResAnim(res::resources.getResAnim("Block"));
	setSize(rect.size);
	setPosition(rect.getLeftTop() + (rect.getSize() / 2));
	setAnchor(Vector2(0.5f, 0.5f));
	attachTo(getStage());*/
	setResAnim(res::resources.getResAnim("Block"));
	setup(this, rect, getStage());
	setup_body(rect);
	body->SetUserData(this);
	//body->SetUserData(this);
	
	//attachTo(getStage());
	//const char* output = std::to_string(43534534535).c_str();
	//log::messageln(output);
	
	
	
	
	
	
	
	
	
	/*b2BodyDef bodydef;
	bodydef.type = b2_staticBody;
	bodydef.position = GF::convert(getPosition(), SCALE);
	
	body = world->getb2World()->CreateBody(&bodydef);
	
	b2PolygonShape box;
	b2Vec2 sz = GF::convert(getSize() / 2, SCALE);
	box.SetAsBox(sz.x,sz.y);

	body->CreateFixture(&box, 0.0f);
	body->SetUserData(this);*/
}


Wall::~Wall() {

}

void Wall::doUpdate(UpdateState &us) {

}









