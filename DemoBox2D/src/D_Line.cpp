#include "D_Line.h"
#include "ImageRes.h"
#include <cmath>
#include "Chainsaw.h"
#include "random2.h"
#include "Black_hole.h"
#include "ForceField.h"
#include "World.h"

D_Line::D_Line(const Vector2 &pos, const Vector2 &pos2, float angle, float dist, void* world_ptr, bool is_final, bool should_make_obstacles, int fadeIn_time) {

	objects = {};
	setPosition(pos);
	setAnchor(0.5f, 1);
	setHeight(dist);
	setWidth(4);
	_is_final_platform = is_final;
	setColor(Color::IndianRed);
	distance = dist;
	setRotation(GF::rad(angle + 90));
	setAlpha(100);
	_end_pos = pos2;
	make_new_arrow();
	_should_have_made_obstacles = should_make_obstacles;
	
	if (should_make_obstacles == true) {
		make_obstacles(pos, pos2, angle, dist, world_ptr, fadeIn_time);
	}

	if (fadeIn_time != 0) {
		setAlpha(0);
		addTween(Actor::TweenAlpha(100), fadeIn_time);
	
	}
	
}

void D_Line::set(float angle_in_radians, const Vector2 &pos, const Vector2 &pos2) {
	setRotation(angle_in_radians + (MATH_PI / 2));
	setPosition(pos);
	_end_pos = pos2;
}

void D_Line::make_new_arrow() {
	current = new arrow(getRotation(), distance);
	addChild(current);
}

void D_Line::fadeIn(spActor actor, unsigned int fadeTime) {
	if (fadeTime != 0) {
		actor->setAlpha(0);
		actor->addTween(Actor::TweenAlpha(255), fadeTime);
	}
}

void D_Line::make_obstacles(const Vector2 &pos, const Vector2 &pos2, float angle, float dist, void* world_ptr, unsigned int fadeTime) {
	
	World *w_ptr = static_cast<World*>(world_ptr);

	//Objects that are not children of the D_Line, should have alpha tweens called. 
	if (_is_final_platform == true) {
		float new_dist = (dist / 2);
		float new_angle = GF::rad(angle);
		Vector2 new_pos = Vector2(pos2.x - (cos(new_angle) * new_dist), pos2.y - (sin(new_angle) * new_dist));
		spEntity e = new Temple(new_pos.y, w_ptr, w_ptr->level);
		fadeIn(e, fadeTime);
		
		objects.push_back(e);
	}
	else {
		
		
		int num_of_items = 1 + random2::getBool();
		for (int i = 0; i < num_of_items; i++) {
			int rndm;
			if (w_ptr->getLevel() != 2) {
				rndm = random2::getBool();//random2::randomrange(0,1);
			}
			else {
				rndm = 0;
			}
			
			float new_dist;
			float new_angle;
			int int_1 = 0;
			int int_2 = 0;
			int int_3 = 0;
			int int_4 = 0;
			Vector2 reserve_pos;
			//Vector2 v = rel_pos - Vector2(cos(GF::rad(angle)) * dist, sin(GF::rad(angle)) * dist);
			Vector2 new_pos;
			spEntity e;
			switch (rndm) {
			case 0: //Chainsaw
				new_dist = (dist / 2);
				new_angle = GF::rad(angle + random2::randomrange_complex(-30, 30));
				new_pos = Vector2(pos2.x - (cos(new_angle) * new_dist), pos2.y - (sin(new_angle) * new_dist));
				e = new Chainsaw(new_pos, random2::randomrange(5, 30), random2::randomrange(10, 30), fadeTime);
				addChild(e);
				
				objects.push_back(e);
				break;
			case 1: //Black hole
				
				int_1 = random2::randomrange(10, 50); //Size of black_hole.
				if (int_1 < 15) {
					new_dist = (dist / random2::randomrange_float(1.5f, 4.5f));
				}
				else {
					new_dist = dist / 2;
				}

				new_angle = GF::rad(angle + random2::randomrange_complex(-30, 30));
				new_pos = Vector2(pos2.x - (cos(new_angle) * new_dist), pos2.y - (sin(new_angle) * new_dist));
				e = new Black_hole(new_pos, random2::randomrange(10, 60), world_ptr, fadeTime);
				addChild(e);
				objects.push_back(e);
				break;
			case 2: //ForceField
				int_1 = random2::randomrange(30, 400);
				int_2 = random2::randomrange(30, 400);
				new_dist = (dist / random2::randomrange_float(1.5f, 4.5f));
				new_angle = GF::rad(angle + random2::randomrange_complex(-30, 30));
				new_pos = Vector2(pos2.x - (cos(new_angle) * new_dist), pos2.y - (sin(new_angle) * new_dist));
				e = new ForceField(RectF(new_pos, Vector2(int_1, int_2)), random2::randomrange(0, 3), random2::randomrange(20, 80), fadeTime);
				//addChild(e);
				fadeIn(e, fadeTime);
				objects.push_back(e);
				break;
			

			}
		}
	}
	
	
	
	
}

void D_Line::destroy() {
		

		for (unsigned int i = 0; i < objects.size(); i++) {
			objects[i]->detach();
			
			//delete objects[i].get();
			
			//objects[i].~intrusive_ptr();
		}
		objects.clear();
	
	
	current->detach();
	//current.~intrusive_ptr();
	detach();
}

void D_Line::fadeOut(unsigned int time) {
	//log::messageln("T");
	GF::fadeOut(this, time);
	for (unsigned int i = 0; i < objects.size(); i++) {
		objects[i]->fadeOut(time);
	}
}