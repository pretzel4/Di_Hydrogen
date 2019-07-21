#pragma once
#include "oxygine-framework.h"
#include "Text.h"
using namespace oxygine;

/*Two Point Information Line*/
/*This will make a line between two points and give you the distance between the two.
they are updated every frame.*/
DECLARE_SMART(TPI_Line, spTPI_Line);
class TPI_Line : public Box9Sprite {
private:
	//This is the type of line that it is.
	const unsigned int _type;
	//This is the additional text that is specific to the determined type.
	spText _add_txt;
	bool is_done_fading_out = false;
	//This is the radius of the circle at which you entered.
	const float _radius;
	//This displays the distance between the two points.
	spText dist_display;
	//This function will calculate the angle and position of the graphical representaion(sprite).
	void calculate();
	//This is a transformation vector.
	//This will be added to the _point2 vector upon angle calculation.
	Vector2 _transform;
	const Vector2 *_point1;
	const Vector2 *_point2;
	void doUpdate(const UpdateState &us);
	bool _is_fading = false;
	bool _is_fading_in = false;
	//This is called after it is done fading.
	void done_fading(Event *ev);
	//This determines if you have called the fadeOut function or not.
	bool hcfo = false;
public:
	//This tells you if the line is done fading.
	bool has_faded_out();
	TPI_Line(const Vector2& point1, const Vector2 &point2, float radius, unsigned int type);
	//This will make the line fade out. This will not destroy the line!
	//This will add an event listener to the object, do not set the event listener; Instead, add one.
	//This will return NULL in the case that it is already fading out.
	spTween fadeOut(int fade_time);
	//This will set the transform of the TPI_Line. (Only effects _point2!)
	void setP_Transform(const Vector2 &new_transform);
	//This will get the transform of the TPI_Line.
	const Vector2 &getP_Transform();
};