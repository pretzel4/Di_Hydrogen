#include "Particle.h"
#include "GF.h"
#include "random2.h"
#include "Filters.h"
#define part_0sz Vector2(8,8)
#define part_1sz Vector2(10,15)
#define part_2sz Vector2(14,14);
#define part_3sz Vector2(12,12);
#define part_decay b2Vec2(1,1)
#define rand_store_size 50 //The random storage vector will contain 50 numbers. The bigger the number, the more random it is, however, it will take more memory.
#define rand_low 0
#define rand_high 50
#define MAX_NUM_HEAVY 30
#define MAX_NUM_LIGHT 50
std::vector<spParticle> Particle::_heavy_objects;
std::vector<spParticle> Particle::_light_objects;
std::vector<spParticle> Particle::_to_delete;
random2::RandomStorage_int Particle::rand_store;
random2::RandomStorage_bool Particle::rand_booleans;
World* Particle::_w_ptr;


void Particle::create(b2Vec2 pos, b2Vec2 direction, int type) {
	//if (_objects.size >= 5) {

	_type = type;
	
	setAnchor(0.5f, 0.5f);
	//This determines if the particle is a light or heavy particle.
	//If it is not light, then it is heavy.
	_is_light = true;
	Vector2 Size;
	Color c;
	unsigned int Alpha_Time;
	unsigned int Color_Subtraction;
	unsigned int size_sub;
	switch (type) {
	case 0: //Collision sparks -Yellow
			//Creating variables that used random numbers to determine their value.
		Alpha_Time = rand_store.getNum() * 8 + 1000;
		Color_Subtraction = rand_store.getNum() * 4;
		size_sub = (rand_store.getNum() / 4);
		Size = part_0sz - Vector2(size_sub, size_sub);
		c = Color(255 - Color_Subtraction, 255 - Color_Subtraction, 0, 255);

		//Adding positional variation, so that they don't all spawn at the same position.



		//Creating the tween that allows for the fading of the particle.
		//Once the tween is done, the object will put itself into the _to_delete vector.

		break;
	case 1: //Confetti -Many colors
			//Creating variables that use random numbers to determine their value.
		Alpha_Time = rand_store.getNum() * 8 + 1000;
		Color_Subtraction = rand_store.getNum() * 4;
		size_sub = (rand_store.getNum() / 4);
		Size = part_0sz - Vector2(size_sub, size_sub);

		//Changing the magnitude of the direction.
		direction.x *= rand_store.getNum();
		direction.y *= rand_store.getNum();
		//Setting a random color to the confetti.
		switch (random2::randomrange(0, 2)) {
		case 0:
			c = Color(255 - Color_Subtraction, 0, 0, 255);
			break;
		case 1:
			c = Color(0, 255 - Color_Subtraction, 0, 255);
			break;
		case 2:
			c = Color(0, 0, 255 - Color_Subtraction, 255);
		}


		//Adding positional variation, so that they don't all spawn at the same position.

		modify_pos(pos, (Size.x * Size.y) / 70);



		break;
	case 2: //Engine Exhaust -FULL
			//Creating variables that used random numbers to determine their value.
		Alpha_Time = rand_store.getNum() * 8 + 400;
		Color_Subtraction = rand_store.getNum() * 4;
		size_sub = (rand_store.getNum() / 4);
		Size = part_2sz - Vector2(size_sub, size_sub);
		switch (rand_booleans.getNum()) {
		case 0:
			c = Color(255 - Color_Subtraction, 255 - Color_Subtraction, 0, 255);
			break;
		case 1:
			c = Color(255 - Color_Subtraction, 0, 0, 255);
		}
		_is_light = false;

		break;
	case 3: //Engine Exhaust -RUNNING OUT
			//Creating variables that used random numbers to determine their value.
		Alpha_Time = rand_store.getNum() * 8 + 1000;
		Color_Subtraction = rand_store.getNum() * 4;
		size_sub = (rand_store.getNum() / 4);
		Size = part_3sz - Vector2(size_sub, size_sub);
		if (166 - Color_Subtraction <= 0) {
			c = Color(0, 0, 0, 255);
		}
		else {
			c = Color(166 - Color_Subtraction, 166 - Color_Subtraction, 0, 255);
		}
		_is_light = false;
		break;
	}

	max_a_tick = Alpha_Time;
	int size;
	switch (_is_light) {
	case 0:
		size = _heavy_objects.size();
		if (size >= MAX_NUM_HEAVY) {
			
			_w_ptr->getb2World()->DestroyBody(_heavy_objects[0]->body);
			_heavy_objects[0]->detach();
			_heavy_objects[0]->has_been_destroyed = true;
			_heavy_objects.erase(_heavy_objects.begin());
			
		}
		break;
	case 1:
		size = _light_objects.size();

		if (size >= MAX_NUM_LIGHT) {
			
			_w_ptr->getb2World()->DestroyBody(_light_objects[0]->body);
			_light_objects[0]->detach();
			_light_objects[0]->has_been_destroyed = true;
			_light_objects.erase(_light_objects.begin());
			
		}
	}
	

	
	
	//Creating the tween that allows for the fading of the particle.
	//Once the tween is done, the object will put itself into the _to_delete vector.
	


	//Setting the sprite to the given variables.
	setSize(Size);
	setColor(c);
	
	//Setting the position of the particle to the given position.
	setPosition(GF::convert(pos));

	//Defining the body.
	b2BodyDef bd;
	bd.angle = random2::randomrange(0, 4);
	bd.position = pos;
	bd.type = b2_dynamicBody;



	//Creating the body.
	body = _w_ptr->getb2World()->CreateBody(&bd);
	switch (type) {
	case 2:
		addFixture(Vector2(4, 4), -1, 10, 0, 3);
		break;
	case 3:
		addFixture(Vector2(4, 4), -1, 10, 0, 3);
	}


	body->SetLinearVelocity(direction);
	body->SetLinearDamping(5);
	if (_is_light) {
		_light_objects.push_back(this);
	}
	else {
		_heavy_objects.push_back(this);
	}
	setName("Particle");
	init_size = getSize();
	body->SetUserData(this);
	_w_ptr->addChild(this);
	
	
}


void Particle::addFixture(const Vector2 &size, int16 groupindex, float32 friction, float32 restitution, float32 density) {
	b2FixtureDef fd;
	
	fd.filter.groupIndex = groupindex;
	fd.filter.categoryBits = PARTICLE;
	fd.friction = friction;
	fd.restitution = restitution;
	fd.density = density;
	b2PolygonShape shape;
	b2Vec2 sz = GF::convert(size);
	shape.SetAsBox(sz.x,sz.y);
	fd.shape = &shape;
	body->CreateFixture(&fd);

}

void Particle::when_t_done(Event *ev) {
	_to_delete.push_back(this);
}

Particle::Particle(const Vector2 &pos, const Vector2 &direction, int type) {
	create(GF::convert(pos), GF::convert(direction), type);
}

Particle::Particle(const b2Vec2 &pos, const b2Vec2 &direction, int type) {
	create(pos, direction, type);
}

void Particle::doUpdate(const UpdateState &us) {
	a_tick += us.dt;
	
	setAlpha(255 - (255 * (a_tick / max_a_tick)));
	
	switch (_type) {
	case 3:
		setSize(init_size - Vector2(init_size.x * (a_tick / max_a_tick), init_size.y * (a_tick / max_a_tick)));
		break;
	case 2:
		
		setSize(init_size - Vector2(init_size.x * (a_tick / max_a_tick), init_size.y * (a_tick / max_a_tick)));
		break;
	}
	if (a_tick >= max_a_tick) {
		setAlpha(0);
		_to_delete.push_back(this);
	}
	/*Doing particle velocity decay. ->
	I decided not to do atmospheric velocity decay, as done in Ship_M.h because it would require more calculations, 
	thus less particles.*/

	
	//body->ApplyForceToCenter(part_decay, true);
}

void Particle::find_erase(spParticle ptr_to_erase) {
	switch (ptr_to_erase->_is_light) {
	case 1:
		for (unsigned int i = 0; i < _light_objects.size(); i++) {
			if (_light_objects[i] == ptr_to_erase) {
				_light_objects.erase(_light_objects.begin() + i);
				break;
			}
		}
		break;
	case 0:
		for (unsigned int i = 0; i < _heavy_objects.size(); i++) {
			if (_heavy_objects[i] == ptr_to_erase) {
				_heavy_objects.erase(_heavy_objects.begin() + i);
				break;
			}
		}
	}
	
}

void Particle::destroy() {
	_w_ptr->getb2World()->DestroyBody(body);
	detach();
	find_erase(this);
}
void Particle::tick() {
	for (unsigned int i = 0; i < _to_delete.size(); i++) {
		
		if (_to_delete[i]->has_been_destroyed == false) {
			//Destroying the body within the object that is going to be deleted.
			_w_ptr->getb2World()->DestroyBody(_to_delete[i]->body);

			//Detach from the world.
			_to_delete[i]->detach();
			//Finding the object within the _object vector and erasing that, so that we do not have a nullptr left in there after deletion.
			find_erase(_to_delete[i]);
			_to_delete[i]->has_been_destroyed = true;
		}
		
		
		
		//Finally deleting the object.
		//_to_delete[i];

	}
	_to_delete.clear();
}

void Particle::init(World* world_ptr) {
	rand_store.setup(rand_store_size, rand_low, rand_high);
	rand_booleans.setup(rand_store_size);
	_w_ptr = world_ptr;
}

//This will modify the positional argument you have given, and give it a random variation.
void Particle::modify_pos(b2Vec2 &pos, float how_much) {
	Vector2 pos_variation = Vector2(how_much + (rand_store.getNum() / 2), how_much + (rand_store.getNum() / 2));
	//Checking to see if the x coordinate should be negative.
	if (rand_booleans.getNum() == 1) {
		pos_variation.x = -pos_variation.x;
	}
	//Checking to see if the y coordinate should be negative.
	if (rand_booleans.getNum() == 1) {
		pos_variation.y = -pos_variation.y;
	}

	pos += GF::convert(pos_variation);
}

void Particle::clear() {
	//THE DELETION PROBLEM HAS SOMETHING TO DO WITH THIS!
	for (unsigned int i = 0; i < _to_delete.size(); i++) {
		_to_delete[i]->destroy();
	}
	_to_delete.clear();
	for (unsigned int i = 0; i < _heavy_objects.size(); i++) {
		_heavy_objects[i]->destroy();
	}
	_heavy_objects.clear();
	for (unsigned int i = 0; i < _light_objects.size(); i++) {
		_light_objects[i]->destroy();
	}
	_light_objects.clear();
}