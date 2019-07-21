#include "Background.h"
#include "ImageRes.h"
#include "Ship_M.h"
#include <iostream>
#include "GF.h"
#include <cmath>
#include "Hot_Balloon.h"
#define max_hot_balloon_altitude 10000
#define minimum_hot_balloon_altitude 500
#define sun_division_speed 20
#define max_hand_altitude 150
Color Background::stage_color;

void I_Hand::click_done(Event *ev) {
	spTweenQueue tq = new TweenQueue();
	if (!_is_reversed) {
		removeTweens();

		tq->add(Actor::TweenRotation(-1), 2000, 1, false, 0, Tween::EASE::ease_outExpo);
		tq->add(Actor::TweenRotation(0), 2000, 1, false, 0);
		tq->setDoneCallback(CLOSURE(this, &I_Hand::click_done));

	}
	else {
		removeTweens();
		tq->add(Actor::TweenRotation(1), 2000, 1, false, 0, Tween::EASE::ease_outExpo);
		tq->add(Actor::TweenRotation(0), 2000, 1, false, 0);
		tq->setDoneCallback(CLOSURE(this, &I_Hand::click_done));
	}
		
	addTween(tq);
}

I_Hand::I_Hand(const Vector2 &position, bool is_reversed) : _is_reversed(is_reversed) {
	setPosition(position);
	setAnchor(0.5f, 0.5f);
	
	setResAnim(res::resources.getResAnim("I_Hand"));
	if (is_reversed) {
		setFlippedX(true);
	}
	//spTweenQueue tq = new TweenQueue();
	//tq->add(addTween(Actor::TweenRotation(MATH_PI / 2), 2000, 1, false, 0));
	//tq->add(addTween(Actor::TweenRotation(-MATH_PI / 4), 2000, 1, false, 0));
	//tq->setDoneCallback(CLOSURE(this, &I_Hand::click_done));
	//addTween(tq);
	click_done(NULL);
}

void Volcano_controller::fade_in() {
	v1->fadeIn(this);
	v2->fadeIn(this);
}

void Volcano_controller::fade_out() {
	v1->fadeOut();
	v2->fadeOut();
}

void Volcano_controller::remove() {
	v1->d_check();
	v2->d_check();
}

bool Volcano_controller::get_is_active() {
	return v1->getAlpha();
}

Volcano_controller::Volcano_controller(const Vector2 &position) : start_pos(position) {
	setPosition(position);
	v1 = new Volcano(Vector2(0, getStage()->getHeight()));
	v2 = new Volcano(Vector2(getStage()->getWidth() / 2, getStage()->getHeight()));
	v1->setAlpha(0);
	v2->setAlpha(0);
	
}

void Sun::reset() {
	start_pos = init_sun_pos;
	setPosition(getStage()->getWidth() / 2, getStage()->getHeight());
	addTween(Actor::TweenPosition(Vector2(start_pos)), 3000, 1);
}

Sun::Sun(const Vector2 &position, unsigned int level) : init_sun_pos(position) {
	setResAnim(res::resources.getResAnim("Sun"));
	setPosition(position);
	setAnchor(0.5f, 0.5f);
	switch (level) {
	case 0:
		setColor(Color::Yellow);
		break;
	default:
		setColor(Color::Yellow);
		break;
	}
	init_color = getColor();
	setPriority(-20);
	addTween(Actor::TweenRotation(MATH_PI * 2), 4000, -1);
}

Cloud_Top::Cloud_Top(const Vector2 &pos) {
	start_pos = pos;
	setResAnim(res::resources.getResAnim("Clouds_Top"));
	setPosition(pos);
	setPriority(-10);
}

Cloud_Bottom::Cloud_Bottom(const Vector2 &pos) {
	start_pos = pos;
	setResAnim(res::resources.getResAnim("Clouds_Bottom"));
	setPosition(pos);
	setPriority(-10);
	
}

Background::Background(void* world_ptr) : has_won_game(&static_cast<World*>(world_ptr)->get_dwg_ptr()){
	_world_ptr = world_ptr;
	World* w_ptr = static_cast<World*>(world_ptr);
	//*has_won_game = 0;//w_ptr->get_dwg_ptr();
	float half = w_ptr->get_boundary_right();
	w_bound = half;
	rand_store_int.setup(50, -half, half);
	rand_store_int_vel.setup(50, 0, 20);
	//Creating the big bunny.
	big_bunny = new Bunny(Vector2(w_ptr->getWidth() / 4, w_ptr->getHeight() / 6));
	big_bunny->setAlpha(0);
	//addChild(big_bunny);
	cm = new C_Milk(Vector2(0, 0));
	cm->setAlpha(0);
	//addChild(cm);
	cm->setPriority(-1000);
	//_I_Hand_L = new I_Hand(Vector2(getStage()->getWidth() / 12, getStage()->getHeight() / 2));
	//addChild(_I_Hand_L);
	// = new I_Hand(Vector2(getStage()->getWidth() - (getStage()->getWidth() / 12), getStage()->getHeight() / 2), true);
	//addChild(_I_Hand_R);

	island_controller = new D_Islands(Vector2(0, 0), world_ptr);
	addChild(island_controller);
	island_controller->setPriority(-1000);
	volcano_controller = new Volcano_controller(Vector2(0, 0));
	addChild(volcano_controller);
	volcano_controller->setPriority(-1000);
	big_bunny->setPriority(-1000);
	rain_controller = new Rain(Vector2(0, 0), world_ptr, false);
	addChild(rain_controller);
	bubble_controller = new Bubble(Vector2(0, 0), world_ptr, false, bubble_type::bubble);
	addChild(bubble_controller);
	bubble_controller->setPriority(-15);
	rain_controller->setPriority(-15);
	sun = new Sun(Vector2(w_ptr->getWidth() / 2, w_ptr->getHeight()), 0);
	addChild(sun);
	sun_s_p = Vector2(w_ptr->getWidth() / 2, w_ptr->getHeight() - (w_ptr->getHeight() / 3));
	float b_height = res::resources.getResAnim("Clouds_Bottom")->getSize().y;
	cb = new Cloud_Bottom(Vector2(0, getStage()->getHeight() - b_height));
	ct = new Cloud_Top(Vector2(0, getStage()->getHeight() / 6));
	cb->attachTo(this);
	ct->attachTo(this);
	addChild(cb);
	addChild(ct);
	
	cb->setColor(Color(0, 0, 0, 255));
	ct->setColor(Color(0, 0, 0, 255));
	
	stage_color = Color(0, 0, 0, 255);
	cb_fade = new Fade_Color();
	stage_fade = new Fade_Color();
	sun_fade = new Fade_Color();
	setTheme(w_ptr->level, w_ptr->getworldNum());
	
}



void Background::createBalloon() {
	Vector2 pos(rand_store_int.getNum(), 0);
	spBalloon balloon = new Balloon(pos, -1000, 0);
	static_cast<World*>(_world_ptr)->addChild(balloon);
}

void Background::createHotBalloon() {
	World* w_ptr = static_cast<World*>(_world_ptr);
	random2::range(max_hot_balloon_altitude - minimum_hot_balloon_altitude, rand_store_int.getNum(), rand_store_int.getMinNum(), rand_store_int.getMaxNum());
	Vector2 pos(w_ptr->get_boundary_right(), -(abs(random2::range(max_hot_balloon_altitude - minimum_hot_balloon_altitude, rand_store_int.getNum(), rand_store_int.getMinNum(), rand_store_int.getMaxNum())) + minimum_hot_balloon_altitude));
	spHot_Balloon balloon = new Hot_Balloon(pos, static_cast<World*>(_world_ptr)->get_boundary_left() - res::resources.getResAnim("Hot_Air_Balloon")->getWidth(), 0);
	w_ptr->addChild(balloon);
}



void Background::doUpdate(const UpdateState &us) {
	
	if (*has_won_game == false) {
		switch (current_level) {
		case 0:
			wait_ticks += us.dt;

			if (wait_ticks >= max_wait_ticks) {
				createBalloon();
				wait_ticks -= max_wait_ticks;
			}
			break;
		case 1:
			wait_ticks += us.dt;

			if (wait_ticks >= max_wait_ticks) {
				createHotBalloon();
				wait_ticks -= max_wait_ticks;
			}
		}

		if (cb_fade->is_active()) {
			Color *c_ptr = &cb_fade->update(us);
			cb->setColor(*c_ptr);
			ct->setColor(*c_ptr);
			//stage_color = *c_ptr;

		}

		if (stage_fade->is_active()) {
			stage_color = stage_fade->update(us);
		}

		if (sun_fade->is_active()) {
			sun->setColor(sun_fade->update(us));
		}
		Vector2 dif_from_start = Ship_M::objects[0]->get_dif_from_start();

		switch (current_level) {
		case 0:
			if (dif_from_start.y > 0) { //This diference is below the starting position, this making it so that is will not move any futher.
				cb->setPosition(Vector2(0, cb->start_pos.y));
				ct->setPosition(Vector2(0, ct->start_pos.y));
				sun->setPosition(Vector2(sun->start_pos.x, sun->start_pos.y));


				if (volcano_controller->get_is_active()) {

					if (abs(dif_from_start.x) < w_bound) {
						volcano_controller->setPosition(Vector2(volcano_controller->start_pos.x - (dif_from_start.x / 14), volcano_controller->start_pos.y));
					}
					else {
						volcano_controller->setPosition(Vector2(volcano_controller->getX(), volcano_controller->start_pos.y));
					}

				}

				if (island_controller->get_is_active()) {
					island_controller->setPosition(Vector2(island_controller->start_pos.x, island_controller->start_pos.y));
				}
			}
			else {
				/*if (-dif_from_start.y < max_hand_altitude) {
					_I_Hand_L->setAlpha(255 - (255 * (-dif_from_start.y / max_hand_altitude)));
					_I_Hand_R->setAlpha(255 - (255 * (-dif_from_start.y / max_hand_altitude)));
				}
				else {
					_I_Hand_L->setAlpha(0);
					_I_Hand_R->setAlpha(0);
				}*/
				
				cb->setPosition(Vector2(0, cb->start_pos.y - (dif_from_start.y) / 7));
				ct->setPosition(Vector2(0, ct->start_pos.y - (dif_from_start.y) / 10));
				sun->setPosition(Vector2(sun->start_pos.x, sun->start_pos.y - (dif_from_start.y) / sun_division_speed));
				if (volcano_controller->get_is_active()) {
					if (abs(dif_from_start.x) < w_bound) {
						volcano_controller->setPosition(Vector2(volcano_controller->start_pos.x - (dif_from_start.x / 14), volcano_controller->start_pos.y - (dif_from_start.y) / 16));
					}
					else {
						volcano_controller->setPosition(Vector2(volcano_controller->getX(), volcano_controller->start_pos.y - (dif_from_start.y) / 16));
					}

				}

				if (island_controller->get_is_active()) {
					island_controller->setPosition(Vector2(0, island_controller->start_pos.y - (dif_from_start.y) / 16));
				}

			}
			break;
		case 1:
			if (dif_from_start.y > 0) { //This diference is below the starting position, this making it so that is will not move any futher.
				cb->setPosition(Vector2(0, cb->start_pos.y));
				ct->setPosition(Vector2(0, ct->start_pos.y));
				sun->setPosition(Vector2(sun->start_pos.x, sun->start_pos.y));
				cm->setPosition(cm->start_pos.x, cm->start_pos.y);
				if (big_bunny->getAlpha() != 0) {
					big_bunny->setPosition(big_bunny->getX(), big_bunny->start_pos.y);
				}

			}
			else {
				cb->setPosition(Vector2(0, cb->start_pos.y - (dif_from_start.y) / 2));
				ct->setPosition(Vector2(0, ct->start_pos.y - (dif_from_start.y) / 3));
				sun->setPosition(Vector2(sun->start_pos.x, sun->start_pos.y - (dif_from_start.y) / sun_division_speed));
				cm->setPosition(cm->start_pos.x, cm->start_pos.y - (dif_from_start.y / 16));
				if (big_bunny->getAlpha() != 0) {
					big_bunny->setPosition(Vector2(big_bunny->getX(), big_bunny->start_pos.y - (dif_from_start.y) / 20));
				}


			}
			break;

		case 2:
			if (Ship_M::objects[0]->get_dif_from_start().y > 0) { //This diference is below the starting position, this making it so that is will not move any futher.
				cb->setPosition(Vector2(0, cb->start_pos.y));
				ct->setPosition(Vector2(0, ct->start_pos.y));
				sun->setPosition(Vector2(sun->start_pos.x, sun->start_pos.y));
			}
			else {
				cb->setPosition(Vector2(0, cb->start_pos.y - (Ship_M::objects[0]->get_dif_from_start().y) / 23));
				ct->setPosition(Vector2(0, ct->start_pos.y - (Ship_M::objects[0]->get_dif_from_start().y) / 23));
				sun->setPosition(Vector2(sun->start_pos.x, sun->start_pos.y - (dif_from_start.y) / sun_division_speed));
			}
		}
	}
	
	
	
	
}

void Background::setTheme(unsigned int level, unsigned int world) {
	World* w_ptr = static_cast<World*>(_world_ptr);
	//ct->setAlpha(255);
	//cb->setAlpha(255);
	//Turning everything off.
	w_ptr->wind_controller.set_activity(false);
	Balloon::clear_all();
	Hot_Balloon::clear_all();
	rain_controller->set_activity(false);
	bubble_controller->set_activity(false);
	
	current_level = level;
	//rgb
	switch (world) {
	case 0:
		switch (level) {
		case 0: //Initilization fade in.
			
			volcano_controller->fade_out();
			island_controller->fadeOut();
			big_bunny->fadeOut();
			//big_bunny->fadeIn(this);
			cm->fadeOut();
			ct->setResAnim(res::resources.getResAnim("Clouds_Top"));
			cb->setResAnim(res::resources.getResAnim("Clouds_Bottom"));
			ct->setColor(Color(0, 0, 0, 255));
			cb->setColor(Color(0, 0, 0, 255));
			cb->start_pos = Vector2(0, getStage()->getHeight() - cb->getHeight());
			ct->start_pos = Vector2(0, getStage()->getHeight() / 6);
			sun->start_pos = sun_s_p;
			cb_fade->setup(Vector4(cb->getColor().r, cb->getColor().g, cb->getColor().b, cb->getColor().a), Vector4(255, 255, 255, 255));
			stage_fade->setup(Vector4(stage_color.r, stage_color.g, stage_color.b, stage_color.a), Vector4(32, 130, 212, 255));
			sun_fade->setup(Vector4(sun->getColor().r, sun->getColor().g, sun->getColor().b, sun->getColor().a), Vector4(255, 252, 40, 255));
			break;
		case 1: //Sunset level
			//big_bunny->fadeOut();
			cb->setResAnim(res::resources.getResAnim("Clouds_Top2"));
			ct->setResAnim(res::resources.getResAnim("Clouds_Top3"));
			cb->start_pos = Vector2(0, -345 - (cb->getHeight() / 2));
			ct->start_pos = Vector2(0, -(345 * 2) - (ct->getHeight() / 2));
			sun->start_pos = Vector2(sun_s_p.x, sun->getPosition().y);
			cb->setPosition(-getStage()->getWidth(), 0);
			ct->setPosition(-getStage()->getWidth(), 0);
			stage_fade->setup(Vector4(stage_color.r, stage_color.g, stage_color.b, stage_color.a), Vector4(231, 207, 75, 255));
			cb_fade->setup(Vector4(cb->getColor().r, cb->getColor().g, cb->getColor().b, cb->getColor().a), Vector4(232, 166, 63, 255));
			sun_fade->setup(Vector4(sun->getColor().r, sun->getColor().g, sun->getColor().b, sun->getColor().a), Vector4(255, 0, 0, 255));
			break;
		case 2:
			cb->setResAnim(res::resources.getResAnim("Space_Bg2"));
			ct->setResAnim(res::resources.getResAnim("Space_Bg2"));
			cb->start_pos = Vector2(0, 0);
			ct->start_pos = Vector2(0, -cb->getHeight());
			cb->setPosition(-getStage()->getWidth(), 0);
			ct->setPosition(-getStage()->getWidth(), 0);
			//Making the stars completely invisible until the background fades to the right color.
			cb->setColor(0, 0, 0, 0);
			ct->setColor(0, 0, 0, 0);
			
			sun->start_pos = Vector2(sun_s_p.x, sun->getPosition().y);
			sun_fade->setup(Vector4(sun->getColor().r, sun->getColor().g, sun->getColor().b, sun->getColor().a), Vector4(255, 0, 0, 0));
			std::function<void()> lam = [&]() {
				std::function<void()> lam2 = [&]() {
					sun->setColor(0, 0, 0, 0);
				};
				cb_fade->setup(Vector4(255, 255, 255, 0), Vector4(255, 255, 255, 255), lam2);

			};

			stage_fade->setup(Vector4(stage_color.r, stage_color.g, stage_color.b, stage_color.a), Vector4(0, 0, 0, 255), lam);
		
		}
		break;
	case 1:
		switch (level) {
		case 0: //Initilization fade in.
			rain_controller->set_activity(true, type::rain);
			ct->setResAnim(res::resources.getResAnim("Clouds_Top"));
			cb->setResAnim(res::resources.getResAnim("Clouds_Bottom"));
			
			cb->start_pos = Vector2(0, getStage()->getHeight() - cb->getHeight());
			ct->start_pos = Vector2(0, getStage()->getHeight() / 6);
			cb->setColor(0, 0, 0, 0);
			ct->setColor(0, 0, 0, 0);
			sun->setColor(0, 0, 0, 0);
			cb->setPosition(-getStage()->getWidth(), 0);
			ct->setPosition(-getStage()->getWidth(), 0);
			sun->reset();
			cb_fade->setup(Vector4(cb->getColor().r, cb->getColor().g, cb->getColor().b, 0), Vector4(25, 25, 25, 255));
			stage_fade->setup(Vector4(stage_color.r, stage_color.g, stage_color.b, stage_color.a), Vector4(40, 40, 40, 255));
			//sun->setColor(0, 0, 0, 0);
			sun_fade->setup(Vector4(sun->getColor().r, sun->getColor().g, sun->getColor().b, sun->getColor().a), Vector4(255, 252, 40, 255));
			break;
		case 1: //Sunset level
			w_ptr->wind_controller.set(b2Vec2(-60, 0), true);
			bubble_controller->set_activity(true, bubble_type::bubble);
			cb->setResAnim(res::resources.getResAnim("Clouds_Top2"));
			ct->setResAnim(res::resources.getResAnim("Clouds_Top3"));
			cb->start_pos = Vector2(0, -345 - (cb->getHeight() / 2));
			ct->start_pos = Vector2(0, -(345 * 2) - (ct->getHeight() / 2));
			sun->start_pos = Vector2(sun_s_p.x, sun->getPosition().y);
			cb->setPosition(-getStage()->getWidth(), 0);
			ct->setPosition(-getStage()->getWidth(), 0);
			stage_fade->setup(Vector4(stage_color.r, stage_color.g, stage_color.b, stage_color.a), Vector4(231, 207, 75, 255));
			cb_fade->setup(Vector4(cb->getColor().r, cb->getColor().g, cb->getColor().b, cb->getColor().a), Vector4(232, 166, 63, 255));
			sun_fade->setup(Vector4(sun->getColor().r, sun->getColor().g, sun->getColor().b, sun->getColor().a), Vector4(255, 0, 0, 255));
			break;
		case 2:
			cb->setResAnim(res::resources.getResAnim("Space_Bg2"));
			ct->setResAnim(res::resources.getResAnim("Space_Bg2"));
			cb->start_pos = Vector2(0, 0);
			ct->start_pos = Vector2(0, -cb->getHeight());
			//Making the stars completely invisible until the background fades to the right color.
			cb->setColor(0, 0, 0, 0);
			ct->setColor(0, 0, 0, 0);
			cb->setPosition(-getStage()->getWidth(), 0);
			ct->setPosition(-getStage()->getWidth(), 0);
			sun->start_pos = Vector2(sun_s_p.x, sun->getPosition().y);
			sun_fade->setup(Vector4(sun->getColor().r, sun->getColor().g, sun->getColor().b, sun->getColor().a), Vector4(255, 0, 0, 255));
			std::function<void()> lam = [&]() {
				std::function<void()> lam2 = [&]() {
					sun->setColor(0, 0, 0, 0);
				};
				cb_fade->setup(Vector4(255, 255, 255, 0), Vector4(255, 255, 255, 255), lam2);
			};

			stage_fade->setup(Vector4(stage_color.r, stage_color.g, stage_color.b, stage_color.a), Vector4(0, 0, 0, 255), lam);
		}
		break;
	case 2:
		switch (level) {
		case 0: //Initilization fade in.
			rain_controller->set_activity(true, type::snow);
			ct->setResAnim(res::resources.getResAnim("Clouds_Top"));
			cb->setResAnim(res::resources.getResAnim("Clouds_Bottom"));
			
			cb->start_pos = Vector2(0, getStage()->getHeight() - cb->getHeight());
			ct->start_pos = Vector2(0, getStage()->getHeight() / 6);
			cb->setPosition(-getStage()->getWidth(), 0);
			ct->setPosition(-getStage()->getWidth(), 0);
			cb->setColor(0, 0, 0, 0);
			ct->setColor(0, 0, 0, 0);
			cb->setPosition(-getStage()->getWidth(), 0);
			ct->setPosition(-getStage()->getWidth(), 0);
			sun->reset();
			cb_fade->setup(Vector4(cb->getColor().r, cb->getColor().g, cb->getColor().b, 0), Vector4(242, 219, 255, 255));
			stage_fade->setup(Vector4(stage_color.r, stage_color.g, stage_color.b, stage_color.a), Vector4(243, 204, 204, 255));
			sun_fade->setup(Vector4(sun->getColor().r, sun->getColor().g, sun->getColor().b, 0), Vector4(255, 252, 40, 255));
			break;
		case 1: //Sunset level
			//Local variable initialization scope encasing.
			//These brackets allow the program to initialize local variables inside the switch statement.
		{
			
			
			cb->setResAnim(res::resources.getResAnim("Clouds_Top2"));
			ct->setResAnim(res::resources.getResAnim("Clouds_Top3"));
			cb->start_pos = Vector2(0, -345 - (cb->getHeight() / 2));
			ct->start_pos = Vector2(0, -(345 * 2) - (ct->getHeight() / 2));
			sun->start_pos = Vector2(sun_s_p.x, sun->getPosition().y);
			cb->setPosition(-getStage()->getWidth(), 0);
			ct->setPosition(-getStage()->getWidth(), 0);
			std::function<void()> lam = [&]() {
				big_bunny->fadeIn(this);
			};

			stage_fade->setup(Vector4(stage_color.r, stage_color.g, stage_color.b, stage_color.a), Vector4(199, 123, 243, 255), lam);
			cb_fade->setup(Vector4(cb->getColor().r, cb->getColor().g, cb->getColor().b, cb->getColor().a), Vector4(232, 193, 255, 255));
			sun_fade->setup(Vector4(sun->getColor().r, sun->getColor().g, sun->getColor().b, sun->getColor().a), Vector4(231, 87, 199, 255));
			break;
		}
		break;
		case 2:
		{
			
			big_bunny->fadeOut();
			cb->setResAnim(res::resources.getResAnim("Space_Bg2"));
			ct->setResAnim(res::resources.getResAnim("Space_Bg2"));
			cb->start_pos = Vector2(0, 0);
			ct->start_pos = Vector2(0, -cb->getHeight());
			//Making the stars completely invisible until the background fades to the right color.
			cb->setColor(0, 0, 0, 0);
			ct->setColor(0, 0, 0, 0);
			cb->setPosition(-getStage()->getWidth(), 0);
			ct->setPosition(-getStage()->getWidth(), 0);
			sun->start_pos = Vector2(sun_s_p.x, sun->getPosition().y);
			sun_fade->setup(Vector4(sun->getColor().r, sun->getColor().g, sun->getColor().b, sun->getColor().a), Vector4(255, 0, 0, 255));
			std::function<void()> lam = [&]() {
				std::function<void()> lam2 = [&]() {
					sun->setColor(0, 0, 0, 0);
				};
				cb_fade->setup(Vector4(255, 255, 255, 0), Vector4(255, 255, 255, 255), lam2);
			};

			stage_fade->setup(Vector4(stage_color.r, stage_color.g, stage_color.b, stage_color.a), Vector4(0, 0, 0, 255), lam);
		}
		}
		
		break;
	case 3:
		switch (level) {
		case 0: //Initilization fade in.
			rain_controller->set_activity(true, type::ash);
			
			volcano_controller->fade_in();
			ct->setResAnim(res::resources.getResAnim("Clouds_Top"));
			cb->setResAnim(res::resources.getResAnim("Clouds_Bottom"));
			
			cb->start_pos = Vector2(0, getStage()->getHeight() - cb->getHeight());
			ct->start_pos = Vector2(0, getStage()->getHeight() / 6);
			cb->setColor(0, 0, 0, 0);
			ct->setColor(0, 0, 0, 0);
			cb->setPosition(-getStage()->getWidth(), 0);
			ct->setPosition(-getStage()->getWidth(), 0);
			sun->reset();
			cb_fade->setup(Vector4(cb->getColor().r, cb->getColor().g, cb->getColor().b, 0), Vector4(104, 10, 10, 140));
			stage_fade->setup(Vector4(stage_color.r, stage_color.g, stage_color.b, stage_color.a), Vector4(58,58,58, 255));
			sun_fade->setup(Vector4(sun->getColor().r, sun->getColor().g, sun->getColor().b, 0), Vector4(40, 40, 40, 255));
			break;
		case 1: //Sunset level
				//Local variable initialization scope encasing.
				//These brackets allow the program to initialize local variables inside the switch statement.
		{
			
			
			volcano_controller->fade_out();
			w_ptr->wind_controller.set(b2Vec2(-60, 0), true);
			bubble_controller->set_activity(true, bubble_type::flower);
			cm->fadeIn(this);



			cb->setResAnim(res::resources.getResAnim("Clouds_Top2"));
			ct->setResAnim(res::resources.getResAnim("Clouds_Top3"));
			cb->start_pos = Vector2(0, -345 - (cb->getHeight() / 2));
			ct->start_pos = Vector2(0, -(345 * 2) - (ct->getHeight() / 2));
			sun->start_pos = Vector2(sun_s_p.x, sun->getPosition().y);
			cb->setPosition(-getStage()->getWidth(), 0);
			ct->setPosition(-getStage()->getWidth(), 0);
			std::function<void()> lam = [&]() {

			};

			stage_fade->setup(Vector4(stage_color.r, stage_color.g, stage_color.b, stage_color.a), Vector4(156, 255, 154, 255), lam);
			cb_fade->setup(Vector4(cb->getColor().r, cb->getColor().g, cb->getColor().b, cb->getColor().a), Vector4(80, 225, 76, 160));
			sun_fade->setup(Vector4(sun->getColor().r, sun->getColor().g, sun->getColor().b, sun->getColor().a), Vector4(231, 87, 199, 255));
			break;
			
			
		}

		case 2:
		{
			cm->fadeOut();
			cb->setResAnim(res::resources.getResAnim("Space_Bg2"));
			ct->setResAnim(res::resources.getResAnim("Space_Bg2"));
			cb->start_pos = Vector2(0, 0);
			ct->start_pos = Vector2(0, -cb->getHeight());
			//Making the stars completely invisible until the background fades to the right color.
			cb->setColor(0, 0, 0, 0);
			ct->setColor(0, 0, 0, 0);
			cb->setPosition(-getStage()->getWidth(), 0);
			ct->setPosition(-getStage()->getWidth(), 0);
			sun->start_pos = Vector2(sun_s_p.x, sun->getPosition().y);
			sun_fade->setup(Vector4(sun->getColor().r, sun->getColor().g, sun->getColor().b, sun->getColor().a), Vector4(255, 0, 0, 255));
			std::function<void()> lam = [&]() {
				std::function<void()> lam2 = [&]() {
					sun->setColor(0, 0, 0, 0);
				};
				cb_fade->setup(Vector4(255, 255, 255, 0), Vector4(255, 255, 255, 255), lam2);
			};

			stage_fade->setup(Vector4(stage_color.r, stage_color.g, stage_color.b, stage_color.a), Vector4(0, 0, 0, 255), lam);
		}
		}
		break;
	case 4:
		switch (level) {
		case 0: //Initilization fade in.
			
			island_controller->setup(true);
			//rain_controller->set_activity(true, type::ash);

			//volcano_controller->fade_in();
			ct->setResAnim(res::resources.getResAnim("Clouds_Top"));
			cb->setResAnim(res::resources.getResAnim("Clouds_Bottom"));
			
			cb->start_pos = Vector2(0, getStage()->getHeight() - cb->getHeight());
			ct->start_pos = Vector2(0, getStage()->getHeight() / 6);
			cb->setColor(0, 0, 0, 0);
			ct->setColor(0, 0, 0, 0);
			cb->setPosition(-getStage()->getWidth(), 0);
			ct->setPosition(-getStage()->getWidth(), 0);
			sun->reset();
			cb_fade->setup(Vector4(cb->getColor().r, cb->getColor().g, cb->getColor().b, 0), Vector4(159, 127, 183, 255));
			stage_fade->setup(Vector4(stage_color.r, stage_color.g, stage_color.b, stage_color.a), Vector4(70, 93, 133, 255));
			sun_fade->setup(Vector4(sun->getColor().r, sun->getColor().g, sun->getColor().b, 0), Vector4(255, 255, 255, 255));
			break;
		case 1: //Sunset level
				//Local variable initialization scope encasing.
				//These brackets allow the program to initialize local variables inside the switch statement.
		{
			

			island_controller->fadeOut();
			w_ptr->wind_controller.set(b2Vec2(-60, 0), true);
			bubble_controller->set_activity(true, bubble_type::rainbow_bubble);
			



			cb->setResAnim(res::resources.getResAnim("Clouds_Top2"));
			ct->setResAnim(res::resources.getResAnim("Clouds_Top3"));
			cb->start_pos = Vector2(0, -345 - (cb->getHeight() / 2));
			ct->start_pos = Vector2(0, -(345 * 2) - (ct->getHeight() / 2));
			sun->start_pos = Vector2(sun_s_p.x, sun->getPosition().y);
			cb->setPosition(-getStage()->getWidth(), 0);
			ct->setPosition(-getStage()->getWidth(), 0);
			std::function<void()> lam = [&]() {

			};

			stage_fade->setup(Vector4(stage_color.r, stage_color.g, stage_color.b, stage_color.a), Vector4(246, 208, 228, 255), lam);
			cb_fade->setup(Vector4(cb->getColor().r, cb->getColor().g, cb->getColor().b, cb->getColor().a), Vector4(255, 66, 164, 255));
			sun_fade->setup(Vector4(sun->getColor().r, sun->getColor().g, sun->getColor().b, sun->getColor().a), Vector4(255, 200, 200, 255));
			break;


		}
		
		case 2:
		{
			
			cb->setResAnim(res::resources.getResAnim("Space_Bg"));
			ct->setResAnim(res::resources.getResAnim("Space_Bg"));
			cb->start_pos = Vector2(0, 0);
			ct->start_pos = Vector2(0, -cb->getHeight());
			//Making the stars completely invisible until the background fades to the right color.
			cb->setColor(0, 0, 0, 0);
			ct->setColor(0, 0, 0, 0);
			sun->start_pos = Vector2(sun_s_p.x, sun->getPosition().y);
			cb->setPosition(-getStage()->getWidth(), 0);
			ct->setPosition(-getStage()->getWidth(), 0);
			sun_fade->setup(Vector4(sun->getColor().r, sun->getColor().g, sun->getColor().b, sun->getColor().a), Vector4(255, 0, 0, 255));
			std::function<void()> lam = [&]() {
				std::function<void()> lam2 = [&]() {
					sun->setColor(0, 0, 0, 0);
				};
				cb_fade->setup(Vector4(255, 255, 255, 0), Vector4(255, 255, 255, 255), lam2);
			};

			stage_fade->setup(Vector4(stage_color.r, stage_color.g, stage_color.b, stage_color.a), Vector4(0, 0, 0, 255), lam);
		}
		}
	}

	
	
	
}

void Background::pre_init() {
	stage_color = Color::DeepSkyBlue;
	
}

Color *Background::getStage_Color() {
	return &stage_color;
}

unsigned int Background::getTheme() {
	return current_level;
}