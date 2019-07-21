#include "World.h"
#include "Wall.h"
#include "Box2DDebugDraw.h"
#include "Ship_M.h"
#include "Ship_Part_M.h"
#include "Platform.h"
#include "Contact_Listener.h"
#include "Chainsaw.h"
#include <ctime>
#include "random2.h"
#include "Black_hole.h"
#include "Temple.h"
#include "ST.h"
#include "Particle.h"
#include "P_System.h"
#include "Balloon.h"
#include "Hot_Balloon.h"
#include "Rain.h"
#include "Noise.h"
#include "Bird_Nest.h"
#include "Final_Bird.h"
#include <functional>
#include "TPI_Line.h"
#define detach_line 50 //This is how far the line that detachs actors is.
#define cam_xlimits 700 //This counts in the negative direction as well.
#define gravity 1000//450//5//800//450
#define max_levels 3
#define max_number_of_worlds 4 //This is the total number of worlds that exist.
std::vector<Actor*> World::to_shift;
Vector2 pos123;
World::World() {
	level = 0;
	worldNum = 0;
	
	//Setting up the wind controller.
	wind_controller = Wind(b2Vec2(10,0), true);
	
	//set size of this object to the size of the window.
	setSize(getStage()->getSize());
	
	Platform::init(this);

	//Initializing the tiles.
	Tile_M::init(this);
	
	random2::fast_srand(static_cast<int>(time(NULL)));
	Temple::reset_cache();
	//create the physics world
	world = new b2World(b2Vec2(0,gravity));
	Particle::init(this);
	bg = new Background(this);
	bg->attachTo(getStage());
	//win_game();

	Ship_Part_M::init(this);
	//Ship_M adds itself as a child to the world in its constructor. No need to do it out here.
	
	//spShip_M player2 = new Ship_M(this, Vector2(0, -60), Ship_M::control_types::arrow_l_r);
	
	
	
	//addChild(player);
	//spChainsaw cs = new Chainsaw(Vector2(500, -150), 150, 13);
	//addChild(cs);
	
	Contact_Listener *cl = new Contact_Listener();
	world->SetContactListener(cl);
	
	
	gui = new GUI(&level, Ship_M::allocate(this).get(), this);
	
	getStage()->addChild(gui);
	create_blocks();
	//_debugDraw = new Box2DDraw;
	//_debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit );
	//_debugDraw->attachTo(this);
	//_debugDraw->setWorld(GF::get_G_Scale(), world);
	//_debugDraw->setPriority(-30000);
}

void World::create_blocks() {
	
	//pos123 = Vector2(-80, 0);
	//create_wall(Vector2(0, ground_level), Vector2(block_size * 10, block_size));
	//create_wall(Vector2(block_size * 12, ground_level), Vector2(block_size * 2, block_size));
	//create_wall(Vector2(block_size * 12, block_size * 1), )
	//spTPI_Line tl = new TPI_Line(pos123, Vector2(-80,-150));
	//addChild(tl);
	//spBalloon b = new Balloon(Vector2(0,-150), 500, 0);
	//addChild(b);
	//spChainsaw b = new Chainsaw(Vector2(-250, -330), 5, 30);
	//addChild(b);
	//spBlack_hole bh = new Black_hole(Vector2(-100, -100), 50, this, 2000);
	//addChild(bh);
	//spHot_Balloon hb = new Hot_Balloon(Vector2(0, -9000), get_boundary_left(), 0);
	//addChild(hb);

	//spBird_Nest bn = new Bird_Nest(Vector2(0, -500), this);
	
	//spFinal_Bird fb;
	//fb = new Final_Bird(Vector2(0, -500), this);
	//addChild(fb);
	
	//spT_Sign ts;
	//ts = new T_Sign(Vector2(0, -300));
	//addChild(ts);
	//spRaindrop rd = new Raindrop(Vector2(0, -600), Vector2(0.04, 0.04));
	//addChild(rd);
	//spST spin = new ST(Vector2(0, -900), 0);
	//addChild(spin);

	//spParticle particle = new Particle(Vector2(0, -300), Vector2(350, 350), 0);
	//addChild(particle);

	//spP_System p_sys = new P_System(Vector2(0, -300), 3.14 + 1, 2000, 0, this);
	//addChild(p_sys);
	//spTemple t = new Temple(-1200, this);
	//addChild(t);
	//create_wall(RectF(0,0,getStage()->getWidth(),64));
	//create_platform(Vector2(0, -300), 10);
	Platform::create_platforms(Vector2(0,-200), 3);
	ground = new Ground(Vector2(0, 100), this, 0);
	//addChild(ground);
	//spBlack_hole bh = new Black_hole(Vector2(199, -268), 20);
	//addChild(bh);
	//create_wall(Vector2(5,-1), Vector2(3,1));
	/*create_wall(Vector2(11,-1), Vector2(3,1));
	create_wall(Vector2(16,-2), Vector2(1,-5));
	create_wall(Vector2(15,-4), Vector2(2,1));
	create_wall(Vector2(15,-7), Vector2(8,1));
	create_wall(Vector2(10,-4), Vector2(3,1));
	create_wall(Vector2(22,-7), Vector2(1,-2));
	create_wall(Vector2(26,-9), Vector2(5,1));*/
	//create_wall(Vector2(), Vector2());
	//create_wall(Vector2(), Vector2());
	//create_wall(Vector2(), Vector2());
	//create_wall(Vector2(), Vector2());
	//create_wall(Vector2(), Vector2());
	
}

void World::create_wall(RectF &rect) {
	
	//rect.setY(rect.getY() + getStage()->getHeight() - 64);
	//spWall wall = 
	addChild(new Wall(this, rect, 0));
	
}

World::~World() {

}

void World::doUpdate(const UpdateState &us) {
	Platform::tick();

	//pos123.y -= us.dt;
	if(!is_paused){
		world->Step(FIXED_TIMESTEP / us.dt, 6, 2);
	}
	
	if (_did_win_game) {
		if_game_win();
	}
	
	
	b2Body* body = world->GetBodyList();
	std::vector<Actor*> to_delete;
	while (body) {
		Actor* actor = (Actor*)body->GetUserData();
		b2Body* next = body->GetNext();
		if (actor) {
			if (!get_is_paused()) {
				wind_controller.simulate(body);
			}
			
			if (actor->getPosition().y < detach_line) {
				const b2Vec2& pos = body->GetPosition();
				actor->setPosition(GF::convert(pos));
				actor->setRotation(body->GetAngle());
			}
			else {
				const b2Vec2& pos = body->GetPosition();
				actor->setPosition(GF::convert(pos));
				actor->setRotation(body->GetAngle());
				if (actor->getName() == "PM" || actor->getName() == "EM") {
					actor->detach();
					
				}
				
			}
		
			
			
		}

		body = next;
	}
	if (should_reset_all == true) {
		RESET_ALL();
		should_reset_all = false;
	}
	/*for (unsigned int i = 0; i < to_delete.size(); i++) {

		to_delete[i]->detach();
	}*/
	if (Platform::objects.size() == 3 || Platform::objects.size() == 4) {
		switch (rti) {
		case 0: //In the case that you want a standard update.

			if (level >= max_levels) {

				iterate_to_next_world();
			}
			else {

				iterate_to_next_level();
			}


			rti = -1;
			break;
		case 1:


			level++;


			//shift_children(new_height);

			//shift_children(new_height);
			Platform::reset_platforms_except(static_cast<Platform*>(platform_ptr), new_height);
			if (level >= max_levels) {

				iterate_to_next_world();
			}
			else {

				world->ShiftOrigin(GF::convert(Vector2(0, new_height)));
				//shift_children(new_height);
				//level++;
				setTheme(level, worldNum);
				//world->ShiftOrigin(GF::convert(Vector2(0, new_height)));
				//setTheme(level);



			}
			rti = -1;
		}
	}
	

	
	/*if (rti) {
	iterate_to_next_level();
	rti = 0;
	}*/
	for (unsigned int i = 0; i < d_cache.size(); i++) {

		static_cast<Actor*>(d_cache[i]->GetUserData())->detach();


	}
	d_cache.clear();
	
	
}



b2World * World::getb2World() {
	return world;
}

void World::create_platform(const Vector2 &pos, int width) {
	//spPlatform pl = new Platform(pos, width);
	//addChild(pl);
}

void World::reset_platforms() {
	Platform::reset_platforms();
}


void World::set_cam_pos(const Vector2 &pos) {
	
	
	Vector2 set_pos;

	if (pos.x >= cam_xlimits) {
		set_pos.x = cam_xlimits;
	}
	else if (pos.x - getStage()->getWidth() <= -cam_xlimits) {
		set_pos.x = (-cam_xlimits) + getStage()->getWidth();
	}
	else {
		set_pos.x = pos.x;
	}
	
	if (pos.y - getStage()->getHeight() <= cam_bottom_ylimits) {
		set_pos.y = cam_bottom_ylimits + getStage()->getHeight();
	}
	else {
		set_pos.y = pos.y;
	}
	
	setPosition(set_pos);

}

bool World::is_within_boundary(const Vector2 &check_pos, const Vector2 &threshold) {
	if (check_pos.x >= cam_xlimits - threshold.x) {
		return false;
	
	}
	else if (check_pos.x <= -cam_xlimits + threshold.x) {
		return false;
		
	}
	else {
		return true;
	}

	if (check_pos.y <= cam_bottom_ylimits + threshold.y) {
		return false;
	}
	else {
		return true;
	}
}

float World::get_boundary_left() {
	return -cam_xlimits;
}

float World::get_boundary_right() {
	return cam_xlimits;
}

float World::get_boundary_size() {
	return cam_xlimits * 2;
}

void World::iterate_to_next_level() {
	
	world->ShiftOrigin(GF::convert(Vector2(0, new_height)));
	cam_bottom_ylimits = 0;
	shift_children(new_height);
	setTheme(level, worldNum);
	
}

//This will increase the level by one, and load up the new world.
//WARNING: INPUT NEW_HEIGHT AS OXY VECTOR TYPE, NOT B2.
void World::ready_to_iterate(float New_Height, void* pltfrm_ptr) {
	level++;
	if (pltfrm_ptr == NULL) {
		rti = 0;
	}
	else {
		rti = 1;
		platform_ptr = pltfrm_ptr;
	}
	
	new_height = New_Height;
}



void World::shift_children(float y_axis_amount) {
	/*b2Body* body = world->GetBodyList();
	std::vector<Actor*> to_delete;
	while (body) {
		b2Body* next = body->GetNext();
		body->SetTransform(b2Vec2(body->GetPosition().x, body->GetPosition().y + GF::convert_to_b2(y_axis_amount)), body->GetAngle());
		body = next;
	}*/

	
	Actor* next_actor = getFirstChild().get();
	bool exit = false;
	while (exit == false) {
		
		if (next_actor == getLastChild()) {
			exit = true;
		}
		//if (next_actor->getName() == "Templefef") {
		//	log::messageln("Shifted!!!");
		//}
		//if (next_actor->getPosition().y > 0) {
		next_actor->setPosition(Vector2(next_actor->getPosition().x, next_actor->getPosition().y - y_axis_amount));
		if (exit != true) {
			next_actor = next_actor->getNextSibling().get();

		}
		
		//}
		
		
	}
	
	/*for (unsigned int i = 0; i < to_shift.size(); i++) {
		Actor* ptr = to_shift[i];
		if (ptr != nullptr) {
			ptr->setPosition(Vector2(ptr->getPosition().x, ptr->getPosition().y - y_axis_amount));
		}
		else {
			throw("Error!");
			to_shift.erase(to_shift.begin() + i);
			i--;
		}
	}*/
}

void World::increase_platform_counter(int addend) {
	gui->increase_p_counter(addend);
}

void World::RESET_ALL() {
	level = 0;
	worldNum = 0;
	Temple::reset_cache();
	//spChainsaw cs = new Chainsaw(Vector2(0, -400), 10, 25);
	//addChild(cs);
	cam_bottom_ylimits = 0;
	//ground->setPosition(Vector2(0, 0));
	//spShip_M player = new Ship_M(this, Vector2(0, -60), Ship_M::control_types::a_d);
	setTheme(0, 0);
	
	reset_platforms();
	
	for (unsigned int i = 0; i < Ship_M::objects.size(); i++) {
		Ship_M::objects[i]->reset();
	}
	
	//Creating a new ground object.
	world->DestroyBody(ground->body);
	ground->destroy();
	ground->detach();
	
	ground = new Ground(Vector2(0, 0), this);
	
	
	gui->reset();
	
}

void World::set_RESET_ALL() {
	should_reset_all = true;
}

void World::setTheme(unsigned int new_level, unsigned int world) {
	level = new_level;
	worldNum = world;
	
	//gui->set_pc_color(Color(Color::Purple));
	bg->setTheme(level, world);
	
}

unsigned int World::getworldNum() {
	return worldNum;
}

void World::pause() {
	Noise::sfx_player.pause();
	//Ship_M::objects[0]->fade_out_engine_sound();
	is_paused = true;
}

bool World::get_is_paused() {
	return is_paused;
}

void World::unpause() {
	Noise::sfx_player.resume();
	is_paused = false;
}

void World::pause_toggle() {
	if (is_paused) {
		unpause();
	}
	else {
		pause();
	}
}


void World::iterate_to_next_world() {
	level = 0;
	worldNum++;
	gui->set_world_counter(worldNum);
	world->ShiftOrigin(GF::convert(Vector2(0, new_height)));
	cam_bottom_ylimits = 0;
	shift_children(-new_height);
	setTheme(level, worldNum);
	setup_newworld(worldNum);
}

void World::setup_newworld(unsigned int world) {

	//Creating a new ground object.
	getb2World()->DestroyBody(ground->body);
	ground->destroy();
	ground->detach();
	ground = new Ground(Vector2(0, 0), this, world);
	
}

void World::shift_pos(Vector2 &pos) {
	//pos.y -= new_height;
}

bool World::is_final_world() {
	if (worldNum >= max_number_of_worlds) {
		return true;
	}
	else {
		return false;
	}
}

bool World::is_final_level() {
	if (level >= max_levels - 1) {
		return true;
	}
	else {
		return false;
	}
}

bool World::did_win_game() {
	return _did_win_game;
}

void World::win_game() {
	_did_win_game = true;
}

void World::if_game_win() {
	if (!has_win_function_called) {
		
		
		gui->fadeOut(CLOSURE(this, &World::done_fading_win));
		
		has_win_function_called = true;
	}
	
	
	
}

bool &World::get_dwg_ptr() {
	return _did_win_game;
}

void World::done_fading_win(Event *ev) {
	//Platform::clean();
	//gui->pause();
	//Ship_M::deallocate();
	//Particle::clear();
	gui->open_win_menu(this);
}

void World::stop_winning() {
	_did_win_game = false;
}

void World::insert_shifter(Actor* to_insert) {
	to_shift.push_back(to_insert);
}

unsigned int World::getLevel() {
	return level;
}