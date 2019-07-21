#include "ST.h"
#include <cmath>
Block::Block(RectF rectangle, const Color &color, float rotation) {
	
	setup(this, rectangle);
	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.position = GF::convert(rectangle.getLeftTop(), GF::get_G_Scale());
	bodydef.angle = rotation;
	bodydef.fixedRotation = false;
	setup_body(rectangle, &bodydef, NULL, NULL);
	setColor(color);
	body->SetUserData(this);
	world_ptr->addChild(this);
}

void Block::onRemovedFromStage() {
	
	
	
	world_ptr->getb2World()->DestroyBody(body);
}

vertexPCT2 BW::initVertex(const Vector2& pos, unsigned int color)
{
	vertexPCT2 v;
	v.color = color;
	v.x = pos.x;
	v.y = pos.y;
	v.z = 0;
	v.u = v.x / 128;
	v.v = v.y / 128;

	return v;
}

vertexPCT2 BW::getVertex(int i, int num, unsigned int color)
{
	float theta = 2.0f * MATH_PI / num * i;

	Vector2 dir = Vector2(scalar::cos(theta), scalar::sin(theta));
	float rad = getStage()->getHeight() * 0.4f * (1 + scalar::sin(theta * 10) / 10);
	Vector2 p = dir * rad;

	Color c = lerp(Color(color), Color(color), scalar::abs(scalar::sin(theta * 3)));
	return initVertex(p, c.rgba());
}

vertexPCT2* BW::createVertices(int num, unsigned int color)
{
	int verticesCount = num * 4;

	vertexPCT2* vertices = new vertexPCT2[verticesCount];

	vertexPCT2* p = vertices;
	for (int n = 0; n < num; ++n)
	{
		//add centered vertex
		*p = initVertex(Vector2(0, 0), color);
		
		++p;

		*p = getVertex(n, num, color);
		
		++p;

		*p = getVertex(n + 1, num, color);
		
		++p;

		//Oxygine uses "triangles strip" rendering mode
		//dublicate last vertex (degenerate triangles)
		*p = getVertex(n + 1, num, color);
		//p->color = Color::BurlyWood;
		++p;
	}

	return vertices;
}

BW::BW(const Vector2 &pos, float radius, float spin_speed) {
	//Making the polygon.
	setPosition(pos);
	poly = new oxygine::Polygon;
	
	vertexPCT2* vertices = createVertices(5, Color::BlueViolet);
	poly->setVertices(vertices, sizeof(vertexPCT2) * 5 * 4, vertexPCT2::FORMAT, true);
	poly->setColor(Color::BlueViolet);
	poly->setPosition(pos);
	poly->setPriority(-10000);
	
	//poly->addTween(Actor::TweenRotation(MATH_PI * 2), 16000, -1);
	world_ptr->addChild(poly);
	
	//poly->setRotation(30);
	//Making the body.
	//-Rectangle
	RectF r;
	r.size = Vector2(radius, radius);
	r.pos = pos;
	//-Shape
	b2CircleShape f;
	f.m_radius = radius;
	//-Fixture definintion
	b2FixtureDef fd;
	fd.filter.categoryBits = ST; //It is a black_hole.
	fd.filter.maskBits = NULL; //Collide with nothing.
	fd.shape = &f;
	fd.density = 1.0f;
	//-Actual creation
	setup_body(r, NULL, &f, &fd);
	body->SetAngularVelocity(spin_speed);
	body->SetUserData(this);
	world_ptr->addChild(this);
}

void BW::onRemovedFromStage() {
	poly->detach();
	//poly.~intrusive_ptr();
	world_ptr->getb2World()->DestroyBody(body);
}

void BW::doUpdate(const UpdateState& us) {
	
	poly->setRotation(body->GetAngle());
}

void BW::update(int num)
{
	vertexPCT2* vertices = createVertices(num, Color::BlueViolet);
	poly->setVertices(vertices, sizeof(vertexPCT2) * num * 4, vertexPCT2::FORMAT, true);
	
}

ST::ST(const Vector2 &rel_pos, const Vector2 &pos, unsigned int type) {
	
	setPosition(pos);
	switch (type) {
	case 0:
		backwheel = new BW(rel_pos + pos, 3, 2.5f);
		break;
	
	}
	
	
	create_parts(rel_pos, pos, type);
	//setPriority(-6000);
	
	
}

void ST::create_parts(const Vector2 &rel_pos, const Vector2 &position, unsigned int type) {
	//RectF r;
	//r.pos = Vector2(0,0);
	//r.size = Vector2(5,5);
	//spBlock b;
	//b = new Block(r, Color(Color::Azure));
	Vector2 pos = rel_pos + position;
	switch (type) {
	case 0:
	{
		float radius = 220;
		spBlock b;
		RectF r;
		r.size = Vector2(40, 40);
		b2WeldJointDef wjd;

		float angle = 0; //IN RADIANS
		int num = 5;
		for (float i = 0; i < 5; i++) {
			angle = (i * MATH_PI / num) + MATH_PI / (num * 2);


			//Defining the rectangle.
			float x = cos(angle) * radius;
			float y = sin(angle) * radius;
			//how_many_90s = floor(angle / 1.57);

			//float b_angle = 3.14 - ((angle - (how_many_90s * 1.57)) + 1.57);
			float b_angle = 0;


			//float b_angle = atan(x / y);
			//Setting position.
			r.pos = pos + Vector2(0, 15);
			//Making block
			b = new Block(r, Color(Color::Azure), 0);
			b2RevoluteJointDef rjd;
			//Making the joint.
			wjd.bodyA = backwheel->getBody();
			wjd.bodyB = b->getBody();
			wjd.localAnchorA = b2Vec2(0, 0);
			wjd.localAnchorB = b2Vec2(GF::convert(Vector2(x, y)));
			wjd.referenceAngle = angle;

			wjd.collideConnected = false;

			GF::weld(&wjd, backwheel->getWorld_ptr());
			_parts.push_back(b);
		}
		break;

		}
	
	}
	
}

void ST::onRemovedFromStage() {
	for (unsigned int i = 0; i < _parts.size(); i++) {
		_parts[i]->detach();
		//_parts[i].~intrusive_ptr();
	}
	_parts.clear();
	backwheel->detach();
	//backwheel.~intrusive_ptr();
}