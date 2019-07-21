#pragma once
#include "oxygine-framework.h"
#include "Entity.h"
#include "S_Visible_M3.h"
#include "K_Visible_M3.h"
using namespace oxygine;

DECLARE_SMART(BackPlate, spBackPlate);
class BackPlate : public Sprite {
private:
	b2Body* _rel_body;
	void doUpdate(const UpdateState &us);
	const Vector2 init_pos;
	//This is the actor accosiated with the body.
	Actor* actor;
public:
	void setRel(b2Body *rel_body);
	BackPlate(const Vector2 &position);
};
/*This class is where all the T_Blocks and T_Block_ks should put attach to. Allowing all the blocks to be faded.*/
DECLARE_SMART(Block_Fader, spBlock_Fader);
class Block_Fader : public Actor {
private:
public:
	Block_Fader(World* world_ptr);
};
DECLARE_SMART(T_Block, spT_Block);
class T_Block : public S_Visible_M3, public Box9Sprite {
private:
	void doUpdate(const UpdateState &us);
public:
	//This will create a fixture relative to the body given.
	T_Block(const Vector2 &rel_pos, Vector2 pos, unsigned int direction, int length_in_blocks);
	//This will get the world pointer.
	void T_Block::onRemovedFromStage() {
		world_ptr->getb2World()->DestroyBody(body);
	}
	//This will return a pointer to the body of the block.
	b2Body* getBody() { return body; };
};
DECLARE_SMART(T_Block_K, spT_Block_K);
class T_Block_K : public K_Visible_M3, public Box9Sprite {
private:
	//This will set the direction of the block depending on what you give it.
	void set_direction(unsigned int new_direction);
	//This is the position whereby the T_Block_K needed to get to.
	//Once the block has reached its boundary, then it will be transformed it this position.
	b2Vec2 destination;
	//This is the boundary - the area where the platform needs to go back.
	float _boundary;
	//This is the initial direction that was inputted when you created the object.
	unsigned int _init_direction;
	//This is the direction that the object is moving.
	unsigned int _direction;
	//This is the speed at which the platform moves.
	float _speed;
	//This is the position that the T_Block_K should move to.
	float _distance;
	//This is the starting position.
	Vector2 start_pos;
	//This is called every frame.
	void doUpdate(const UpdateState &us);
public:
	//This will create a fixture relative to the body given.
	T_Block_K(const Vector2 &rel_pos, Vector2 pos, unsigned int direction, int length_in_blocks, float speed, float distance);
	//This will get the world pointer.
	void T_Block_K::onRemovedFromStage() {
		world_ptr->getb2World()->DestroyBody(body);
	}
};
DECLARE_SMART(Temple, spTemple);
class Temple : public Entity {
private:
	//This is the index of the random data.
	static unsigned int Temple::current_index;
	//This will tick the current index up one.
	void tick_ci();
	//This is data that is used when picking what temple to create.
	struct r_data {
	private:
		unsigned int _level = 0;
		unsigned int _world = 1;
		//This is used for marking the data as a place holder.
		bool _is_dummy = false;
	public:

		

		r_data() {

		}

		r_data(bool is_dummy) : _is_dummy(is_dummy) {
			
		}

		r_data(unsigned int level, unsigned int world) : _level(level), _world(world) {

		}

		bool operator==(r_data ob1) {
			if (_level == ob1._level && _world == ob1._world) {
				return true;
			}
			else {
				return false;
			}
		}

		void operator=(r_data ob1) {
			_level = ob1._level;
			_world = ob1._world;
			_is_dummy = ob1._is_dummy;

		}
		//This will get the _level data member(FOR READING PURPOSES ONLY).
		unsigned int getLevel() {
			return _level;
		}
		//This will get the _world data member(FOR READING PURPOSES ONLY).
		unsigned int getWorld() {
			return _world;
		}
		//This will tell you if the object is a dummy(place holder) or not.
		bool is_dummy() {
			return _is_dummy;
		}
		
	};

	static std::vector<r_data> Temple::_random_data;
	
	//This is the block fader.
	spBlock_Fader bf;
	//This is the width of the temple in blocks.
	float sd_w_blocks;
	//This is the width of the temple in pixels.
	float sd_w_pixels;
	//This is the width of the temple in blocks.
	float sd_h_blocks;
	//This is the width of the temple in pixels.
	float sd_h_pixels;
	//This will create a single wall.
	void create_wall(const Vector2 &rel_pos, Vector2 pos, unsigned int direction, int length_in_blocks, World* world_ptr);
	//This will create a single spike.
	void create_spike(const Vector2 &rel_pos, Vector2 pos, unsigned int direction, int length_in_blocks, bool is_flipped, World* world_ptr);
	//This creates a spinning obstacle.
	void create_ST(const Vector2 &rel_pos, Vector2 pos, unsigned int type, World* world_ptr);
	//This creates a force field.
	void create_ForceField(const Vector2 &rel_pos, RectF box, int direction, float force, World* world_ptr, unsigned int fadeTime);
	//This creates a moveable T_Block.
	void create_wall_K(const Vector2 &rel_pos, Vector2 pos, unsigned int direction, int length_in_blocks, float speed, float distance, World* world_ptr);
	//This is the background behind the temple.
	spBackPlate bg;
	//This contains all the T_Blocks.
	std::vector<spActor> walls;
	//This will create all the walls, according to the type given.
	void create_structures(unsigned int type, World* world_ptr, const Vector2 &rel_pos, unsigned int fadeTime);
	//This will fill the _random_data vector full of random temple creation information.
	static void fill_random();
	//This will check inside of the _random_data vector to see if the one given it already inside of the vector.
	static bool check_data(r_data data_to_check);
	//This will check to see if the index and object supplied are equal to the object at the specified index.
	static bool check_data(r_data data_to_check, unsigned int index);
	//This will calculate all the different possibilities the temples can come out as.
	static void calculate_possibilities(std::vector<r_data> *v_ptr);
public:

	//This will clear the cache of previously created temples.
	//This should be called when you reseted the world or something of the sort.
	static void reset_cache();
	Temple(float y_coordinate, World* world_ptr, unsigned int type, unsigned int fadeTime = 0);
	void destroy();
	//This is called when the actor is removed from the stage(called detach()).
	void onRemovedFromStage();
	//This will fade the temple out.
	//This is an overwritten virtual function.
	void fadeOut(unsigned int time = 0);

};