#include "Platform.h"
#include "ImageRes.h"
#include "Ship_M.h"
#include <cmath>
#include "random2.h"
#include "Black_hole.h"
#include "ForceField.h"
#include "Debug.h"
#include "Final_Bird.h"
#include "Platform.h"
#include "Noise.h"
#define fadeOut_time 2000
#define fadeIn_time 2000
#define max_num_of_platforms 4//4
#define LAL 20 //Lower angle limit
#define UAL 160 //Upper angle limit
#define MAS 90 //Middle angle standard
#define FDAL 20 //Final degree addition limit
#define d_c_str 300
#define tpin_radius 20
#define default_dist 1500
#define final_x_divisor 2.5 //This is far from the left boundary the bird nest spawns.
//typedef intrusive_ptr<Platform> spPlatform;
int Platform::final_x;
float Platform::average_height;
Vector2 Platform::start_pos;
std::vector<spPlatform> Platform::objects;
std::vector<spPlatform> Platform::should_delete;
spPlatform Platform::to_delete;
int Platform::start_threshold;
bool Platform::_should_delete;
int Platform::how_many_platforms;
bool Platform::can_create_platforms;
Vector2 Platform::restart_pos;





Platform::Platform(const Vector2 &pos, int number_of_blocks, float angle, const Vector2 &pos2, float dist) : _init_pos(pos), _init_pos2(pos2), d_angle_rads(GF::rad(angle + 90)) {
	how_many_platforms++;
	
	//Creating confetti cannons.
	confetti_left = new P_System(Vector2(pos.x - (getSize().x / 2), pos.y), Vector2(-d_c_str, d_c_str / 2), 1, world_ptr, false);
	confetti_right = new P_System(Vector2(pos.x + (getSize().x / 2), pos.y), Vector2(d_c_str, d_c_str / 2), 1, world_ptr, false);
	world_ptr->addChild(confetti_left);
	world_ptr->addChild(confetti_right);
	confetti_right->setName("Confetti_right");
	confetti_left->setName("Confetti_left");
	//This determines if the platforms should make obstacles or not.
	bool should_make_obstacles = true;
	//If this is the very first platform, then don't make any obstacles.
	if (world_ptr->level == 0 && how_many_platforms == 1 && world_ptr->getworldNum() == 0) {
		should_make_obstacles = false;
	}

	setResAnim(res::resources.getResAnim("Platform"));
	
	setup(this, RectF(pos, Vector2(16, 16)));
	
	setSize(Vector2(16 * number_of_blocks, 16));

	if (how_many_platforms >= max_num_of_platforms) {
		
		end_line = new W_Line(world_ptr.get(), Vector2(getPosition().x - (getWidth() / 2), getHeight() / 2));
		addChild(end_line);
		is_final_platform = true;
		//how_many_platforms = 0;
	}

	
	switch (world_ptr->level) {
	case 0:
		type = NORMAL;
		break;
	case 1:
		type = NORMAL;
		break;
	case 2:
		if (is_final_platform) {
			type = NORMAL;
		}
		else {
			type = static_cast<types>(random2::getBool());
		}
		
	}
	if (is_final_platform) {
		if (world_ptr->is_final_world() && world_ptr->is_final_level()) {
			
			type = BIRD_NEST;
			should_make_obstacles = false;
		}
	}
	
	
	switch (type) {
	case 0: //It's a normal platform, do nothing.
		break;
	case 1: //It's a UPSIDE_DOWN_FORCEFIELD platform.
	{
		spForceField ff = new ForceField(RectF(Vector2(pos.x + 8, pos.y + 250), Vector2(getWidth() + 100, 500)), 0, 35, fadeIn_time);
		spSpike s = new Spike(Vector2((pos.x - (getSize().x / 2)) + 8, pos.y), Vector2(0, -1), 1, number_of_blocks, false);
		world_ptr->addChild(s);
		specials.push_back(ff);
		specials.push_back(s);
		break;
	}
		
	case 2:
	{
		spBird_Nest bn;
		bn = new Bird_Nest(Vector2(getX() - (getWidth() / 2) + 2, getY() - ((getHeight() / 2) + 16)), world_ptr.get());
		spFinal_Bird fb;
		fb = new Final_Bird(Vector2(world_ptr->get_boundary_left(), getY() - (res::resources.getResAnim("Bird_Body")->getHeight() / 2)), world_ptr.get());
		specials.push_back(bn);
		specials.push_back(fb);
		break;
	}
		
	}
	setAlpha(0);
	addTween(Actor::TweenAlpha(255), fadeIn_time);
	for (unsigned int i = 0; i < specials.size(); i++) {
		specials[i]->setAlpha(0);
		specials[i]->addTween(Actor::TweenAlpha(255), fadeIn_time);
	}
	setHorizontalMode(Box9Sprite::StretchMode::TILING_FULL);
	setGuides(0,0,0,0);
	b2FixtureDef fd;
	fd.filter.categoryBits = PLATFORM; //Is a platform.
	fd.filter.maskBits = DEFAULT_MASK; //Collides with everything.
	
	setup_body(RectF(Vector2(pos.x + (16 / 2),pos.y + (16 / 2)), Vector2(16 * number_of_blocks, 16)), NULL, NULL, &fd);
	setName("Pltfrm");
	_dline = new D_Line(pos, pos2, angle, dist, world_ptr.get(), is_final_platform, should_make_obstacles, fadeIn_time);
	world_ptr->addChild(_dline);
	
	body->SetUserData(this);
	_tpin = new TPIN(getPosition(), tpin_radius, world_ptr.get(), TPIN::type::PLATFORM);
	
}

void Platform::doUpdate(const UpdateState &us) {
	_dline->set(d_angle_rads - (MATH_PI / 2), GF::convert(body->GetPosition()), _init_pos2);
	_tpin->SetPosit(getPosition());
}


void Platform::has_landed() {
	
	setColor(Color::Red);
	if (!_has_been_landed) {
		if (Ship_M::objects[0]->getFuel() <= 0) {
			Noise::play_ID(Noise::st::Almost_Made_It, Noise::player::sfx);
		}
		Ship_M::objects[0]->refuel();
		fire_cannons();
		
		if (is_final_platform) {
			
			Noise::play_ID(Noise::st::Land_On_Platform_Final, Noise::player::sfx);
			if (world_ptr->is_final_world() && world_ptr->is_final_level()) {
				//This is called if you won the game.
				world_ptr->win_game();
				//special_destroy();
			}
			else {
				//how_many_platforms = 0;
				float new_height = getPosition().y + (getStage()->getHeight() / 2);
				
				world_ptr->ready_to_iterate(new_height);
				iterate();
			}
			
			//reset_platforms_except(this);
		}
		else {
			
			Noise::play_ID(Noise::st::Land_On_Platform, Noise::player::sfx);
			iterate();
		}
		
		
	}
	
	
	_has_been_landed = true;
}

void Platform::check_start(b2Body *ptr) {
	//Making clones of the pointer to modify the UserData of the original.
	Engine_M *_ptr = (Engine_M*)ptr->GetUserData();
	_ptr->is_settled = true;
	ptr->SetUserData(_ptr);
	Engine_M *__ptr = static_cast<Engine_M*>(ptr->GetUserData());
	Ship_M *___ptr = static_cast<Ship_M*>(_ptr->abstract_ptr);
	if (Ship_M::objects[__ptr->playerIndex]->engine_check() == true) {
		if (___ptr->is_in_working_order() == true) {
			has_landed();
		}
		else {
			
			if (___ptr->getFuel() <= 0 && ___ptr->get_has_crashed() == false) {
				has_landed();
			}
		}
		
	}
	
	
	
	_ptr->ifcontact_platform_start();
}

void Platform::check_end(b2Body *ptr) {
	//if (Ship_M::check_all() == true) {
		//iterate();
	//}
	Engine_M *_ptr = (Engine_M*)ptr->GetUserData();
	_ptr->is_settled = false;
	ptr->SetUserData(_ptr);
	Engine_M *__ptr = static_cast<Engine_M*>(ptr->GetUserData()); //Used for testing and reference.
	Ship_M *___ptr = static_cast<Ship_M*>(_ptr->abstract_ptr);
	if (_has_been_landed == true) {
		if (Ship_M::objects[__ptr->playerIndex]->are_both_engines_not_settled() == true && ___ptr->is_in_working_order() == true) {
			//iterate();
			//fadeOut_and_destroy();
		};
	}
	/*if (static_cast<Ship_M*>(__ptr->getParent())->engine_check() == true) {
		iterate();
	};*/ 
	
	

	//ptr->ifcontact_platform_end();
	
}

void Platform::clean() {
	
	for (unsigned int i = 0; i < objects.size(); i++) {
		objects[i]->removeTweens();
		objects[i]->destroy();
		
		//&objects[i].
		//delete objects[i].get();
		//objects[i].~intrusive_ptr();
	}
	for (unsigned int i = 0; i < should_delete.size(); i++) {
		should_delete[i] = NULL;
	}
	should_delete.clear();
	to_delete = NULL;
	can_create_platforms = true;
	objects.clear();
	how_many_platforms = 0;
	
	//Checking to see if there are any objects that are still pointing to something besides a nullptr.
	/*for (unsigned int i = 0; i < objects.size(); i++) {
		if (objects[i].ge != nullptr) {
			delete objects[i];
		}
	}
	objects.clear();*/
}

void Platform::iterate() {
	if (has_iterated == false) {
		

		spPlatform p = this;
		to_delete = p;
		_should_delete = true;
		
	}
	has_iterated = true;
	
}

void Platform::tick() {
	
	
	if (!world_ptr->did_win_game()) {
		unsigned int number_of_items = objects.size();
		

		/*if (_should_delete == true) {
		_should_delete = false;
		//GF::println_va(to_delete->_ref_counter);
		for (unsigned int i = 0; i < objects.size(); i++) {
		if (to_delete == objects[i]) {

		//objects.erase(objects.begin() + i);
		break;
		}
		}
		//GF::println_va(to_delete->_ref_counter);

		//to_delete->releaseRef();
		//GF::println_va(to_delete->_ref_counter);
		//to_delete = nullptr;

		//GF::println_va(to_delete->_ref_counter);

		}*/
		

		if (to_delete.get() != NULL) {
			if (to_delete->type != Platform::types::BIRD_NEST) {
				bool should_add;
				if (to_delete->is_final_platform) {
					should_add = false;
				}
				else {
					should_add = true;
				}

				unsigned int how_much = 0;
				unsigned int index = 0;

				while (1 < 2) {

					bool Is_Final = false;
					if (how_many_platforms >= max_num_of_platforms - 1) {
						if (world_ptr->is_final_world() && world_ptr->is_final_level()) {
							Is_Final = true;
						}
					}


					if (objects[index] == to_delete) {

						Vector2 new_pos = objects[objects.size() - 1]->getPosition();
						how_much++;
						objects[index]->fadeOut_and_destroy();
						if (to_delete->get_is_final_platform()) {
							can_create_platforms = true;
							//world_ptr->shift_pos(new_pos);
							create_platforms(new_pos, 3);
						}
						else {
							create_new_platform(new_pos, Is_Final);
						}
						//to_delete->detach();
						//world_ptr->getb2World()->DestroyBody(to_delete->body);
						to_delete = NULL;


						break;


					}
					else {
						how_much++;
						objects[index]->fadeOut_and_destroy();


						//world_ptr->getb2World()->DestroyBody(objects[0]->body);

						create_new_platform(objects[objects.size() - 1]->getPosition(), Is_Final);

					}
					index++;

				}
				
					
				
			}
			//else {
			//	special_destroy();
			//}
			

		}

		//Deleting platforms.
		for (unsigned int i = 0; i < should_delete.size(); i++) {
			for (unsigned int b = 0; b < objects.size(); b++) {
				if (objects[b] == should_delete[i]) {
					objects.erase(objects.begin() + b);
					break;
				}
			}
			
			should_delete[i]->destroy();
			//should_delete[i].~intrusive_ptr();


		}
		should_delete.clear();
	}
	
	
}

void Platform::create_platforms(const Vector2 &init_pos, int threshold) {
	how_many_platforms = 0;
	start_pos = init_pos;
	start_threshold = threshold;
	int iter = 0;
	Vector2 prev_pos = init_pos;
	
	while (-1) {
		iter++;
		if (iter >= threshold) {
			break;
		}
		else {
			bool Is_Final = false;
			if (how_many_platforms >= max_num_of_platforms - 1) {
				if (world_ptr->is_final_world()) {
					Is_Final = true;
				}
			}
			//Creating a new platform, also updating the prev_pos variable to the new one.
			prev_pos = create_new_platform(prev_pos, Is_Final);
			
		}
	}
	
	/*pltfrm = new Platform(Vector2(0, -400), 8);
	objects.push_back(pltfrm);
	world_ptr->addChild(pltfrm);
	pltfrm = new Platform(Vector2(0, -600), 8);
	objects.push_back(pltfrm);
	world_ptr->addChild(pltfrm);*/
	//objects.push_back(new Platform(Vector2(0, -400), 8));
	//objects.push_back(new Platform(Vector2(0, -600), 8));
	
	/*for (unsigned int i = 0; i < threshold; i++) {
		Platform *pltfrm = new Platform(Vector2(0, 0 - (i * 100)), 6);
		objects.push_back(pltfrm);
		world_ptr->addChild(pltfrm);
		
	}*/
}



Vector2 Platform::create_new_platform(const Vector2 &rel_pos, bool is_final) {
	if (can_create_platforms == true) {
		
		while (1 < 2) {
			switch (is_final) {
			case 0: //False
			{
				float dist = default_dist;
				//float angle = 87;
				//float angle = 90;
				/*(switch (how_many_platforms) {
				case 0:
					angle = 87;
					break;
				case 1:
					angle = 87;
					break;
				case 2:
					angle = 80;
					break;
				case 3:
					angle = 132;
					break;
				}*/
				float angle = random2::randomrange_complex(LAL, UAL);
								//float angle = 90;
								//float angle = 270 + angle_add;
								//float angle = 380;
				Vector2 v = rel_pos - Vector2(cos(GF::rad(angle)) * dist, sin(GF::rad(angle)) * dist);

				if (world_ptr->is_within_boundary(v, Vector2(70, 0)) == true) {
					Platform* pltfrm = new Platform(v, 11, angle, rel_pos, dist);
					objects.push_back(pltfrm);
					world_ptr->addChild(pltfrm);
					
					if (pltfrm->is_final_platform == true) {
						can_create_platforms = false;
					}

					return v;
				}
				break;
			}
			break;
			case 1: //True
			{

				bool is_x_negative = false;
				float dist = 1500;
				//float average_height = -400;
				float x_dif = final_x - rel_pos.x;
				
				if (x_dif < 0) { //Is negative
					//x_dif = abs(x_dif);
					is_x_negative = true;
				}
				//x_dif = -x_dif;
				float angle = GF::degrees(atan(x_dif / average_height));
				angle += 90;
				dist = GF::pyagor_hypot(x_dif, average_height);
				//angle = -(360 - angle);
				//float angle = 90;
				//float angle = 270 + angle_add;
				//float angle = 380;
				Vector2 v;
				if (is_x_negative == true) {
					v = rel_pos - Vector2(-x_dif, sin(GF::rad(angle)) * dist);
				}
				else {
					v = rel_pos - Vector2(-x_dif, average_height);
				}
				//sin(GF::rad(angle)) * dist
				//v.x = final_x;
				//if (world_ptr->is_within_boundary(v, Vector2(70, 0)) == true) {
				Platform* pltfrm = new Platform(v, 11, angle, rel_pos, dist);
				objects.push_back(pltfrm);
				world_ptr->addChild(pltfrm);
				if (pltfrm->is_final_platform == true) {
					can_create_platforms = false;
				}

				return v;
				//}
				break;





				/*float dist = 1500;
				float angle = random2::randomrange_complex(LAL, LAL + FDAL);
								  //float angle = 90;
								  //float angle = 270 + angle_add;
								  //float angle = 380;
				float x_dif = rel_pos.x - final_x;
				dist = x_dif / cos(GF::rad(angle));
				Vector2 v = rel_pos - Vector2(cos(GF::rad(angle)) * dist, sin(GF::rad(angle)) * dist);

				//if (world_ptr->is_within_boundary(v, Vector2(70, 0)) == true) {
					spPlatform pltfrm = new Platform(v, 11, angle, rel_pos, dist);
					objects.push_back(pltfrm);
					world_ptr->addChild(pltfrm);
					if (pltfrm->is_final_platform == true) {
						can_create_platforms = false;
					}

					return v;
				//}
				break;*/
			}
				
			}
			



			//GF::rad(angle + 90)

		}
	}
	
	
}

void Platform::destroy() {
	for (unsigned int i = 0; i < specials.size(); i++) {
		specials[i]->detach();
		//specials[i].~intrusive_ptr();
	}
	
	//_dline = NULL;h();
	confetti_right->detach();
	confetti_left->detach();
	confetti_left = NULL;
	confetti_right = NULL;
	//confetti_left.~intrusive_ptr();
	//confetti_right.~intrusive_ptr();
	if (_dline.get() != NULL) {
		_dline->destroy();
		_dline = NULL;
		//_dline.~intrusive_ptr();
	}
	
	if (end_line.get() != NULL) {
		end_line->destroy();
		end_line = NULL;
	}
	if (_tpin.get() != NULL) {
		_tpin->detach();
		_tpin = NULL;
		//_tpin.~intrusive_ptr();
	}
	
	world_ptr->getb2World()->DestroyBody(body);
	
	detach();

}

void Platform::special_destroy() {
	for (unsigned int i = 0; i < specials.size(); i++) {
		specials[i]->detach();
		//specials[i].~intrusive_ptr();
	}

	confetti_left->detach();
	confetti_right->detach();
	confetti_left = NULL;
	confetti_right = NULL;
	//confetti_left.~intrusive_ptr();
	//confetti_right.~intrusive_ptr();
	if (_dline.get() != NULL) {
		_dline->destroy();
		//_dline.~intrusive_ptr();
	}

	if (end_line.get() != NULL) {
		end_line->destroy();
	}
	if (_tpin.get() != NULL) {
		_tpin->detach();
		//_tpin.~intrusive_ptr();
	}

	world_ptr->getb2World()->DestroyBody(body);

}

void Platform::reset_platforms() {
	clean();
	create_platforms(restart_pos, start_threshold);
}

const std::vector<spPlatform> *Platform::getObjects() {
	return &objects;
}

Vector2 *Platform::get_first_platform_pos() {
	if (objects.size() > 0) {
		Vector2 pos = objects[0]->getPosition();
		return &pos;
	}
	else {
		return NULL;
	}
}

bool Platform::get_is_final_platform() {
	return is_final_platform;
}

void Platform::init(World* world_ptr) {
	can_create_platforms = true;
	final_x = world_ptr->get_boundary_left() / final_x_divisor;
	float lowest_height = sin(GF::rad(LAL)) * default_dist;
	float highest_height = sin(GF::rad(MAS)) * default_dist;
	average_height = GF::average(lowest_height, highest_height);
	restart_pos = Vector2(0, -200);
	how_many_platforms = 0;
}

void Platform::reset_platforms_except(Platform * ptr, float shift_height) {
	
	unsigned int how_many = 0;
	//This determines if it should be counting how_many or not.
	bool should_count = true;
	
	//This will clean up the platforms before the one given.
	while(1 < 2){

		if (objects[0].get() == ptr) {
			how_many++;
			should_count = false;
			break;
		}
		else {
			if (should_count) {
				how_many++;
			}
			objects[0]->destroy();
			//objects[0].~intrusive_ptr();
			objects.erase(objects.begin());
			
		}
		
	}
	world_ptr->increase_platform_counter(how_many);
	//Making it so that the class can now create platforms.
	can_create_platforms = true;
	how_many_platforms = 0;
	
	//Making new platforms
	
	int iter = 0;
	
	Vector2 prev_pos = ((ptr->getPosition()  - Vector2(8,8)));

	while (-1) {
		iter++;
		if (iter >= start_threshold - 1) {
			break;
		}
		else {
			
				//Creating a new platform, also updating the prev_pos variable to the new one.
				prev_pos = create_new_platform(prev_pos);
			
			

		}
	}

}

void Platform::fire_cannons() {
	confetti_left->set_is_active(true);
	confetti_right->set_is_active(true);
}

void Platform::fadeOut_and_destroy() {
	world_ptr->increase_platform_counter(1);
	spTween t = addTween(Actor::TweenAlpha(0), fadeOut_time);
	t->setDoneCallback(CLOSURE(this, &Platform::done_fading));
	_dline->fadeOut(fadeOut_time);
	_tpin->fadeOut(fadeOut_time);
	for (unsigned int i = 0; i < specials.size(); i++) {
		GF::fadeOut(specials[i].get(), fadeOut_time, 0);
	}
	//should_delete.push_back(this);
}

void Platform::done_fading(Event *ev) {
	//if (this->getParent()) {
	should_delete.push_back(this);

	//}
}

void Platform::stop_tweens() {
	//for (unsigned int i = 0; i < objects.size(); i++) {
	//	objects[i]->removeTweens();
	//}
}