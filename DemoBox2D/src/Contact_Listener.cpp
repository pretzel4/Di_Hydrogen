#include "Contact_Listener.h"
#include "Ship_Part_M.h"
#include "Engine_M.h"
#include "Platform.h"
#include "Col.h"
#include "Ship_Engine.h"
#include "Black_hole.h"
#include "ForceField.h"
#include "TPIN.h"
void Contact_Listener::BeginContact(b2Contact* contact) {
	b2Body* bodyA = (b2Body*)contact->GetFixtureA()->GetBody();
	b2Body* bodyB = (b2Body*)contact->GetFixtureB()->GetBody();
	Actor *bodyAUserData = (Actor*)contact->GetFixtureA()->GetBody()->GetUserData();
	Actor *bodyBUserData = (Actor*)contact->GetFixtureB()->GetBody()->GetUserData();

	
	//Checking TPIN sensors.
	if (bodyAUserData->getName() == "TPIN" && (bodyBUserData->getName() == "PM" || bodyBUserData->getName() == "EM")) {
		static_cast<TPIN*>(bodyAUserData)->when_entering(static_cast<Ship_Part_M*>(bodyBUserData));
	}
	else if (bodyBUserData->getName() == "TPIN" && (bodyAUserData->getName() == "PM" || bodyAUserData->getName() == "EM")) {
		static_cast<TPIN*>(bodyBUserData)->when_entering(static_cast<Ship_Part_M*>(bodyAUserData));
	}
	

	//Checking for ship crash collision.
	if (contact->GetFixtureB()->IsSensor() == false && (bodyAUserData->getName() == "PM" || bodyAUserData->getName() == "EM")) {
		static_cast<Ship_Part_M*>(bodyA->GetUserData())->if_contact(contact, static_cast<Ship_Part_M*>(bodyA->GetUserData()), bodyBUserData);
	}
	else if (contact->GetFixtureA()->IsSensor() == false && (bodyBUserData->getName() == "PM" || bodyBUserData->getName() == "EM")){
		static_cast<Ship_Part_M*>(bodyB->GetUserData())->if_contact(contact, static_cast<Ship_Part_M*>(bodyB->GetUserData()), bodyAUserData);
	}
	
	//Checking for black_hole sensor collision.
	if (bodyAUserData->getName() == "BHS") {
		static_cast<Field*>(bodyAUserData)->if_entered(bodyB, bodyBUserData);
			
	}
	else if (bodyBUserData->getName() == "BHS") {
		static_cast<Field*>(bodyBUserData)->if_entered(bodyA, bodyAUserData);
	}

	//Checking for ForceField sensor collision.
	if (bodyAUserData->getName() == "ForceField") {
		static_cast<ForceField*>(bodyAUserData)->if_entered(bodyB, bodyBUserData);

	}
	else if (bodyBUserData->getName() == "ForceField") {
		static_cast<ForceField*>(bodyBUserData)->if_entered(bodyA, bodyAUserData);
	}

	//Checking for black_hole vertex collision.
	if (bodyAUserData->getName() == "BHV") {
		static_cast<Vertex*>(bodyAUserData)->if_contact(bodyB, bodyBUserData);
	}
	else if (bodyBUserData->getName() == "BHV") {
		static_cast<Vertex*>(bodyBUserData)->if_contact(bodyA, bodyAUserData);
	}
	
	//Checking for landing collision.
	if (bodyAUserData->getName() == "Pltfrm" && bodyBUserData->getName() == "EM") {
		static_cast<Platform*>(bodyAUserData)->check_start(bodyB);
	}
	else if (bodyAUserData->getName() == "EM" && bodyBUserData->getName() == "Pltfrm") {
		static_cast<Platform*>(bodyBUserData)->check_start(bodyA);
	}
	
	//Checking for spike collision.
	if (bodyAUserData->getName() == "Spikes" && bodyBUserData->getName() == "EM") {
		static_cast<Spike*>(bodyAUserData)->if_contact(static_cast<Ship_Part_M*>(bodyBUserData), bodyAUserData);
	}
	else if (bodyAUserData->getName() == "EM" && bodyBUserData->getName() == "Spikes") {
		static_cast<Spike*>(bodyBUserData)->if_contact(static_cast<Ship_Part_M*>(bodyAUserData), bodyBUserData);
	}
	else if (bodyAUserData->getName() == "Spikes" && bodyBUserData->getName() == "PM") {
		static_cast<Spike*>(bodyAUserData)->if_contact(static_cast<Ship_Part_M*>(bodyBUserData), bodyAUserData);
	}
	else if (bodyAUserData->getName() == "PM" && bodyBUserData->getName() == "Spikes") {
		static_cast<Spike*>(bodyBUserData)->if_contact(static_cast<Ship_Part_M*>(bodyAUserData), bodyBUserData);
	}
	






}

void Contact_Listener::EndContact(b2Contact* contact) {
	b2Body* bodyA = (b2Body*)contact->GetFixtureA()->GetBody();
	b2Body* bodyB = (b2Body*)contact->GetFixtureB()->GetBody();
	Actor *bodyAUserData = (Actor*)contact->GetFixtureA()->GetBody()->GetUserData();
	Actor *bodyBUserData = (Actor*)contact->GetFixtureB()->GetBody()->GetUserData();
	//GF::println_va(bodyAUserData->ColType);
	
	//Checking TPIN sensors.
	if (bodyAUserData->getName() == "TPIN" && (bodyBUserData->getName() == "PM" || bodyBUserData->getName() == "EM")) {
		static_cast<TPIN*>(bodyAUserData)->when_leaving(static_cast<Ship_Part_M*>(bodyBUserData));
	}
	else if (bodyBUserData->getName() == "TPIN" && (bodyAUserData->getName() == "PM" || bodyAUserData->getName() == "EM")) {
		static_cast<TPIN*>(bodyBUserData)->when_leaving(static_cast<Ship_Part_M*>(bodyAUserData));
	}


	if (contact->GetFixtureB()->IsSensor() == false && (bodyAUserData->getName() == "PM" || bodyAUserData->getName() == "EM")) {
		static_cast<Ship_Part_M*>(bodyAUserData)->end_contact(static_cast<Ship_Part_M*>(bodyA->GetUserData()));
	}
	else if (contact->GetFixtureA()->IsSensor() == false && (bodyBUserData->getName() == "PM" || bodyBUserData->getName() == "EM")) {
		static_cast<Ship_Part_M*>(bodyBUserData)->end_contact(static_cast<Ship_Part_M*>(bodyB->GetUserData()));
	}

	if (bodyAUserData->getName() == "Pltfrm" && bodyBUserData->getName() == "EM") {
		static_cast<Platform*>(bodyAUserData)->check_end(bodyB);
	}
	else if (bodyAUserData->getName() == "EM" && bodyBUserData->getName() == "Pltfrm") {
		static_cast<Platform*>(bodyBUserData)->check_end(bodyA);
	}

	//Checking for black_hole sensor collision.
	if (bodyAUserData->getName() == "BHS") {
		static_cast<Field*>(bodyAUserData)->if_exited(bodyB);

	}
	else if (bodyBUserData->getName() == "BHS") {
		static_cast<Field*>(bodyBUserData)->if_exited(bodyA);
	}

	//Checking for black_hole sensor collision.
	if (bodyAUserData->getName() == "ForceField") {
		static_cast<ForceField*>(bodyAUserData)->if_left(bodyB);

	}
	else if (bodyBUserData->getName() == "ForceField") {
		static_cast<ForceField*>(bodyBUserData)->if_left(bodyA);
	}
}