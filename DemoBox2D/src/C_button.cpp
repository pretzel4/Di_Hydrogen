#include "C_button.h"
#include "ImageRes.h"
#include "Noise.h"


C_button::C_button(const Vector2 &pos, char* words_to_display, std::function<void()> &callback, bool should_play_sound) : _play_sound(should_play_sound) {
	setResAnim(res::resources.getResAnim("Button"));
	setPosition(pos);
	setAnchor(0.5f, 0.5f);
	_text = new Text(Vector2(0, 0), words_to_display);
	_text->attachTo(getStage());
	addChild(_text);
	lambda = callback;
	
	
	
}

void C_button::setText(char* string) {
	_text->setText(string);
}

void C_button::clicked(Event* ev) {
	
	if (!_is_locked) {
		
		frame = 2;
		setAnimFrame(getResAnim(), frame);
		if (_play_sound) {
			Noise::play_ID(Noise::st::Button_Click, Noise::player::sfx);
		}
		lambda();
		
	}

}