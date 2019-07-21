#pragma once
#include "oxygine-framework.h"
#include "Filters.h"
#include "D_Visible_M3.h"
#include "K_Visible_M3.h"
#include "World.h"
#include "GF.h"
#include "Entity.h"
using namespace oxygine;
DECLARE_SMART(Block, spBlock);
class Block : public D_Visible_M3, public ColorRectSprite {
private:

public:
	//This is a square that can be placed on the ST.
	Block(RectF rectangle, const Color &color, float rotation);
	
	void onRemovedFromStage();
	//This will get the body.
	b2Body* getBody() {
		return body;
	}
};

DECLARE_SMART(BW, spBW);
class BW : public Actor, public K_Visible_M3, public Filters {
private:
	spPolygon poly;
	//This initializes all the vertices.
	static vertexPCT2 initVertex(const Vector2& pos, unsigned int color);
	//This will return the vertex
	static vertexPCT2 getVertex(int i, int num, unsigned int color);
	//This will actually create the vertices.
	static vertexPCT2* BW::createVertices(int num, unsigned int color);
public:
	//This is the back whirly thing.
	BW(const Vector2 &pos, float radius, float spin_speed);
	//This is called when it is removed from the stage.
	void onRemovedFromStage();
	//This is called every frame.
	void doUpdate(const UpdateState& us);
	void update(int num);
	//This will get the body of the Backwheel.
	b2Body *getBody() {
		return body;
	}
	//This will get the world pointer.
	World* getWorld_ptr() {
		return world_ptr.get();
	}
};
DECLARE_SMART(ST, spST);
class ST : public Entity {
private:
	//This will create all the blocks and parts.
	void create_parts(const Vector2 &rel_pos, const Vector2 &position, unsigned int type);
	//This contains all the parts that you attached to the backwheel.
	std::vector<spActor> _parts;
	//This is the backwheel.
	spBW backwheel;

public:
	//This is a Spinning Thing. It will spin a circle and will have blocks attached to it.
	ST(const Vector2 &rel_pos, const Vector2 &pos, unsigned int type);
	void onRemovedFromStage();
	
};