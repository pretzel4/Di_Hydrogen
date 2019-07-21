#pragma once

#include "oxygine-sound.h"

#include "oxygine-framework.h"
using namespace oxygine;
#define default_sound_fade_time 1000
namespace Noise {
	
	
	enum st {
		Rocket_Fire0 = 0,
		Break0 = 1,
		Break1 = 2,
		Break2 = 3,
		Rocket_Fire1 = 4,
		Rocket_Fire2 = 5,
		Button_Click = 6,
		Run_Out_Of_Fuel = 7,
		Land_On_Platform = 8,
		Land_On_Platform_Final = 9,
		Almost_Made_It = 10,
		Black_Hole_Suck = 11,
		Black_Hole_Wind = 12,
		Collision = 13
	};
	enum player {
		sfx = 0,
		music = 1
	};

	class instance {
	private:
		//This determines if the si pointer is null or not.
		bool is_null = true;
		//Use this to update the constants.
		void set_consts(st ID, player type) const;
	public:
		

		void operator=(instance &other){
			si = other.si;
			set_consts(other._ID, other._type);
		}
		PlayOptions _settings;
		//This is used for distinguishing what type of sound or music you want to play.
		mutable st _ID;
		//This is used for determining if the _ID is music or an sfx.
		mutable player _type;
		//This is used for modifying the sound being played right now.
		spSoundInstance si = spSoundInstance();
		instance(st ID, player type, const PlayOptions &settings = oxygine::PlayOptions()) : _ID(ID), _type(type), _settings(settings) {
			
			
		}
		//You should use this to play the instance.
		void play();
		//This will stop the playback.
		void pause();
		//This will fade the pause.
		void fade_pause(int ms_time = default_sound_fade_time);
		//This will resume the playback(if it was otherwise paused).
		void resume();
		//This should be called whenever you are done with it.
		void destroy();
		//This pause the playback and put it back to the start.
		void stop();
		void setCoord(Vector2 position) {

		}

		
	};
	
	//This should not be modified unless you know what you are doing!
	namespace detail {
		//This determines if the music is muted or not.
		extern bool _is_music_muted;
		//This determines if the sound is muted or not.
		extern bool _is_sound_muted;
		char* get_string(st ID);
	}
	extern SoundPlayer sfx_player;
	extern SoundPlayer music_player;
	//This will return NULL if it did not play.
	//If overwrite is true, the sound will be played even if the type is muted.
	spSoundInstance play_ID(st ID, player type, PlayOptions &settings = oxygine::PlayOptions(), bool overwrite = false);
	//This will initialize everything.
	void init();
	//This will clean up all the namespace before exit.
	void clean();
	//This should be called every frame.
	void update();
	//This will mute the sound.
	void mute_sound();
	//This will unmute the sound.
	void unmute_sound();
	//This will mute the music.
	void mute_music();
	//This will unmute the music.
	void unmute_music();
	//This will set the sound to the given boolean.
	//True = muted, False = unmuted.
	void set_sound_state(bool new_state);
	//This will set the music to the given boolean.
	//True = muted, False = unmuted.
	void set_music_state(bool new_state);
	//This will tell you what the state of the music is at. (muted or not muted).
	bool get_music_state();
	//This will tell you what the state of the sound is at. (muted  or not muted).
	bool get_sound_state();
	//This will toggle the state of the sound.
	//This will return the new state as well.
	bool toggle_sound_state();
	//This will toggle the state of the music.
	//This will return the new state as well.
	bool toggle_music_state();
	
};