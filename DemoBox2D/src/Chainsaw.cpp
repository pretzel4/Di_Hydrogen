#include "Chainsaw.h"
#include "Platform.h"

Circle::Circle(const Vector2 &position, float radius) {
	setResAnim(res::resources.getResAnim("Chainsaw_Circle"));
	setPosition(position);
	radius = (radius * 16) + circle_extention; //Because the circle in the sprite is a little smaller then the actual sprite rect.

	setAnchor(0.5f, 0.5f);
	setSize(Vector2(radius, radius));
}

Ball::Ball(const Vector2 &axis_point, float rot_speed) {
	setName("Pinwheel");
	setResAnim(res::resources.getResAnim("Pinwheel"));
	setup(this, RectF(Vector2(axis_point.x, axis_point.y), getSize()));
	b2FixtureDef fd;
	fd.filter.categoryBits = CHAINSAW;
	fd.filter.maskBits = SHIP_PART | PARTICLE;
	//fd.filter.groupIndex = ;
	fd.density = 1.0f;
	setup_body(RectF(axis_point, getSize() - Vector2(7, 7)), NULL, NULL, &fd);
	body->SetUserData(this);
}

void Stick::create_joint() {
	b2RevoluteJointDef jd;
	jd.collideConnected = false;
	jd.type = e_weldJoint;

	Vector4 jd_joints(0, getSize().y / 2, 0, 0);
	jd.localAnchorA = b2Vec2(GF::convert(Vector2(jd_joints[0], jd_joints[1]), GF::get_G_Scale()));
	jd.localAnchorB = b2Vec2(GF::convert(Vector2(jd_joints[2], jd_joints[3]), GF::get_G_Scale()));
	jd.bodyA = body;
	jd.bodyB = pin->getBody();
	b2RevoluteJoint *wj = (b2RevoluteJoint*)world_ptr->getb2World()->CreateJoint(&jd);
	rj = wj;


	//pin2
	b2RevoluteJointDef jd2;
	jd2.collideConnected = false;
	jd2.type = e_weldJoint;

	Vector4 jd2_joints(0, -getSize().y / 2, 0, 0);
	jd2.localAnchorA = b2Vec2(GF::convert(Vector2(jd2_joints[0], jd2_joints[1]), GF::get_G_Scale()));
	jd2.localAnchorB = b2Vec2(GF::convert(Vector2(jd2_joints[2], jd2_joints[3]), GF::get_G_Scale()));
	jd2.bodyA = body;
	jd2.bodyB = pin2->getBody();
	b2RevoluteJoint *wj2 = (b2RevoluteJoint*)world_ptr->getb2World()->CreateJoint(&jd2);
	rj2 = wj2;



}

void Stick::doUpdate(const UpdateState &us) {
	if (circ->getPosition() != GF::convert(body->GetPosition())) {
		GF::println_va(circ->getPosition().x);
		GF::println_va(circ->getPosition().y);
		circ->setPosition(GF::convert(body->GetPosition()));
		//throw("Error");
	}
}

Stick::Stick(const Vector2 &axis_point, float dist_in_16s, float rot_speed, unsigned int fadeTime) {

	setResAnim(res::resources.getResAnim("Pin_stick"));
	setup(this, RectF(axis_point - (Vector2(8 / 2, (16 * dist_in_16s) / 2)), Vector2(8, 16 * dist_in_16s)));
	setAnchor(0.5f, 0.5f);
	b2FixtureDef fd;
	fd.filter.categoryBits = CHAINSAW;
	fd.filter.maskBits = SHIP_PART | PARTICLE;
	setup_body(RectF(axis_point, Vector2(8, 16 * dist_in_16s)), NULL, NULL, &fd);


	setVerticalMode(Box9Sprite::StretchMode::TILING_FULL);
	setGuides(0, 0, 0, 0);


	body->SetAngularVelocity(rot_speed);
	circ = new Circle(axis_point, dist_in_16s);
	GF::fadeIn(circ.get(), fadeTime);
	world_ptr->addChild(circ);
	world_ptr->insert_shifter(circ.get());
	circ->setPriority(5000);
	//Vector2(axis_point.x - (getWidth() / 2), axis_point.y - (dist_in_16s * 16)
	pin = new Ball(Vector2(axis_point.x, axis_point.y + ((dist_in_16s * 16) / 2)), pin_turn_speed);
	pin2 = new Ball(Vector2(axis_point.x, axis_point.y - ((dist_in_16s * 16) / 2)), pin_turn_speed);
	GF::fadeIn(pin.get(), fadeTime);
	GF::fadeIn(pin2.get(), fadeTime);
	GF::fadeIn(this, fadeTime);
	world_ptr->addChild(pin);
	world_ptr->addChild(pin2);
	world_ptr->addChild(this);
	create_joint();
	body->SetUserData(this);

}

Chainsaw::Chainsaw(const Vector2 &pos, float rot_speed, float length, unsigned int fadeTime) {
	
	setPosition(pos);
	pole = new Stick(pos, length, rot_speed, fadeTime);
	
	
	
}



void Chainsaw::destroy() {
	has_called_destroy = true;
	pole->destroy();
	//pole.~intrusive_ptr();
	//detach();
}

void Chainsaw::onRemovedFromStage() {
	destroy();
}

void Chainsaw::fadeOut(unsigned int time) {
	if (!has_called_destroy) {
		pole->fadeOut(time);
	}
	else {
		throw("HAS_CALLED_DESTROY_ERROR1265");
	}
	
}