#include "Temple.h"
#include "ImageRes.h"
#include "ImageRes.h"
#include "Spike.h"
#include "ST.h"
#include "ForceField.h"
#include "Debug.h"
#define grid_size 16
#define max_world 5
#define max_level 3
#define total_wl max_world * max_level
#define max_rand_try 3 //The higher this number is, the more random the temples are; however, it will make for a slower restart and start up.
std::vector<Temple::r_data> Temple::_random_data;
unsigned int Temple::current_index;
BackPlate::BackPlate(const Vector2 &position) : init_pos(position) {
	
}

void BackPlate::doUpdate(const UpdateState &us) {
	if (_rel_body != NULL) {
		setPosition(GF::convert(_rel_body->GetPosition()) - (actor->getSize() / 2));
	}
}

void BackPlate::setRel(b2Body *rel_body) {
	_rel_body = rel_body;
	actor = static_cast<Actor*>(_rel_body->GetUserData());
}

Block_Fader::Block_Fader(World* world_ptr) {
	setPosition(Vector2(0,0));
	world_ptr->addChild(this);
}

T_Block::T_Block(const Vector2 &rel_pos, Vector2 pos, unsigned int direction, int length_in_blocks) {
	RectF r;
	pos *= grid_size;
	length_in_blocks *= grid_size;
	//Checking which direction it is going in, and making the rectangle.
	switch (direction) {
	case 0://Direction is up
		r = RectF(rel_pos + pos, Vector2(grid_size, -length_in_blocks));
		break;
	case 1://Direction is right
		r = RectF(rel_pos + pos, Vector2(length_in_blocks, grid_size));
		break;
	case 2://Direction is down
		r = RectF(rel_pos + pos, Vector2(grid_size, length_in_blocks));
		break;
	case 3://Direction is left
		r = RectF(rel_pos + pos, Vector2(-length_in_blocks, grid_size));
		break;
	}
	

	//Making the sprite
	setName("T_Block");
	setPosition(pos);
	setResAnim(res::resources.getResAnim("Block2"));
	setVerticalMode(Box9Sprite::StretchMode::TILING_FULL);
	//setVerticalGuides(0, 0);
	setHorizontalMode(Box9Sprite::StretchMode::TILING_FULL);
	setGuides(0, 0, 0, 0);
	
	
	//setAnchor(0.0f, 0.0f);
	//-=-=-=-=-=-=-=-=-=-
	setup(this, RectF(rel_pos + pos, Vector2(16, 16)));
	//setPosition(getPosition() + (r.getSize() / 2));
	setSize(r.size.x, r.size.y);
	r.pos += (r.getSize() / 2);
	setup_body(r);
	setPriority(1000);
	body->SetUserData(this);
	
	
}

void T_Block::doUpdate(const UpdateState &us) {
	
}

T_Block_K::T_Block_K(const Vector2 &rel_pos, Vector2 pos, unsigned int direction, int length_in_blocks, float speed, float distance) : _speed(speed){
	RectF r;
	pos *= grid_size;
	length_in_blocks *= grid_size;
	//Checking which direction it is going in, and making the rectangle.
	switch (direction) {
	case 0://Direction is up
		r = RectF(rel_pos + pos, Vector2(grid_size, -length_in_blocks));
		break;
	case 1://Direction is right
		r = RectF(rel_pos + pos, Vector2(length_in_blocks, grid_size));
		break;
	case 2://Direction is down
		r = RectF(rel_pos + pos, Vector2(grid_size, length_in_blocks));
		break;
	case 3://Direction is left
		r = RectF(rel_pos + pos, Vector2(-length_in_blocks, grid_size));
		break;
	}


	//Making the sprite
	setName("T_Block_K");
	setPosition(pos);
	setResAnim(res::resources.getResAnim("Block2"));
	setVerticalMode(Box9Sprite::StretchMode::TILING_FULL);
	//setVerticalGuides(0, 0);
	setHorizontalMode(Box9Sprite::StretchMode::TILING_FULL);
	setGuides(0, 0, 0, 0);


	//setAnchor(0.0f, 0.0f);
	//-=-=-=-=-=-=-=-=-=-
	setup(this, RectF(rel_pos + pos, Vector2(16, 16)));
	//setPosition(getPosition() + (r.getSize() / 2));
	setSize(r.size.x, r.size.y);
	r.pos += (r.getSize() / 2);
	setup_body(r);
	setPriority(1000);
	body->SetUserData(this);
	start_pos = getPosition();
	_direction = direction;
	distance *= grid_size;
	switch (direction) {
		case 1:
			
			_boundary = start_pos.y;//getPosition().y - distance;
			destination = GF::convert(Vector2(start_pos.x, start_pos.y - distance));
			if (distance < 0) {
				_init_direction = 0;
			}
			else {
				_init_direction = 2;
			}
			break;
		case 2:
			
			_boundary = start_pos.x;//getPosition().x + distance;
			destination = GF::convert(Vector2(start_pos.x + distance, start_pos.y));
			if (distance < 0) {
				_init_direction = 3;
				break;
			}
			else {
				_init_direction = 1;
			}
	}

	set_direction(_init_direction);
	_direction = _init_direction;
	
}

void T_Block_K::set_direction(unsigned int new_direction) {
	_direction = new_direction;
	
	switch (new_direction) {
	case 0:
		if (_boundary == start_pos.y) {
			_boundary = GF::convert_to_Oxy(destination.y);
		}
		else {
			_boundary = start_pos.y;
		}
		
		body->SetLinearVelocity(b2Vec2(0, _speed));
		break;
	case 1:
		if (_boundary == start_pos.x) {
			_boundary = GF::convert_to_Oxy(destination.x);
		}
		else {
			_boundary = start_pos.x;
		}
		body->SetLinearVelocity(b2Vec2(_speed, 0));
		break;
	case 2:
		if (_boundary == start_pos.y) {
			_boundary = GF::convert_to_Oxy(destination.y);
		}
		else {
			_boundary = start_pos.y;
		}
		body->SetLinearVelocity(b2Vec2(0, -_speed));
		break;
	case 3:
		if (_boundary == start_pos.x) {
			_boundary = GF::convert_to_Oxy(destination.x);
		}
		else {
			_boundary = start_pos.x;
		}
		body->SetLinearVelocity(b2Vec2(-_speed, 0));
	}
}

void T_Block_K::doUpdate(const UpdateState &us) {
	//float pos = getPosition().y;
	//Debug::print(getPosition().y);
	//Debug::print(_boundary);
	switch (_direction) {
	case 0:
		if (getPosition().y >= _boundary) {
			
			set_direction(2);
		}
		break;
	case 1:
		if (getPosition().x >= _boundary) {
			
			set_direction(3);
			
		}
		break;
	case 2:
		if (getPosition().y <= _boundary) {
			
			set_direction(0);
		}
		break;
	case 3:
		if (getPosition().x <= _boundary) {
			
			set_direction(1);
			
		}
		

	}
}

void Temple::create_spike(const Vector2 &rel_pos, Vector2 pos, unsigned int direction, int length_in_blocks, bool is_flipped, World* world_ptr) {
	spSpike e;
	e = new Spike(rel_pos, pos, direction, length_in_blocks, is_flipped);
	
	world_ptr->addChild(e);
	walls.push_back(e);
}


void Temple::create_wall(const Vector2 &rel_pos, Vector2 pos, unsigned int direction, int length_in_blocks, World* world_ptr) {
	spT_Block e;
	e = new T_Block(rel_pos, pos, direction, length_in_blocks);
	if (pos == Vector2(0, 0) && direction == 1) {
		bg->setRel(e->getBody());
	}
	world_ptr->addChild(e);
	walls.push_back(e);
}

void Temple::create_ST(const Vector2 &rel_pos, Vector2 pos, unsigned int type, World* world_ptr) {
	/*spT_Block e;
	e = new T_Block(rel_pos, pos, 1, 5);
	world_ptr->addChild(e);
	walls.push_back(e);*/
	
	//pos *= grid_size;
	spST e;
	e = new ST(rel_pos, pos, type);
	
	world_ptr->addChild(e);
	walls.push_back(e);
}

void Temple::create_wall_K(const Vector2 &rel_pos, Vector2 pos, unsigned int direction, int length_in_blocks, float speed, float distance, World* world_ptr) {
	spT_Block_K e;
	e = new T_Block_K(rel_pos, pos, direction, length_in_blocks, speed, distance);
	world_ptr->addChild(e);
	walls.push_back(e);
}

void Temple::create_ForceField(const Vector2 &rel_pos, RectF box, int direction, float force, World* world_ptr, unsigned int fadeTime) {
	box.pos.x *= grid_size;
	box.pos.y *= grid_size;
	box.size.x *= grid_size;
	box.size.y *= grid_size;
	box.pos += box.getSize() / 2;
	box.pos += rel_pos;
	spForceField e;
	e = new ForceField(box, direction, force, fadeTime, false);
	world_ptr->addChild(e);
	walls.push_back(e);
}

void Temple::tick_ci() {
	current_index++;
}

void Temple::create_structures(unsigned int type, World* world_ptr, const Vector2 &rel_pos, unsigned int fadeTime) {
	float int_1;
	float int_2;
	float int_3;
	float int_4;
	float int_5;
	float int_6;
	float int_7;
	World* w_ptr = static_cast<World*>(world_ptr);



	/*bool legit_data = true;
	r_data current_data;
	while (exit == false) {
		current_data = r_data(random2::randomrange(0, 2), random2::randomrange(0, 4));
		for (unsigned int i = 0; i < _random_data.size(); i++) {
			if (current_data == _random_data[i]) {
				legit_data = false;
				break;
			}
		}
		if (legit_data) {
			break;
		}
	}
	if (!exit) {
		_random_data.push_back(current_data);
	}*/
	r_data* r_ptr = &_random_data[current_index];
	
	switch (r_ptr->getWorld()){//r_ptr->getWorld()) {
	case 0:
		switch (r_ptr->getLevel()) {
		case 0:

			


			int_1 = round(sd_w_blocks / 2); //This is the left bottom wall.
			int_2 = 15; //This is the bottom entrance size.
			int_3 = sd_w_blocks - (int_1 + int_2); //This is the right bottom wall.
			int_4 = sd_w_blocks / 2; //This is the size of the middle block.
			create_wall(rel_pos, Vector2(0, 0), 2, sd_h_blocks, world_ptr);
			create_wall(rel_pos, Vector2(0, sd_h_blocks - 1), 1, int_1, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, sd_h_blocks - 1), 1, int_3, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - 1, 0), 2, sd_h_blocks, world_ptr); //Right wall
			create_wall(rel_pos, Vector2(0, 0), 1, int_1, world_ptr); //Ceiling left
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, 0), 1, int_3, world_ptr); //Ceiling right
			create_wall(rel_pos, Vector2(sd_w_blocks / 2 - (int_4 / 2), sd_h_blocks / 2 - 0.5), 1, int_4, world_ptr); //Middle block
			create_spike(rel_pos, Vector2(sd_w_blocks / 2 - (int_4 / 2), sd_h_blocks / 2 - 1.5), 1, int_4, false, world_ptr); //Middle block - Top spikes
			create_spike(rel_pos, Vector2(sd_w_blocks / 2 - (int_4 / 2), sd_h_blocks / 2 + 0.5), 1, int_4, true, world_ptr); //Middle block - Bottom spikes
																															 //Left wall spikes
			create_spike(rel_pos, Vector2(1, 1), 2, sd_h_blocks - 2, false, world_ptr); //Middle block - Bottom spikes
																						//Right wall spikes
			create_spike(rel_pos, Vector2(sd_w_blocks - 2, 1), 2, sd_h_blocks - 2, true, world_ptr); //Middle block - Bottom spikes

																									 //create_spike(rel_pos, Vector2(sd_w_blocks - 1, 1), 0, sd_h_blocks - 2, world_ptr); //Middle block - Bottom spikes
																									 //create_spike(rel_pos, Vector2(sd_w_blocks - 1, 1), 0, sd_h_blocks - 2, world_ptr); //Middle block - Bottom spikes

																									 //create_wall(rel_pos, Vector2(2, 3), 1, sd_h_blocks, world_ptr);
			break;
		case 1:
			int_1 = round(sd_w_blocks / 2); //This is the left bottom wall.
			int_2 = 15; //This is the bottom entrance size.
			int_3 = sd_w_blocks - (int_1 + int_2); //This is the right bottom wall.

			create_wall(rel_pos, Vector2(0, 0), 2, sd_h_blocks, world_ptr);
			create_wall(rel_pos, Vector2(0, sd_h_blocks - 1), 1, int_1, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, sd_h_blocks - 1), 1, int_3, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - 1, 0), 2, sd_h_blocks, world_ptr); //Right wall
			create_wall(rel_pos, Vector2(0, 0), 1, int_1, world_ptr); //Ceiling left
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, 0), 1, int_3, world_ptr); //Ceiling right
			create_wall(rel_pos, Vector2(int_1, 0), 2, sd_h_blocks, world_ptr); //Left wall middle down
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, 0), 2, sd_h_blocks, world_ptr); //Right wall middle down
																							  //create_ST(rel_pos, Vector2(sd_w_blocks / 2, sd_h_blocks / 2), 0, world_ptr);

			break;
		case 2:
			int_1 = round(sd_w_blocks / 2); //This is the left bottom wall.
			int_2 = 15; //This is the bottom entrance size.
			int_3 = sd_w_blocks - (int_1 + int_2); //This is the right bottom wall.
			int_4 = round(sd_w_blocks / 3); //Position of middle "direction right" block.
			create_wall(rel_pos, Vector2(0, 0), 2, sd_h_blocks, world_ptr);
			create_wall(rel_pos, Vector2(0, sd_h_blocks - 1), 1, int_1, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, sd_h_blocks - 1), 1, int_3, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - 1, 0), 2, sd_h_blocks, world_ptr); //Right wall
			create_wall(rel_pos, Vector2(0, 0), 1, int_1, world_ptr); //Ceiling left
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, 0), 1, int_3, world_ptr); //Ceiling right
			create_wall(rel_pos, Vector2(sd_w_blocks / 3, sd_h_blocks / 2), 1, sd_w_blocks / 3, world_ptr);
			create_wall(rel_pos, Vector2(int_4, sd_h_blocks / 2), 1, int_4, world_ptr); //Middle block flat.
			create_wall(rel_pos, Vector2(int_4 * 2, sd_h_blocks / 3), 2, sd_h_blocks / 4, world_ptr); //Middle block upward
			create_spike(rel_pos, Vector2(int_4, (sd_h_blocks / 2) - 1), 1, int_4, false, world_ptr); //Spikes on top of the middle block.
			create_spike(rel_pos, Vector2(1, 1), 2, sd_h_blocks - 2, false, world_ptr); //Left wall spikes
			create_spike(rel_pos, Vector2(sd_w_blocks - 2, 1), 2, sd_h_blocks - 2, true, world_ptr); //Right wall spikes
			create_spike(rel_pos, Vector2((int_4 * 2) + 1, sd_h_blocks / 3), 2, sd_h_blocks / 4, false, world_ptr); //Right wall spikes
			break;




		}
		break;
	case 1:
		switch (r_ptr->getLevel()) {
		case 0:
			
			



			int_2 = 15;//15 //This is the bottom entrance size.
			int_1 = (sd_w_blocks / 2) - (int_2 / 2); //This is the left bottom wall. WIDTH
			int_3 = sd_w_blocks - (int_1 + (int_2 / 2)); //This is the right bottom wall. 
			int_4 = 12; //This is the strength of the forcefields.
			create_wall(rel_pos, Vector2(0, 0), 2, sd_h_blocks, world_ptr); //Left wall
			create_wall(rel_pos, Vector2(0, sd_h_blocks - 1), 1, int_1, world_ptr); //Left bottom wall
			create_wall(rel_pos, Vector2(int_1 + int_2, sd_h_blocks - 1), 1, int_1, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - 1, 0), 2, sd_h_blocks, world_ptr); //Right wall
			create_wall(rel_pos, Vector2(0, 0), 1, int_1, world_ptr); //Ceiling left
			create_wall(rel_pos, Vector2(int_1 + int_2, 0), 1, int_1, world_ptr); //Ceiling right
			
			create_ForceField(rel_pos, RectF(Vector2(1,1), Vector2((sd_w_blocks / 2) - 1, sd_h_blocks - 2)), 3, int_4, world_ptr, fadeTime); //Left side
			create_ForceField(rel_pos, RectF(Vector2((sd_w_blocks / 2), 1), Vector2((sd_w_blocks / 2 - 1), sd_h_blocks - 2)), 1, int_4, world_ptr, fadeTime); //Right side
			create_spike(rel_pos, Vector2(1, 1), 2, sd_h_blocks - 2, false, world_ptr); //Middle block - Bottom spikes
																						//Right wall spikes
			create_spike(rel_pos, Vector2(sd_w_blocks - 2, 1), 2, sd_h_blocks - 2, true, world_ptr); //Middle block - Bottom spikes
			break;
		case 1:
			int_2 = 15;//15 //This is the bottom entrance size.
			int_1 = (sd_w_blocks / 2) - (int_2 / 2); //This is the left bottom wall. WIDTH
			int_3 = sd_w_blocks - (int_1 + (int_2 / 2)); //This is the right bottom wall. 
			int_4 = 12; //This is the strength of the forcefields.
			create_wall(rel_pos, Vector2(0, 0), 2, sd_h_blocks, world_ptr); //Left wall
			create_wall(rel_pos, Vector2(0, sd_h_blocks - 1), 1, int_1, world_ptr); //Left bottom wall
			create_wall(rel_pos, Vector2(int_1 + int_2, sd_h_blocks - 1), 1, int_1, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - 1, 0), 2, sd_h_blocks, world_ptr); //Right wall
			create_wall(rel_pos, Vector2(0, 0), 1, int_1, world_ptr); //Ceiling left
			create_wall(rel_pos, Vector2(int_1 + int_2, 0), 1, int_1, world_ptr); //Ceiling right
			create_ST(rel_pos, Vector2(sd_w_pixels / 2, sd_h_pixels / 2), 0, world_ptr);

			create_spike(rel_pos, Vector2(1, 1), 2, sd_h_blocks - 2, false, world_ptr); //Middle block - Bottom spikes
																						//Right wall spikes
			create_spike(rel_pos, Vector2(sd_w_blocks - 2, 1), 2, sd_h_blocks - 2, true, world_ptr);
			break;
		case 2:	 
			int_1 = round(sd_w_blocks / 2); //This is the left bottom wall.
			int_2 = 15; //This is the bottom entrance size.
			int_3 = sd_w_blocks - (int_1 + int_2); //This is the right bottom wall.
			int_4 = 15; //This is the distance that the right wall moves left.
			create_wall(rel_pos, Vector2(0, 0), 2, sd_h_blocks, world_ptr);
			create_wall(rel_pos, Vector2(0, sd_h_blocks - 1), 1, int_1, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, sd_h_blocks - 1), 1, int_3, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - (1 + int_4), 0), 2, sd_h_blocks, world_ptr); //Right wall
			create_wall(rel_pos, Vector2(0, 0), 1, int_1, world_ptr); //Ceiling left
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, 0), 1, int_3, world_ptr); //Ceiling right
			create_ST(rel_pos, Vector2(sd_w_pixels / 2, sd_h_pixels / 2), 0, world_ptr);																								 //Left wall spikes
			create_spike(rel_pos, Vector2(1, 1), 2, sd_h_blocks - 2, false, world_ptr); //Middle block - Bottom spikes
			create_spike(rel_pos, Vector2(sd_w_blocks - (2 + int_4), 1), 2, sd_h_blocks - 2, true, world_ptr);
			break;
		}
		break;
	case 2:
		switch (r_ptr->getLevel()) {
		case 0:




			
			int_2 = 15;//15 //This is the bottom entrance size.
			int_1 = (sd_w_blocks / 2) - (int_2 / 2); //This is the left bottom wall. WIDTH
			int_3 = sd_w_blocks - (int_1 + (int_2 / 2)); //This is the right bottom wall. 
			int_4 = 30; //This is the strength of the forcefields.
			int_5 = 15; //This is the size of the middle block.
			int_6 = 10; //This is the height from the top that the middle block is.
			create_wall(rel_pos, Vector2(0, 0), 2, sd_h_blocks, world_ptr); //Left wall
			create_wall(rel_pos, Vector2(0, sd_h_blocks - 1), 1, int_1, world_ptr); //Left bottom wall
			create_wall(rel_pos, Vector2(int_1 + int_2, sd_h_blocks - 1), 1, int_1, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - 1, 0), 2, sd_h_blocks, world_ptr); //Right wall
			create_wall(rel_pos, Vector2(0, 0), 1, int_1, world_ptr); //Ceiling left
			create_wall(rel_pos, Vector2(int_1 + int_2, 0), 1, int_1, world_ptr); //Ceiling right
			
			create_ForceField(rel_pos, RectF(Vector2(0, 0), Vector2(sd_w_blocks, sd_h_blocks)), 0, int_4, world_ptr, fadeTime);
			create_wall(rel_pos, Vector2((sd_w_blocks / 2) - (int_5 / 2), int_6), 1, int_5, world_ptr);
			create_spike(rel_pos, Vector2((sd_w_blocks / 2) - (int_5 / 2), int_6 + 1), 1, int_5, true, world_ptr);
			break;
		case 1:
			
			int_2 = 15;//15 //This is the bottom entrance size.
			int_1 = (sd_w_blocks / 2) - (int_2 / 2); //This is the left bottom wall. WIDTH
			int_3 = sd_w_blocks - (int_1 + (int_2 / 2)); //This is the right bottom wall. 
			int_4 = 12; //This is the strength of the forcefields.
			create_wall_K(rel_pos, Vector2(0, 0), 2, sd_h_blocks, 5, int_1, world_ptr); //Left wall
			create_wall(rel_pos, Vector2(0, sd_h_blocks - 1), 1, int_1, world_ptr); //Left bottom wall
			create_wall(rel_pos, Vector2(int_1 + int_2, sd_h_blocks - 1), 1, int_1, world_ptr);
			create_wall_K(rel_pos, Vector2(sd_w_blocks - 1, 0), 2, sd_h_blocks, 5, -int_1, world_ptr); //Right wall
			create_wall(rel_pos, Vector2(0, 0), 1, int_1, world_ptr); //Ceiling left
			create_wall(rel_pos, Vector2(int_1 + int_2, 0), 1, int_1, world_ptr); //Ceiling right
			create_wall_K(rel_pos, Vector2(int_1 - 0.12f, 0), 1, int_2 + 0.12f, 2, 10, world_ptr);

			//create_spike(rel_pos, Vector2(1, 1), 2, sd_h_blocks - 2, false, world_ptr); //Middle block - Bottom spikes
																						//Right wall spikes
			//create_spike(rel_pos, Vector2(sd_w_blocks - 2, 1), 2, sd_h_blocks - 2, true, world_ptr);
			break;
		case 2:
			int_1 = round(sd_w_blocks / 2); //This is the left bottom wall.
			int_2 = 15; //This is the bottom entrance size.
			int_3 = sd_w_blocks - (int_1 + int_2); //This is the right bottom wall.
			int_4 = 15; //This is the distance that the right wall moves left.
			create_wall(rel_pos, Vector2(0, 0), 2, sd_h_blocks, world_ptr);
			create_wall(rel_pos, Vector2(0, sd_h_blocks - 1), 1, int_1, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, sd_h_blocks - 1), 1, int_3, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - 1, 0), 2, sd_h_blocks, world_ptr); //Right wall
			create_wall(rel_pos, Vector2(0, 0), 1, int_1, world_ptr); //Ceiling left
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, 0), 1, int_3, world_ptr); //Ceiling right
			create_ST(rel_pos, Vector2(sd_w_pixels / 4, sd_h_pixels / 2), 0, world_ptr);
			create_ST(rel_pos, Vector2(sd_w_pixels / 1.25f, sd_h_pixels / 2.30f), 0, world_ptr);	

			//Left wall spikes
			create_spike(rel_pos, Vector2(1, 1), 2, sd_h_blocks - 2, false, world_ptr); //Middle block - Bottom spikes
			//Right wall spikes
			create_spike(rel_pos, Vector2(sd_w_blocks - 2, 1), 2, sd_h_blocks - 2, true, world_ptr);
			break;
		}
		break;
	case 3:
		switch (r_ptr->getLevel()){//r_ptr->getLevel()) {
		case 0:
			


			int_2 = 15; //This is the bottom entrance size.
			int_1 = (sd_w_blocks / 2) - (int_2 / 2); //This is the left bottom wall.
			
			int_3 = (sd_w_blocks - (int_1 + int_2)); //This is the right bottom wall.
			int_4 = 15; //This is the distance that the right wall moves left.
			int_5 = 15; //This is the strength of the forcefields.
			create_wall(rel_pos, Vector2(0, 0), 2, sd_h_blocks, world_ptr);
			create_wall(rel_pos, Vector2(0, sd_h_blocks - 1), 1, int_1, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, sd_h_blocks - 1), 1, int_3, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - 1, 0), 2, sd_h_blocks, world_ptr); //Right wall
			create_wall(rel_pos, Vector2(0, 0), 1, int_1, world_ptr); //Ceiling left
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, 0), 1, int_3, world_ptr); //Ceiling right
			//create_ST(rel_pos, Vector2(sd_w_pixels / 4, sd_h_pixels / 2), 1, world_ptr);
			//create_ST(rel_pos, Vector2(sd_w_pixels / 1.25f, sd_h_pixels / 2.30f), 1, world_ptr);
			//Left wall spikes
			create_spike(rel_pos, Vector2(1, 1), 2, sd_h_blocks - 2, false, world_ptr); //Middle block - Bottom spikes
																						//Right wall spikes
			create_spike(rel_pos, Vector2(sd_w_blocks - 2, 1), 2, sd_h_blocks - 2, true, world_ptr);
			create_ForceField(rel_pos, RectF(Vector2(1, 1), Vector2((sd_w_blocks / 2) - 1, sd_h_blocks - 2)), 0, int_5, world_ptr, fadeTime); //Left side
			create_ForceField(rel_pos, RectF(Vector2((sd_w_blocks / 2), 1), Vector2((sd_w_blocks / 2 - 1), sd_h_blocks - 2)), 2, int_5, world_ptr, fadeTime); //Right side
			



			
			break;
		case 1:
			int_2 = 15;//15 //This is the bottom entrance size.
			int_1 = (sd_w_blocks / 2) - (int_2 / 2); //This is the left bottom wall. WIDTH
			int_3 = sd_w_blocks - (int_1 + (int_2 / 2)); //This is the right bottom wall. 
			int_4 = 35; //This is the strength of the forcefields.
			int_5 = sd_w_blocks / 2; //This is the size of the middle block.
			int_6 = 10; //This is the height from the top that the middle block is.
			create_wall(rel_pos, Vector2(0, 0), 2, sd_h_blocks, world_ptr); //Left wall
			create_wall(rel_pos, Vector2(0, sd_h_blocks - 1), 1, int_1, world_ptr); //Left bottom wall
			create_wall(rel_pos, Vector2(int_1 + int_2, sd_h_blocks - 1), 1, int_1, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - 1, 0), 2, sd_h_blocks, world_ptr); //Right wall
			create_wall(rel_pos, Vector2(0, 0), 1, int_1, world_ptr); //Ceiling left
			create_wall(rel_pos, Vector2(int_1 + int_2, 0), 1, int_1, world_ptr); //Ceiling right
			create_wall_K(rel_pos, Vector2((sd_w_blocks / 2) - (int_5 / 2), 0), 2, int_6, 4, -int_6, world_ptr); //Left gate
			create_wall_K(rel_pos, Vector2((sd_w_blocks / 2) + (int_5 / 2), 0), 2, int_6, 4, int_6, world_ptr); //Right gate
			create_ForceField(rel_pos, RectF(Vector2(1,1), Vector2(sd_w_blocks, sd_h_blocks - 2)), 0, 15, world_ptr, fadeTime);
			create_wall(rel_pos, Vector2((sd_w_blocks / 2) - (int_5 / 2), int_6), 1, int_5, world_ptr);
			
			//create_spike(rel_pos, Vector2((sd_w_blocks / 2) - (int_5 / 2), int_6 + 1), 1, int_5, true, world_ptr);

			//create_spike(rel_pos, Vector2(1, 1), 2, sd_h_blocks - 2, false, world_ptr); //Middle block - Bottom spikes
			//Right wall spikes
			//create_spike(rel_pos, Vector2(sd_w_blocks - 2, 1), 2, sd_h_blocks - 2, true, world_ptr);
			break;
		case 2:
			int_2 = 15;//15 //This is the bottom entrance size.
			int_1 = (sd_w_blocks / 2) - (int_2 / 2); //This is the left bottom wall. WIDTH
			int_3 = sd_w_blocks - (int_1 + (int_2 / 2)); //This is the right bottom wall. 
			int_4 = 12; //This is the height of the middle block.

			create_wall_K(rel_pos, Vector2(0, (sd_h_blocks / 2) - (int_4 / 2)), 2, int_4, 40, sd_w_blocks, world_ptr); //Left wall
			create_wall(rel_pos, Vector2(0, sd_h_blocks - 1), 1, int_1, world_ptr); //Left bottom wall
			create_wall(rel_pos, Vector2(int_1 + int_2, sd_h_blocks - 1), 1, int_1, world_ptr);
			create_spike(rel_pos, Vector2(1, 1), 1, int_1 - 1, true, world_ptr); //Left top spikes
			create_spike(rel_pos, Vector2(int_1 + int_2, 1), 1, int_1 - 1, true, world_ptr); //Right top spikes
			create_wall(rel_pos, Vector2(sd_w_blocks - 1, 0), 2, sd_h_blocks, world_ptr); //Right wall
			create_wall(rel_pos, Vector2(0, 0), 2, sd_h_blocks, world_ptr); //Left wall
			create_spike(rel_pos, Vector2(1, sd_h_blocks - 2), 1, int_1 - 1, false, world_ptr); //Bottom left spikes
			create_spike(rel_pos, Vector2(int_1 + int_2, sd_h_blocks - 2), 1, int_1 - 1, false, world_ptr); //Bottom right spikes.
																											//create_wall_K(rel_pos, Vector2(sd_w_blocks - 1, 0), 2, sd_h_blocks, 5, -int_1, world_ptr); //Right wall
			create_wall(rel_pos, Vector2(0, 0), 1, int_1, world_ptr); //Ceiling left
			create_wall(rel_pos, Vector2(int_1 + int_2, 0), 1, int_1, world_ptr); //Ceiling right
																				  //create_wall_K(rel_pos, Vector2(int_1 - 0.12f, 0), 1, int_2 + 0.12f, 2, 10, world_ptr);
			break;
		}
		break;
	case 4:
		switch (r_ptr->getLevel()) {
		case 0:
			int_2 = 15;//15 //This is the bottom entrance size.
			int_1 = (sd_w_blocks / 2) - (int_2 / 2); //This is the left bottom wall. WIDTH
			int_3 = int_1;//sd_w_blocks - (int_1 + (int_2 / 2)); //This is the right bottom wall.
			int_4 = sd_w_blocks / 2; //This is the size of the middle block.
			int_5 = 10; //This is the standard width of all the forcefields.
			int_6 = 15; //This is the power of the forcefields.
			create_wall(rel_pos, Vector2(0, 0), 2, sd_h_blocks, world_ptr);
			create_wall(rel_pos, Vector2(0, sd_h_blocks - 1), 1, int_1, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, sd_h_blocks - 1), 1, int_3, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - 1, 0), 2, sd_h_blocks, world_ptr); //Right wall
			create_wall(rel_pos, Vector2(0, 0), 1, int_1, world_ptr); //Ceiling left
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, 0), 1, int_3, world_ptr); //Ceiling right
			create_wall(rel_pos, Vector2(sd_w_blocks / 2 - (int_4 / 2), sd_h_blocks / 2 - 0.5), 1, int_4, world_ptr); //Middle block
			create_spike(rel_pos, Vector2(sd_w_blocks / 2 - (int_4 / 2), sd_h_blocks / 2 - 1.5), 1, int_4, false, world_ptr); //Middle block - Top spikes
			create_spike(rel_pos, Vector2(sd_w_blocks / 2 - (int_4 / 2), sd_h_blocks / 2 + 0.5), 1, int_4, true, world_ptr); //Middle block - Bottom spikes
			//Left wall spikes
			//create_spike(rel_pos, Vector2(1, 1), 2, sd_h_blocks - 2, false, world_ptr); //Middle block - Bottom spikes
			//Right wall spikes
			//create_spike(rel_pos, Vector2(sd_w_blocks - 2, 1), 2, sd_h_blocks - 2, true, world_ptr); //Middle block - Bottom spikes
			create_ForceField(rel_pos, RectF(Vector2((sd_w_blocks / 2) - (int_4 / 2), (sd_h_blocks / 2) + 0.5), Vector2(int_5, (sd_h_blocks / 2) - 0.5)), 3, 15, world_ptr, fadeTime);																									 //create_spike(rel_pos, Vector2(sd_w_blocks - 1, 1), 0, sd_h_blocks - 2, world_ptr); //Middle block - Bottom spikes
			create_ForceField(rel_pos, RectF(Vector2(((sd_w_blocks / 2) + (int_4 / 2)) - (int_5 + 1), (sd_h_blocks / 2) + 0.5f), Vector2(int_5, (sd_h_blocks / 2) - 0.5)), 1, 15, world_ptr, fadeTime);																									 //create_spike(rel_pos, Vector2(sd_w_blocks - 1, 1), 0, sd_h_blocks - 2, world_ptr); //Middle block - Bottom spikes
			create_ForceField(rel_pos, RectF(Vector2((sd_w_blocks / 2) - (int_4 / 2), 0), Vector2(int_5, sd_h_blocks / 2)), 1, 15, world_ptr, fadeTime);	 //create_spike(rel_pos, Vector2(sd_w_blocks - 1, 1), 0, sd_h_blocks - 2, world_ptr); //Middle block - Bottom spikes
			create_ForceField(rel_pos, RectF(Vector2((sd_w_blocks / 2) + (int_4 / 2) - (int_5 + 1), 0.5f), Vector2(int_5, sd_h_blocks / 2)), 3, 15, world_ptr, fadeTime);
			break;
		case 1:
			int_2 = 15;//15 //This is the bottom entrance size.
			int_1 = (sd_w_blocks / 2) - (int_2 / 2); //This is the left bottom wall. WIDTH
			int_3 = int_1;//sd_w_blocks - (int_1 + (int_2 / 2)); //This is the right bottom wall.
			int_4 = sd_w_blocks / 2; //This is the size of the middle block.
			int_5 = 8; //This is the size of the middle block hole.
			int_6 = (int_4 / 2) - (int_5 / 2); //This is the width of the left middle block.
			int_7 = 23; //This is the width of the forcefields on the sides.
			create_wall(rel_pos, Vector2(0, 0), 2, sd_h_blocks, world_ptr);
			create_wall(rel_pos, Vector2(0, sd_h_blocks - 1), 1, int_1, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, sd_h_blocks - 1), 1, int_3, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - 1, 0), 2, sd_h_blocks, world_ptr); //Right wall
			create_wall(rel_pos, Vector2(0, 0), 1, int_1, world_ptr); //Ceiling left
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, 0), 1, int_3, world_ptr); //Ceiling right
			create_wall_K(rel_pos, Vector2(sd_w_blocks / 2 - (int_4 / 2), sd_h_blocks / 2 - 0.5f), 1, int_6, 10, sd_h_blocks / 2, world_ptr);
			create_wall_K(rel_pos, Vector2((sd_w_blocks / 2 - (int_4 / 2)) + (int_6 + int_5), sd_h_blocks / 2 - 0.5f), 1, int_6, 10, sd_h_blocks / 2, world_ptr);
			//Left wall spikes
			create_spike(rel_pos, Vector2(1, 1), 2, sd_h_blocks - 2, false, world_ptr); //Middle block - Bottom spikes
																						//Right wall spikes
			create_spike(rel_pos, Vector2(sd_w_blocks - 2, 1), 2, sd_h_blocks - 2, true, world_ptr); //Middle block - Bottom spikes
			create_ForceField(rel_pos, RectF(Vector2(1, 1), Vector2(int_7, sd_h_blocks - 1)), 3, 20, world_ptr, fadeTime);
			create_ForceField(rel_pos, RectF(Vector2(sd_w_blocks - (int_7 + 1), 0), Vector2(int_7, sd_h_blocks)), 1, 20, world_ptr, fadeTime);

			break;
		case 2:
			int_2 = 15;//15 //This is the bottom entrance size.
			int_1 = (sd_w_blocks / 2) - (int_2 / 2); //This is the left bottom wall. WIDTH
			int_3 = int_1;//sd_w_blocks - (int_1 + (int_2 / 2)); //This is the right bottom wall.
			int_4 = sd_h_blocks / 1.5f; //This is the height of the first wall that you come to.
			int_5 = sd_w_blocks / 1.5f; //This is the length of the first wall.
			int_6 = sd_h_blocks / 2.5f; //This is the height of the second wall you come across.
			int_7 = sd_w_blocks / 1.5f; //This is the length of the second wall you come across.
			create_wall(rel_pos, Vector2(0, 0), 2, sd_h_blocks, world_ptr);
			create_wall(rel_pos, Vector2(0, sd_h_blocks - 1), 1, int_1, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, sd_h_blocks - 1), 1, int_3, world_ptr);
			create_wall(rel_pos, Vector2(sd_w_blocks - 1, 0), 2, sd_h_blocks, world_ptr); //Right wall
			create_wall(rel_pos, Vector2(0, 0), 1, int_1, world_ptr); //Ceiling left
			create_wall(rel_pos, Vector2(sd_w_blocks - int_3, 0), 1, int_3, world_ptr); //Ceiling right

																						//This is the first wall you come up to.
			create_wall(rel_pos, Vector2(0, int_4), 1, int_5, world_ptr);
			//This is the second wall you come to.
			create_wall(rel_pos, Vector2(sd_w_blocks - int_7, int_6), 1, int_7, world_ptr);
			//Left wall spikes
			create_spike(rel_pos, Vector2(1, 1), 2, sd_h_blocks - 2, false, world_ptr); //Middle block - Bottom spikes			
			create_spike(rel_pos, Vector2(sd_w_blocks - 2, 1), 2, sd_h_blocks - 2, true, world_ptr); //Middle block - Bottom spikes

			break;
		}
	}
	tick_ci();
}



Temple::Temple(float y_coordinate, World* world_ptr, unsigned int type, unsigned int fadeTime) {
	
	sd_w_pixels = world_ptr->get_boundary_size(),
	sd_w_blocks = sd_w_pixels / grid_size;
	sd_h_blocks = 48;
	sd_h_pixels = sd_h_blocks * grid_size;
	bf = new Block_Fader(world_ptr);
	Vector2 pos = Vector2(world_ptr->get_boundary_left(), y_coordinate - (sd_h_pixels / 2));
	//Naming the temple and setting the position, etc...
	setPosition(pos);
	setName("Temple");
	
	//Making the background.
	bg = new BackPlate(Vector2(0,0));
	bg->setResAnim(res::resources.getResAnim("Temple_Backdrop"));
	bg->setPosition(Vector2(0,0));
	bg->setSize(Vector2(world_ptr->get_boundary_size(), sd_h_pixels));
	bg->setColor(Color::DarkGray);
	bg->setPriority(-500);
	bg->setName("Temple");
	world_ptr->addChild(bg);
	
	//-=-=-=-=-=-=-=-=-=-=-
	world_ptr->addChild(this);
	world_ptr->insert_shifter(this);
	create_structures(type, world_ptr, pos, fadeTime);
	setPriority(-8000);
	
	
}

void Temple::destroy() {
	for (unsigned int i = 0; i < walls.size(); i++) {
		walls[i]->detach();
		//walls[i].~intrusive_ptr();
	}
	walls.clear();
	bg->detach();
	//bg.~intrusive_ptr();
}

void Temple::onRemovedFromStage() {
	destroy();
}

void Temple::fadeOut(unsigned int time) {
	//GF::fadeOut(this, time, 0);
	GF::fadeOut(bf.get(), time, 0);
	//bg->setAlpha(255);
	//bg->setPosition(Vector2(0, 0));
	for (unsigned int i = 0; i < walls.size(); i++) {
		GF::fadeOut(walls[i].get(), time);
	}
	GF::fadeOut(this, time, 0);
}

void Temple::reset_cache() {
	_random_data.clear();
	fill_random();
	current_index = 0;
}

bool Temple::check_data(r_data data_to_check) {
	bool is_already_made = false;
	for (unsigned int b = 0; b < _random_data.size(); b++) {
		if (data_to_check == _random_data[b]) {
			is_already_made = true;
			break;
		}
	}
	return is_already_made;
}

bool Temple::check_data(r_data data_to_check, unsigned int index) {
	if (_random_data[index] == data_to_check) {
		return true;
	}
	else {
		return false;
	}
}

void Temple::calculate_possibilities(std::vector<r_data> *v_ptr) {
	v_ptr->clear();
	for (unsigned int i = 0; i < max_world; i++) {
		for (unsigned int b = 0; b < max_level; b++) {
			v_ptr->push_back(r_data(b, i));
		}
	}
}

void Temple::fill_random() {
	unsigned int how_many_placed = 0;
	//Filling place holders.
	//for (unsigned int i = 0; i < total_wl; i++) {
	//	_random_data.push_back(r_data(true));
	//}
	
	std::vector<r_data> possibilities;
	calculate_possibilities(&possibilities);
	
	//std::vector<unsigned int> tried_plis;
	unsigned int pli = 0;
	//Filling in random data.


	
	for (unsigned int i = 0; i < total_wl; i++) {
		pli = random2::randomrange(0, possibilities.size());
		_random_data.push_back(possibilities[pli]);
		possibilities.erase(possibilities.begin() + pli);
		
	}



	/*for (unsigned int i = 0; i < total_wl; i++) {
		
		unsigned int count = 0; //This determines how many times it will try to fill the place holder before manually filling them. (smf)
		bool smf = false; //Should Manually Fill
		bool exit = false;
		while (exit == false) {
			r_data current_data(random2::randomrange(0, 2), random2::randomrange(0, 4));
			//bool test = false;
			
			//while (test == false) {
				//bool legit = true;
			pli = random2::randomrange(0, total_wl - how_many_placed);
				for (unsigned int i = 0; i < tried_plis.size(); i++) {
					if (tried_plis[i] == pli) {
						legit = false;
						break;
					}
				}
				if (legit) {
					break;
				}
		//	}
			
			if (count > max_rand_try) {
				smf = true;
			}
			if (!smf) { //Filling in randomly.
				
				
				if (!check_data(current_data) && _random_data[pli].is_dummy()) {
					_random_data[pli] = current_data; //Insertion! -=-
					exit = true;
					how_many_placed++;
					break;
				}
				else {
					count++;
				}
			}
			else {
				for (unsigned int b = 0; b < _random_data.size(); b++) {
					if (_random_data[b].is_dummy()) {
						_random_data[b] = current_data; //Insertion! -=-
						exit = true;
						how_many_placed++;
						break;
					}
				}
			}
			
		}
		
	}
	for (unsigned int i = 0; i < _random_data.size(); i++) {
		if (_random_data[i].is_dummy()) {
			log::messageln("gef");
			//throw("error!");
		}
	}*/
}