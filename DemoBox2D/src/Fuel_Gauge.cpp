#include "Fuel_Gauge.h"
#include "ImageRes.h"
#include "GF.h"
#include "Ship_Torso.h"
#define ani_speed 1000
#define speed 0.04
void fra::start(float current_fuel, float new_fuel) {
	_fuel_to_add_total = new_fuel - current_fuel;
	_ticks = 0;
}

float fra::tick(const UpdateState &us) {
	if (_fuel_to_add_total != 0) {
		float r = speed * us.dt;
		_ticks += r;
		if (_ticks >= _fuel_to_add_total) {

			r = r - (_fuel_to_add_total - _ticks);
			_fuel_to_add_total = 0;
			return r;
		}
		else {
			return r;
		}
	}
	else {
		return 0;
	}
	
	/*if (_fuel_to_add_total != 0) {
		_ticks += us.dt;
		bool should_reset = false;
		if (_ticks > _max_ticks) {
			throw("ERROR");
			_ticks = _max_ticks;
			should_reset = true;
		}
		prev_return = (_fuel_to_add_total * (_ticks / _max_ticks)) - prev_return;
		if (should_reset) {
			_fuel_to_add_total = 0;
		}
		return prev_return;
	}*/
	
}

L_fuel::L_fuel(const Vector2 &position) {
	setResAnim(res::resources.getResAnim("Ship_Fuel"));
	setAnchor(0.5f, 0.5f);
	setPosition(position);
}

void Fuel_Gauge::update_fuel(float new_fuel) {
	_f_per = 32 - (32 * (new_fuel / _max_fuel));
	fuel_sprite->setY(init_pos.y - (_f_per));
	
	
	//clip_rect->setHeight(init_height - per);
	//fuel_sprite->setAnchorInPixels(Vector2(22, (init_height + (per) - 16)));
	//clip_rect->setAnchorY(1.0f - (0.5f * (new_fuel / _max_fuel)));
	
}

Fuel_Gauge::Fuel_Gauge(const Vector2 &global_pos, const Vector2 &pos, float max_fuel, spWorld world_ptr) : init_pos(pos), init_height(res::resources.getResAnim("Ship_torso")->getHeight()), _fra(10000) {
	//setProgress(0);
	
	setAnchor(0.5f, 0.5f);
	//setPosition(-pos / 2);
	float per = 16;
	//This creates the backplate of the fuel, when the fuel decreases, it will be replaced with the back_plate.
	bp = new back_plate(pos);
	/*debug_rect = new ColorRectSprite;
	debug_rect->setColor(Color::Green);
	debug_rect->setAnchor(0.5f, 0.5f);
	debug_rect->setPosition(pos);*/
	
	fuel_sprite = new L_fuel(pos);
	fuel_sprite->setAnchor(0.5f, 0.5f);
	fuel_sprite->setY(init_pos.y - per);
	//Mask rectangle ~used for determines where the fuel sprite should be visible.
	/*mask_rect = new MaskedSprite;
	mask_rect->setPosition(pos);
	mask_rect->setSize(res::resources.getResAnim("Ship_torso")->getSize());
	mask_rect->setAnchor(0.5f, 0.5f);*/
	
	//Clip rectangle ~used for making it so that the fuel drains from your ship~
	clip_rect = new ClipRectActor();
	clip_rect->setPosition(global_pos);
	clip_rect->setSize(res::resources.getResAnim("Ship_torso")->getSize());
	clip_rect->setAnchor(0.5f, 0.5f);
	clip_rect->setY(init_pos.y);
	clip_rect->setHeight(init_height);
	
	//Alpha rectangle ~used as a mask for the fuel sprite~
	/*alpha_rect = new Sprite();
	alpha_rect->setResAnim(res::resources.getResAnim("Torso_Alpha"));
	alpha_rect->setPosition(pos);
	alpha_rect->setSize(res::resources.getResAnim("Ship_torso")->getSize());
	alpha_rect->setAnchor(0.5f, 0.5f);
	//alpha_rect->setY(alpha_rect->getY() - 7);
	mask_rect->setMask(alpha_rect);*/
	
	
	_max_fuel = max_fuel;
	/*bar = new ProgressBar;
	bar->setDirection(ProgressBar::dir_90);
	
	bar->setPosition(pos);
	bar->setAnchor(0.5f, 0.5f);
	bar->setResAnim(res::resources.getResAnim("Ship_Fuel"));
	bar->attachTo(getStage());
	bar->setProgress(0);
	//bar->_setSize(32, 32);
	//bp->attachTo(getStage());
	
	addChild(bar);*/
	addChild(bp);
	clip_rect->setPriority(4500);
	world_ptr->addChild(clip_rect);
	clip_rect->addChild(fuel_sprite);
	//world_ptr->addChild(debug_rect);
	//addChild(mask_rect);
	//mask_rect->addChild(fuel_sprite);

	//bar->addTween(ProgressBar::TweenProgress(1), ani_speed);
	//The animation will
	//setProgress(1);
}

//void Fuel_Gauge::update() {
//	bar->addTween(ProgressBar::TweenProgress(0.5), ani_speed);
//}

float Fuel_Gauge::update_animation(const UpdateState &us) {
	return _fra.tick(us);
}

void Fuel_Gauge::update_rotation(Sprite* sprite) {
	//clip_rect->setRotation(-sprite->getRotation());
	fuel_sprite->setRotation(sprite->getRotation());
	
	//RectF bounds = sprite->getSrcRect();
	//RectF bounds = sprite->computeGlobalTransformation();
	//Transform bounds = sprite->computeGlobalTransform();
	// RectF bounds = getActorTransformedDestRect(sprite, sprite->computeGlobalTransform());
	//debug_rect->setPosition(bounds.pos);
	global_pos = sprite->getPosition();
	clip_rect->setX(global_pos.x);
	clip_rect->setY(global_pos.y + (_f_per));
	//Vector2 pos = Vector2(bounds.x, bounds.y);
	//clip_rect->setPosition(pos);
	
	//Finding the heighest vertex.
	/*for (unsigned int i = 0; i < 4; i++) {
		switch (i) {
		case 0:
			y = bounds.a;
			break;
		case 1:
			if (bounds.b < y) {
				y = bounds.b;
			}
			break;
		case 2:
			if (bounds.c < y) {
				y = bounds.c;
			}
			break;
		case 3:
			if (bounds.d < y) {
				y = bounds.d;
			}
			break;
		}
	}*/
	//Find the furthest left vertex.
	/*for (unsigned int i = 0; i < 4; i++) {
		switch (i) {
		case 0:
			x = bounds.a;
			break;
		case 1:
			if (bounds.toMatrix().buildSRT < y) {
				x = bounds.b;
			}
			break;
		case 2:
			if (bounds.c < y) {
				x = bounds.c;
			}
			break;
		case 3:
			if (bounds.d < y) {
				x = bounds.d;
			}
			break;
		}
	}*/

	//clip_rect->
	//clip_rect->setSize(bounds.size);

	/*debug_rect->setSize(clip_rect->getSize());
	debug_rect->setPosition(clip_rect->getPosition());
	debug_rect->setHeight(clip_rect->getHeight());
	debug_rect->setRotation(clip_rect->getRotation());*/


	//clip_rect->setWidth();
	//debug_rect->setSize(clip_rect->getSize());
	//debug_rect->setPosition(clip_rect->getPosition());
	//debug_rect->setAnchorY(clip_rect->getAnchorY());
	//debug_rect->setAnchorInPixels(clip_rect->getAnchor());
	//clip_rect->setPosition(bounds.pos);
}

void Fuel_Gauge::setFuel(float new_fuel, float rotation) {
	_last_fuel = new_fuel;
	update_fuel(new_fuel);
	
	//bar->setDirection(ProgressBar::dir_radial_cw);
	/*if (new_fuel == 100) {
		log::messageln("test");
	}
	float percent = new_fuel / _max_fuel;
	bar->setProgress(percent);
	bar->addTween(ProgressBar::TweenProgress(percent), ani_speed);*/
	
}

void Fuel_Gauge::destroy() {
	fuel_sprite->detach();
	clip_rect->detach();
	bp->detach();
	detach();
}

void Fuel_Gauge::refill(float new_fuel) {
	_fra.start(_last_fuel, new_fuel);
}
