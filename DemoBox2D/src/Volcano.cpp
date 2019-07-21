#include "Volcano.h"
#include "ImageRes.h"
#define volcano_fade_speed 3000
#define volcano_glow_speed 4000 //4 seconds
Cover::Cover(const Vector2 &position) {
	setPosition(position);
	setResAnim(res::resources.getResAnim("Volcano_front"));
}

void Volcano::doUpdate(const UpdateState &us) {
	if (getAlpha() != 0) {
		setColor(fc.update(us));
	}
}

Volcano::Volcano(const Vector2 &position) {
	setResAnim(res::resources.getResAnim("Volcano_back"));
	setPosition(Vector2(position.x, position.y - getResAnim()->getHeight()));
	
	_fade_out_lamb = [&]() {
		fc.setup(Vector4(255, 255, 255, getAlpha()), Vector4(0, 0, 0, getAlpha()), _fade_in_lamb, volcano_glow_speed);
	};
	_fade_in_lamb = [&]() {
		fc.setup(Vector4(0, 0, 0, getAlpha()), Vector4(255, 255, 255, getAlpha()), _fade_out_lamb, volcano_glow_speed);
	};
	fc.setup(Vector4(0, 0, 0, getAlpha()), Vector4(255, 255, 255, getAlpha()), _fade_out_lamb, volcano_glow_speed);
	
	_cover = new Cover(Vector2(0,0));
	addChild(_cover);
}
