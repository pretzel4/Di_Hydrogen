#include "TPI_Line.h"
#include "ImageRes.h"
#include <cmath>
#include "GF.h"
#define grid_size 16
#define max_distance_stretch 200
#define fadeIn_time 2000
void TPI_Line::doUpdate(const UpdateState &us) {
	calculate();
}

TPI_Line::TPI_Line(const Vector2& point1, const Vector2 &point2, float radius, unsigned int type) : _point1(&point1), _point2(&point2), _radius(GF::convert_to_Oxy(radius)), _type(type), _transform(Vector2(0,0))  {
	
	dist_display = new Text(Vector2(100, 30), "");
	_add_txt = new Text(Vector2(100, 0), "");
	_add_txt->setFontSize(16);
	switch (type) {
	case 0:
		_add_txt->setText("");
		break;
	case 1:
		dist_display->setColor(Color::OrangeRed);
		setScale(0.5f, 0.5f);
		setColor(Color::Red);
		setAlpha(255);
		//_add_txt->setText("Black Hole");
		//_add_txt->setColor(Color::Red);
		break;
	}
	
	
	
	setResAnim(res::resources.getResAnim("I_Dot"));
	addChild(dist_display);
	addChild(_add_txt);
	
	setPosition(point1);

	//setHeight(16 * grid_size);
	setAnchorX(0.5f);
	setWidth(16);
	setVerticalMode(Box9Sprite::StretchMode::TILING_FULL);

	
	calculate();
	//setVerticalGuides(10, 11);
	setGuides(10, 11, 1, 16 * 3);
	setAlpha(0);
	addTween(Actor::TweenAlpha(255), fadeIn_time);
	_is_fading_in = true;
}

void TPI_Line::setP_Transform(const Vector2 &new_transform) {
	_transform = new_transform;
}

const Vector2 &TPI_Line::getP_Transform() {
	return _transform;
}

void TPI_Line::calculate() {
	if (_point1 != NULL && _point2 != NULL) {
		
		Vector2 _point2_c = *_point2 - _transform;
		Vector2 _point1_c = *_point1;
		setPosition(_point1_c);
		float y_dif = _point1_c.y - _point2_c.y;
		float x_dif = _point1_c.x - _point2_c.x;
		float distance = GF::pyagor_hypot(x_dif, y_dif);
		float angle = atan2(y_dif, x_dif) + (MATH_PI / 2);//atan(y_dif / x_dif) - (MATH_PI / 2);
		dist_display->setRotation(-angle);
		_add_txt->setRotation(-angle);
		setRotation(angle);

		std::string s;

		if (_type == 0) {
			setHeight(distance);
			s.append(GF::convert_to_string(distance));
			s.append("m");

		}
		else {
			setHeight(distance * 1.5f);
			s = "";
		}

		//dist_display->setNum(distance);
		dist_display->setText(s);

		if (!_is_fading && _type == 0) {
			float alpha = 255 * (distance / _radius);
			if (alpha > 255) {
				alpha = 255;
			}
			if (_is_fading_in) {
				if (getAlpha() >= alpha) {
					removeTweens();
					_is_fading_in = false;
					setAlpha(alpha);
				}

			}
			else {
				setAlpha(alpha);
			}

		}
	}
	


}

bool TPI_Line::has_faded_out() {
	return is_done_fading_out;
}

spTween TPI_Line::fadeOut(int fade_time) {
	if (hcfo == false) {
		removeTweens();
		hcfo = true;

		_is_fading_in = false;
		_is_fading = true;
		spTween t = addTween(Actor::TweenAlpha(0), fade_time);
		t->addDoneCallback(CLOSURE(this, &TPI_Line::done_fading));
		return t;
	}
	return NULL;
	
}

void TPI_Line::done_fading(Event *ev) {
	//_is_fading = false;
	is_done_fading_out = true;
}