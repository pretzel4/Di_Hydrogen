#include "Final_Bird.h"
#include "ImageRes.h"
#include "GF.h"
#include "World.h"
#define eye_ball_rotation_speed 2000
#define jaw_strength 90
T_Sign::T_Sign(const Vector2 &position) {
	setPosition(position);
	setResAnim(res::resources.getResAnim("Bird_Thanks"));
}

Jaw::Jaw(const Vector2 &position, void* world_ptr) {
	setPosition(position);
	setResAnim(res::resources.getResAnim("Bird_Jaw"));
	setName("NOTHING");
	//Unpacking the world object.
	World* w_ptr = static_cast<World*>(world_ptr);
	//Creating the body.
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position = GF::convert(position);
	body = w_ptr->getb2World()->CreateBody(&bd);
	
	//Creating the fixture.
	b2PolygonShape poly_shape;
	b2FixtureDef fd;
	b2Vec2 vertices[3];
	float G = GF::get_G_Scale();
	vertices[0].Set(GF::convert_to_b2(16, G), GF::convert_to_b2(0, G));
	vertices[1].Set(GF::convert_to_b2(0, G), GF::convert_to_b2(10, G));
	vertices[2].Set(GF::convert_to_b2(65, G), GF::convert_to_b2(79, G));
	
	//vertices[7].Set(GF::convert_to_b2(57, G), GF::convert_to_b2(261, G));
	for (unsigned int i = 0; i < 3; i++) {
		vertices[i].Set(vertices[i].x, vertices[i].y);
	}
	poly_shape.Set(vertices, 3);
	fd.shape = &poly_shape;
	fd.density = 1.0f;
	body->CreateFixture(&fd);
	body->SetUserData(this);
	w_ptr->addChild(this);

}

void Jaw::close() {
	body->ApplyTorque(-jaw_strength, true); //-15

}

b2Body* Jaw::getBody() {
	return body;
}

void Jaw::destroy(void* world_ptr) {
	World* w_ptr = static_cast<World*>(world_ptr);
	w_ptr->getb2World()->DestroyBody(body);
}

Eye_Ball::Eye_Ball(const Vector2 &position) {
	setResAnim(res::resources.getResAnim("Bird_Eye"));
	setPosition(position);
	setAnchor(0.5f, 0.5f);
	addTween(Actor::TweenRotation(MATH_PI * 2), eye_ball_rotation_speed, -1, false, 0, Tween::ease_inExpo);
}

void Main_Body::doUpdate(const UpdateState &us) {
	ticks += us.dt;
	if (ticks >= max_ticks) {
		jaw->close();
		ticks = max_ticks / random2::randomrange(0, 4);
	}
}

void Main_Body::destroy(void* world_ptr) {
	World* w_ptr = static_cast<World*>(world_ptr);
	jaw->destroy(world_ptr);
	jaw->detach();
	w_ptr->getb2World()->DestroyBody(body);
	eb->detach();
}

Main_Body::Main_Body(const Vector2 &position, void* world_ptr) : max_ticks(3) {

	World* w_ptr = static_cast<World*>(world_ptr);
	setResAnim(res::resources.getResAnim("Bird_Body"));
	setPosition(position);
	setName("NOTHING");
	//Creating the body.
	b2BodyDef bd;
	bd.type = b2_staticBody;
	bd.position = GF::convert(position);
	body = w_ptr->getb2World()->CreateBody(&bd);
	//Creating fixtures for the body.-=-=-=-=-=-=-=-=-

	//Body
	b2PolygonShape poly_shape;
	b2FixtureDef fd;
	b2Vec2 vertices[7];
	float G = GF::get_G_Scale();
	vertices[0].Set(GF::convert_to_b2(221, G), GF::convert_to_b2(83, G));
	vertices[1].Set(GF::convert_to_b2(178, G), GF::convert_to_b2(26, G));
	vertices[2].Set(GF::convert_to_b2(73, G), GF::convert_to_b2(64, G));
	vertices[3].Set(GF::convert_to_b2(0, G), GF::convert_to_b2(108, G));
	vertices[4].Set(GF::convert_to_b2(0, G), GF::convert_to_b2(286, G));
	vertices[5].Set(GF::convert_to_b2(57, G), GF::convert_to_b2(261, G));
	vertices[6].Set(GF::convert_to_b2(194, G), GF::convert_to_b2(136, G));
	//vertices[7].Set(GF::convert_to_b2(57, G), GF::convert_to_b2(261, G));
	for (unsigned int i = 0; i < 7; i++) {
		vertices[i].Set(vertices[i].x, vertices[i].y);
	}
	poly_shape.Set(vertices, 6);
	fd.shape = &poly_shape;
	
	//Top Beak
	b2PolygonShape poly_shape2;
	b2FixtureDef fd2;
	b2Vec2 vertices2[3];
	
	vertices2[0].Set(GF::convert_to_b2(0, G), GF::convert_to_b2(0, G));
	vertices2[1].Set(GF::convert_to_b2(-16, G), GF::convert_to_b2(20, G));
	vertices2[2].Set(GF::convert_to_b2(78, G), GF::convert_to_b2(20, G));
	//vertices[3].Set(GF::convert_to_b2(0, G), GF::convert_to_b2(108, G));
	//vertices[4].Set(GF::convert_to_b2(0, G), GF::convert_to_b2(286, G));
	//vertices[5].Set(GF::convert_to_b2(57, G), GF::convert_to_b2(261, G));
	//vertices[6].Set(GF::convert_to_b2(194, G), GF::convert_to_b2(136, G));
	//vertices[7].Set(GF::convert_to_b2(57, G), GF::convert_to_b2(261, G));
	b2Vec2 xy_add = GF::convert(Vector2(getWidth() / 2.3f, getHeight() / 3.7f));
	for (unsigned int i = 0; i < 3; i++) {
		vertices2[i].Set(vertices2[i].x + xy_add.x, vertices2[i].y + xy_add.y);
	}
	
	poly_shape2.Set(vertices2, 3);
	fd2.shape = &poly_shape2;

	body->CreateFixture(&fd);
	body->CreateFixture(&fd2);
	body->SetUserData(this);
	//This is creating the eyeball on the bird.
	eb = new Eye_Ball(Vector2(getWidth() / 2.8, getHeight() / 6));
	addChild(eb);
	jaw = new Jaw(position + Vector2(100, 100), world_ptr);
	jaw->setPriority(-1000);

	
	b2RevoluteJointDef rjd;
	rjd.bodyA = body;
	rjd.bodyB = jaw->getBody();
	//rjd.enableMotor = true;
	rjd.localAnchorA = GF::convert(Vector2(getWidth() / 2.5f, getHeight() / 2.7f));
	rjd.enableLimit = true;
	rjd.lowerAngle = -MATH_PI / 3.3;
	rjd.upperAngle = 1;
	//rjd.localAnchorA = GF::convert(Vector2(, 0));
	//rjd.localAnchorB = GF::convert(Vector2(0, 0));
	rjd.collideConnected = false;
	w_ptr->getb2World()->CreateJoint(&rjd);

	w_ptr->addChild(this);
}


Final_Bird::Final_Bird(const Vector2 &position, void* world_ptr) : _world_ptr(world_ptr) {
	setPosition(position);
	mb = new Main_Body(position, world_ptr);
	ts = new T_Sign(Vector2(mb->getWidth() / 1.8, -170));
	//ts = new T_Sign(Vector2(0,0));
	static_cast<World*>(world_ptr)->addChild(this);
	mb->addChild(ts);
}

void Final_Bird::onRemovedFromStage() {
	mb->destroy(_world_ptr);
	ts->detach();
	mb->detach();
}