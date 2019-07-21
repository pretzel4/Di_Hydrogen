#pragma once
#include "oxygine-framework.h"
#include "Box2D\Box2D.h"
using namespace oxygine;

class Contact_Listener : public b2ContactListener {
	//Virtual function called when any contact has been made with any part.
	void BeginContact(b2Contact* contact);
	//Virtual function called when any contact is done being made with any part.
	void EndContact(b2Contact* contact);
};
