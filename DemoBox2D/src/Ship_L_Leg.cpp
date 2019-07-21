#include "Ship_L_Leg.h"
#include "ImageRes.h"

Ship_L_Leg::Ship_L_Leg() {
	
}

Ship_L_Leg::Ship_L_Leg(Vector2 pos, int Playerindex, std::vector<b2Joint *> &joints) {
	ship_joints = &joints;
	playerIndex = Playerindex;
	setup("Ship_Left_Leg", pos, SHIP_PART, DEFAULT_MASK, e_l_leg);
}

void Ship_L_Leg::onRemovedFromStage() {
	if (!_has_been_deleted) {
		s_emitter->detach();
		c_sound.destroy();
		delete_joints(*ship_joints);
		_world_ptr->getb2World()->DestroyBody(body);
		_has_been_deleted = true;
	}
}
