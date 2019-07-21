#pragma once
#include "oxygine-framework.h"
#include "oxygine/res/ResAnim.h"
#include "oxygine/core/NativeTexture.h"
#include "oxygine/AnimationFrame.h"
#include "ImageRes.h"
#include "GF.h"
#include "Tile_M.h"
#include "Entity.h"
#include "Temple.h"
using namespace oxygine;

class D_Line;

DECLARE_SMART(arrow, sparrow);
class arrow : public Sprite {
public:

	spTween tw;
	arrow(float angle, float dist) {
		
		
		setPosition(0,0);
		setResAnim(res::resources.getResAnim("Arrow"));
		setAnchor(0.5f, 0.5f);
		
		//setAlpha(0.5);
		//setRotation(angle);
		spTween t = addTween(Actor::TweenPosition(0,dist), 3000, -1, false, 0, Tween::ease_inOutCubic);
		addTween(Actor::TweenAlpha(0), 3000, -1, false, 0, Tween::ease_inOutCubic);
		//spTween
		tw = t;
		//t->addDoneCallback(CLOSURE(this, &arrow::end_tween));
		//t->detachWhenDone();
	}
	
	
};

DECLARE_SMART(D_Line, spD_Line);

class D_Line : public ColorRectSprite {
	
private:
	//This tells if the platform holding this line is the final platform.
	bool _is_final_platform;
	//This tells you what level the world is on.
	//This is used primarily for knowing what obstacles to make.
	unsigned int _world_level;
	//This tells if the d_line should have made obstacles or not.
	bool _should_have_made_obstacles = false;
	std::vector<spEntity> objects;
	//This is the distance that all the arrows should move.
	float distance = 32;
	//This is the second point at which the D_Line goes to.
	Vector2 _end_pos;
	//This is the arrow that moves across the line.
	sparrow current;
	//This will call a tween upon the actor, such that it will fade in slowly.
	inline void fadeIn(spActor actor, unsigned int fadeTime);
public:
	
	void make_new_arrow();
	//Angle needs to be in radians!
	//This will calculate the line faster then the alternative.
	D_Line(const Vector2 &pos, const Vector2 &pos2, float angle, float dist, void* world_ptr, bool is_final, bool should_make_obstacles = true, int fadeIn_time = 0);

	//This will create obstacles in randomly near the line.
	void make_obstacles(const Vector2 &pos, const Vector2 &pos2, float angle, float dist, void* world_ptr, unsigned int fadeTime);
	//This will clean up everything.
	void destroy();
	//This will make everything fade out how it should.
	void fadeOut(unsigned int time = 0);
	//This will set the variables within to the given parameters.
	void set(float angle_in_radians, const Vector2 &pos, const Vector2 &pos2);
	

};
