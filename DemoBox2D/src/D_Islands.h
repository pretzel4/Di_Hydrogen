#pragma once
#include "oxygine-framework.h"
#include "random2.h"
#include "ImageRes.h"
using namespace oxygine;

	



DECLARE_SMART(Island, spIsland);
class Island : public Sprite {
private:
	//This will get a sprite according to the number given.
	const ResAnim* getSpri(unsigned int index) {
		switch (index) {
		case 0:
			return res::resources.getResAnim("Rock_Decor0");
			break;
		case 1:
			return res::resources.getResAnim("Rock_Decor1");
			break;
		case 2:
			return res::resources.getResAnim("Rock_Decor2");
		}
	}
public:
	//This will convert the position and size of the island and give you a rectangle.

	//This will create the island.
	Island(const Vector2 &position, unsigned int sprite_index);
};
DECLARE_SMART(D_Islands, spD_Islands);
class D_Islands : public Actor {
private:
	//This will get a sprite according to the number given.
	const ResAnim* getSpri(unsigned int index) {
		switch (index) {
		case 0:
			return res::resources.getResAnim("Rock_Decor0");
			break;
		case 1:
			return res::resources.getResAnim("Rock_Decor1");
			break;
		case 2:
			return res::resources.getResAnim("Rock_Decor2");
		}
	}
	//This will create the island and put the object into the _objects vector.
	inline void create_island(const Vector2 &position, unsigned int index);
	//This contains all the islands pertaining to this object.
	std::vector<spIsland> _objects;
	//This should point to the world.
	void* _world_ptr;
public:
	//This is the position that you created the object at.
	const Vector2 start_pos;

	D_Islands(const Vector2 &position, void* world_ptr);
	//This will make the islands randomize.
	void setup(bool should_fade);
	//This will make the islands fade in.
	void fadeIn();
	//This will make the islands fade out.
	void fadeOut();
	//This will tell you if the islands are active(has a alpha greater than 0).
	bool get_is_active();
	//This will deallocate the island's memory.
	void deallocate();
};