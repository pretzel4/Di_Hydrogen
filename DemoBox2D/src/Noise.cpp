#include "Noise.h"
#include "ImageRes.h"

#define how_many_channels 16
extern SoundPlayer Noise::sfx_player = SoundPlayer();
extern SoundPlayer Noise::music_player = SoundPlayer();
extern bool Noise::detail::_is_music_muted = false;
extern bool Noise::detail::_is_sound_muted = false;

bool Noise::toggle_sound_state() {
	if(detail::_is_sound_muted) {
		detail::_is_sound_muted = false;
	}
	else {
		detail::_is_sound_muted = true;
	}
	return detail::_is_sound_muted;
}

bool Noise::toggle_music_state() {
	if (detail::_is_music_muted) {
		detail::_is_music_muted = false;
	}
	else {
		detail::_is_music_muted = true;
	}
	return detail::_is_music_muted;
}

bool Noise::get_music_state() {
	return detail::_is_music_muted;
}

bool Noise::get_sound_state() {
	return detail::_is_sound_muted;
}

void Noise::mute_sound() {
	detail::_is_sound_muted = true;
}

void Noise::unmute_sound() {
	detail::_is_sound_muted = false;
}

void Noise::mute_music() {
	detail::_is_music_muted = true;
}

void Noise::unmute_music() {
	detail::_is_music_muted = false;
}

void Noise::set_sound_state(bool new_state) {
	switch (new_state) {
	case 0:
		unmute_sound();
		break;
	case 1:
		mute_sound();
	}
	
}

void Noise::set_music_state(bool new_state) {
	switch (new_state) {
	case 0:
		unmute_music();
		break;
	case 1:
		mute_music();
	}
}

void Noise::instance::destroy() {
	if (is_null == false) {
		if (si.get() != NULL) {
			if (si->isPlaying() == true && is_null == false) {
				si->stop();
			}
		}
		
	}
	
	
}

void Noise::instance::set_consts(st ID, player type) const{
	_ID = ID;
	_type = type;
}
void Noise::instance::pause() {
	if (!is_null) {
		si->fadeOutPause(0);
	}
	
}

//This pause the playback and put it back to the start.
void Noise::instance::stop() {
	si->stop();
}

void Noise::instance::fade_pause(int ms_time) {
	si->fadeOutPause(ms_time);
}

void Noise::instance::resume() {
	si->resume();
}

void Noise::instance::play() {
	if (SoundSystem::instance->get() != NULL){
		switch (_type) {
		case player::sfx:
			if (!detail::_is_sound_muted) {
				si = play_ID(_ID, _type, _settings);

				is_null = false;
			}
			break;
		case player::music:
			if (!detail::_is_music_muted) {
				si = play_ID(_ID, _type, _settings);
				is_null = false;
			}
		}
	}
	else {
		logs::messageln("RAN OUT OF CHANNELS!");
	}
	
	
}

char* Noise::detail::get_string(st ID) {
	switch (ID) {
	case 0:
		return "Engine_Fire0";
	case 1:
		return "Break0";
	case 2:
		return "Break1";
	case 3:
		return "Break2";
	case 4:
		return "Engine_Fire1";
	case 5:
		return "Engine_Fire2";
	case 6:
		return "Button_Click";
	case 7:
		return "Run_Out_Of_Fuel";
	case 8:
		return "Land_On_Platform";
	case 9:
		return "Land On Final Platform";
	case 10:
		return "Almost Made It";
	case 11:
		return "Black_Hole_Suck";
	case 12:
		return "Black_Hole_Wind";
	case 13:
		return "Collision";
	}
}

spSoundInstance Noise::play_ID(st ID, player type, PlayOptions &settings, bool overwrite) {
	switch (type) {
	case 0:
		if (detail::_is_sound_muted == false || overwrite == true) {
			return sfx_player.play(detail::get_string(ID), settings);
		}
		break;
	case 1:
		if (!detail::_is_music_muted == false || overwrite == true) {
			return music_player.play(detail::get_string(ID), settings);
		}

	}
	
	return NULL;
}

void Noise::init() {
	//SoundSystem::create();
	//SoundSystem::get();
	SoundSystem::create()->init(how_many_channels);
	
	SoundPlayer::initialize();
	sfx_player.setResources(&res::resources);
	music_player.setResources(&res::resources);
	//sfx_player.play("track_44100_mono");
}

void Noise::clean() {
	sfx_player.stop();
	music_player.stop();
	SoundPlayer::free();
	SoundSystem::free();
}

void Noise::update() {
	SoundSystem::get()->update();
	sfx_player.update();
	music_player.update();
}