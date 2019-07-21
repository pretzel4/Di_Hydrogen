#include "Ground.h"
#include "World.h"
#include "ImageRes.h"
#include "GF.h"
#include "random2.h"
#include "Balloon.h"
#define ground_move_speed -10
#define ground_fadeIn_speed 2000 * 5
Ground::Ground(const Vector2 &pos, void* world_ptr, unsigned int type) : _type(type), b_c_b(static_cast<World*>(world_ptr)->cam_bottom_ylimits) {
	_world_ptr = world_ptr;
	World* w_ptr = static_cast<World*>(world_ptr);
	float height = res::resources.getResAnim("Block")->getHeight();
	RectF rect = RectF(start_pos, Vector2(World::get_boundary_size(), height));
	switch (type) {
	case 0: //Reset world ground spawn.
		start_pos = pos;
		setResAnim(res::resources.getResAnim("Block"));
		
		break;
	case 1: //End of world ground spawn.
		
		start_pos = pos;
		setResAnim(res::resources.getResAnim("Block_W2"));
		//GF::fadeIn(this, ground_fadeIn_speed, 255);
		break;
	case 2:
		start_pos = pos;
		setResAnim(res::resources.getResAnim("Block_W2"));
		setColor(Color::AntiqueWhite);
		//GF::fadeIn(this, ground_fadeIn_speed, 255);
		break;
	case 3:
		start_pos = pos;
		setResAnim(res::resources.getResAnim("Block_W2"));
		setColor(Color::DarkGray);
		//GF::fadeIn(this, ground_fadeIn_speed, 255);
		break;
	case 4:
		start_pos = pos;
		setResAnim(res::resources.getResAnim("Block_W2"));
		setColor(Color::LawnGreen);
		//GF::fadeIn(this, ground_fadeIn_speed, 255);
		break;
	
	}
	setName("Wall");
	
	
	
	
	//SETTING UP SPRITE-=-
	setPosition(rect.getLeftTop() + (rect.getSize() / 2));
	setAnchor(0.5f, 0.5f);
	setSize(rect.size);
	w_ptr->addChild(this);
	//CREATING BODY-=-
	b2BodyDef bodydef;
	bodydef.type = b2_staticBody;
	bodydef.position = GF::convert(rect.getLeftTop(), GF::get_G_Scale());
	body = w_ptr->getb2World()->CreateBody(&bodydef);
	//CREATING FIXTURE-=-
	b2FixtureDef fd;
	b2PolygonShape box;
	b2Vec2 sz = GF::convert(rect.getSize() / 2, GF::get_G_Scale());
	box.SetAsBox(sz.x, sz.y);
	fd.shape = &box;
	body->CreateFixture(&fd);
	body->SetUserData(this);
	setHorizontalMode(Box9Sprite::StretchMode::TILING_FULL);
	setGuides(0, 0, 0, 0);
	create_props();
	GF::fadeIn(this, ground_fadeIn_speed, 255);

}

void Ground::create_props() {
	
	Vector2 prev_pos(random2::randomrange(0, 15), -64); //This is the position of the last prop created.
	unsigned int how_many = random2::randomrange(7, 14); //This is how many props will be on the ground.
	for (unsigned int i = 0; i < how_many; i++) {
		spSprite prop = new Sprite();
		switch (random2::getBool()) {
		case 0:
			switch (_type) {
			case 0:
				prop->setResAnim(res::resources.getResAnim("Ground_Grass_0"));
				break;
			case 1:
				prop->setResAnim(res::resources.getResAnim("Ground_Grass_0_W2"));
			}
			
			break;
		case 1:
			switch (_type) {
			case 0:
				prop->setResAnim(res::resources.getResAnim("Ground_Flower_0"));
				break;
			case 1:
				prop->setResAnim(res::resources.getResAnim("Ground_Flower_0_W2"));
			}
			
		}
		prop->setPosition(prev_pos);
		prev_pos.x += random2::randomrange(64, 128);
		//prop->attachTo(getStage());
		addChild(prop);
		props.push_back(prop);
	}
}

void Ground::destroy() {
	for (unsigned int i = 0; i < props.size(); i++) {
		props[i]->detach();
	//	props[i].~intrusive_ptr();
	}
	props.clear();
}