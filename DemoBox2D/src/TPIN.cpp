#include "TPIN.h"
#include "Ship_M.h"
TPIN::TPIN(const Vector2 &position, float radius, World* world_ptr, type Type) : _pos(position), _radius(radius), num_of_parts_within(0) {
	
	_type = Type;
	b2BodyDef bd;
	bd.position = GF::convert(position);
	setName("TPIN");
	body = world_ptr->getb2World()->CreateBody(&bd);
	_world_ptr = world_ptr;
	bd.type = b2_staticBody;
	b2CircleShape shape;
	shape.m_radius = radius;
	b2FixtureDef fd;
	fd.isSensor = true;
	fd.shape = &shape;
	body->CreateFixture(&fd);
	body->SetUserData(this);
	world_ptr->addChild(this);

}

void TPIN::SetPosit(const Vector2 &new_pos) {
	_pos = new_pos;
}

unsigned int TPIN::getType() {
	return _type;
}

void TPIN::when_entering(Ship_Part_M *part_ptr) {
	
	//if (num_of_parts_within == 0) {
	if (num_of_parts_within == 0) {
		Ship_M* player = Ship_M::objects[part_ptr->playerIndex].get();
		_i_ptr = player->line_controller.create(_pos, _radius, _type);
	}
		
		
	//}
	
	num_of_parts_within++;
	
}

void TPIN::when_leaving(Ship_Part_M *part_ptr) {
	num_of_parts_within--;
	
	if (num_of_parts_within == 0) {
		Ship_M* player = Ship_M::objects[part_ptr->playerIndex].get();
		player->line_controller.fadeOut_and_delete(_i_ptr);
		//_i_ptr = NULL;
	}
}

void TPIN::onRemovedFromStage() {
	_world_ptr->getb2World()->DestroyBody(body);
	if (_i_ptr.get() != NULL) {
		Ship_M::objects[0]->line_controller.destroy(_i_ptr);
	}
	
}

void TPIN::fadeOut(int fadeTime) {
	
	//if (num_of_parts_within != 0) {
		Ship_M::objects[0]->line_controller.fadeOut_and_delete(_i_ptr);
		//_i_ptr->fadeOut(fadeTime)->detachWhenDone();
	//}
}

