#include "Dynamic_M2.h"

Dynamic_M2::Dynamic_M2() {
	
}

b2Fixture* Dynamic_M2::setup_body(RectF &rect, b2BodyDef *overwrite_body, b2Shape *overwrite_shape, b2FixtureDef *fixture_def) {
	
	if (overwrite_body == NULL) {
		b2BodyDef bodydef;
		bodydef.type = b2_dynamicBody;
		bodydef.position = GF::convert(rect.getLeftTop(), GF::get_G_Scale());
		body = world_ptr->getb2World()->CreateBody(&bodydef);
	}
	else {
		body = world_ptr->getb2World()->CreateBody(overwrite_body);
	}
	b2FixtureDef dfd;
	dfd.density = 1.0f;
	b2FixtureDef *fd = fixture_def;
	if (fd == NULL) {
		fd = &dfd;
	}
	if (overwrite_shape == NULL) {
		
		
		b2PolygonShape box;
		b2Vec2 sz = GF::convert(rect.getSize() / 2, GF::get_G_Scale());
		box.SetAsBox(sz.x, sz.y);
		fd->shape = &box;
		return body->CreateFixture(fd);
		
	}
	else {
		fd->shape = overwrite_shape;
		return body->CreateFixture(fd);
	}


}

void Dynamic_M2::setup_body_adv(std::vector<Vector2> &vertices) {
	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	body = world_ptr->getb2World()->CreateBody(&bodydef);
	b2PolygonShape shape;
	for (unsigned int i = 0; i < vertices.size(); i++) {
		shape.m_vertices[i] = GF::convert(vertices[i], GF::get_G_Scale());
	}
	body->CreateFixture(&shape, 0.0f);

}