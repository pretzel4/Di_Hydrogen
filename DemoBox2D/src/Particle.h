#pragma once
#include "oxygine-framework.h"
#include "Box2D\Box2D.h"
#include "World.h"
#include "random2.h"
#include "Filters.h"
using namespace oxygine;
DECLARE_SMART(Particle, spParticle);
class Particle : public ColorRectSprite, public Filters{
private:
	//~Particle();
	//This tells if the particle has been deleted or not.
	bool has_been_destroyed = false;
	bool _is_light = true;
	float a_tick = 0;
	float max_a_tick = 0;
	Vector2 init_size;
	//This will add a fixture to the particle.
	void addFixture(const Vector2 &size, int16 groupindex, float32 friction, float32 restitution, float32 density);
	//This will modify the positional argument you have given, and give it a random variation.
	void modify_pos(b2Vec2 &pos, float how_much);
	//This is a static vector that holds onto a bunch of random integers.
	static random2::RandomStorage_int Particle::rand_store;

	//This is a static vector that holds onto a bunch of random 1s and 0s(booleans).
	static random2::RandomStorage_bool Particle::rand_booleans;
	//This is the type of particle it is.
	int _type;

	//This is a body that is used for watching where the particles should go.
	b2Body *body;
	
	//This will clean up the particle.
	void destroy();
	
	//This is a pointer to the world.
	static World* Particle::_w_ptr;
	//This contains pointers to all the particles that require a lot of CPU.
	static std::vector<spParticle> Particle::_heavy_objects;
	//This contains pointers to all the particles that don't require a lot of CPU.
	static std::vector<spParticle> Particle::_light_objects;

	//This contains all the objects that need to be deleted next frame.
	static std::vector<spParticle> Particle::_to_delete;
	//This is called every frame.
	void doUpdate(const UpdateState &us);

	//This is called when the tween is done.
	void when_t_done(Event *ev);
	//This will setup everything.
	void create(b2Vec2 pos, b2Vec2 direction, int type);

	//This will loop through the _objects class and erase(NOT DELETE) the object within.
	static void find_erase(spParticle ptr_to_erase);

public:
	//This will create a single particle in the direction given.
	//It will set its own parent(the world_ptr).
	Particle(const b2Vec2 &pos, const b2Vec2 &direction, int type);
	//This will create a single particle in the direction given.
	//It will set its own parent(the world_ptr).
	Particle(const Vector2 &pos, const Vector2 &direction, int type);
	//This will check to see if any particles need to be deleted.
	static void tick();
	//This initiates the class.
	static void init(World* world_ptr);
	//This will clear the world of all particles.
	static void clear();
};