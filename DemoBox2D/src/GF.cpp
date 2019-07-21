#include "GF.h"
#include "World.h"
#define GlobalScale 60.0f
#define pi 3.14
Vector2 GF::convert(const b2Vec2& pos, float SCALE) {
	return Vector2(pos.x*SCALE, pos.y*SCALE);
}

b2Vec2 GF::convert(const Vector2& pos, float SCALE) {
	return b2Vec2(pos.x / SCALE, pos.y / SCALE);
}

float GF::convert_to_Oxy(float b2, float SCALE) {
	return b2 * SCALE;
}

float GF::convert_to_b2(float Oxy, float SCALE) {
	return Oxy / SCALE;
}

//This returns a global scale.
float GF::get_G_Scale() {
	return GlobalScale;
}

float GF::rad(float degree) {
	return degree * (pi / 180);
}

float GF::degrees(float radians) {
	return radians * (180 / pi);
}

void GF::println_va(int var) {
	logs::messageln_va(std::to_string(var).c_str(), "5");
}

void GF::print_va(int var) {
	logs::message_va(std::to_string(var).c_str(), "5");
}

int GF::StringtoInt(std::string string) {
	return std::stoi(string);
}

float GF::StringtoFloat(std::string string) {
	return std::stof(string);
}

unsigned int GF::StringtoUnint(std::string string) {
	return std::stoul(string);
}

float GF::getDistance(const Vector2 &position1, const Vector2 &position2) {
	float d = sqrt((pow((position1.x - position2.x), 2) + pow((position1.y - position2.y), 2)));
	return d;
}

bool GF::does_exist(int index, int size_of_vector) {
	if (index >= 0 && index <= size_of_vector - 1) {
		return true;
	}
	else {
		return false;
	}
}

float GF::average(float num1, float num2) {
	return (num1 + num2) / 2;
}

void GF::populate4(unsigned int ar[4], unsigned int r, unsigned int b, unsigned int g, unsigned int alpha) {
	ar[0] = r;
	ar[1] = b;
	ar[2] = g;
	ar[3] = alpha;
}

std::string GF::convert_to_string(int number_to_convert) {
	return std::to_string(number_to_convert);
}

void GF::flip(std::vector<int> &vec) {
	std::reverse(vec.begin(), vec.end());
}

b2Joint *GF::weld(b2JointDef *jd, void* world_ptr) {
	World* w_ptr = static_cast<World*>(world_ptr);
	return w_ptr->getb2World()->CreateJoint(jd);
}

void GF::inflict(spActor actor, Tween* tween) {
	//if (tween != NULL) {
	//	actor->addTween(actor);
	//}
}

void GF::fadeIn(Actor* actor, unsigned int time, unsigned int max) {
	if (time != 0) {
		actor->setAlpha(0);
		actor->addTween(Actor::TweenAlpha(max), time);
	}
}

void GF::fadeOut(Actor* actor, unsigned int time, unsigned int min) {
	if (time != 0) {
		actor->addTween(Actor::TweenAlpha(min), time);
	}
}

float GF::pyagor_hypot(float leg1, float leg2) {
	return sqrt(powf(leg1, 2) + powf(leg2, 2));
}