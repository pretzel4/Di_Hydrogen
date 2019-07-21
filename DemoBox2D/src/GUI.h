#pragma once

#include "Background.h"
#include "Text.h"
#include "I_control.h"
#include "C_button.h"
#include "P_Button.h"
#include "Planet.h"

DECLARE_SMART(Pause_Sign, spPause_Sign);
class Pause_Sign : public Actor {
private:
	bool _state = false;
	spText _text;
public:
	Pause_Sign(const Vector2 &position);
	//This will make the text appear.
	void activate();
	//This will make the text dissapear.
	void deactivate();
	//This will set the state depending on what you give.
	void set_state(bool new_state);
	//This will get you the state of the pause sign.
	bool get_state();
};
DECLARE_SMART(Logo, spLogo);
class Logo : public Sprite {
private:
	void tilt_right_done(Event *ev);
	void tilt_left_done(Event *ev);
public:
	Logo(const Vector2 &position);
};
DECLARE_SMART(Fader, spFader);
class Fader : public ColorRectSprite {
private:
	//This is what the fader should attach itself to when it starts to fade in.
	spActor _tat;
	//This is the default fading speed.
	const int _fade_speed;
public:
	//This will make it fade to black.
	void fade255();
	//This will make it fade to black, and then call the function given.
	void fade255(const EventCallback &ev);
	//This will make it fade in.
	void fade0();
	//This is used for fading the screen in and out.
	Fader(const RectF &rect, spActor to_attach_to, bool should_start_black);
};


DECLARE_SMART(World_Counter, spWorld_Counter);
class World_Counter : public TextField {
private:
	
	//This will play an animation.
	void play_ani();
	//This will update the text.
	inline void update_text(unsigned int world_number);
public:
	//This will set the world to the given amount.
	void setWorld(unsigned int world);
	//This counts the world you are on.
	World_Counter(const Vector2 &position, unsigned int starting_world);
	//This will reset the counter.
	void reset();
};
DECLARE_SMART(High_Score, spHigh_Score);
class High_Score : public TextField {
private:
	//This is the file that the object gets its high score information from.
	const char* _file;
	//This is the score that got overwritten by the new high score.
	int old_score;
	//This is the score that you just got when opening the menu.
	//This is stored incase we need to return back and undo what we just did.
	int recent_score;
	//This will replace one score with another.
	void find_replace(const char* file, unsigned int size, unsigned int find_score, unsigned int new_score);
	//This will set the given file into the given information.
	void set(const char* file, std::vector<unsigned int> scores, unsigned int size);
	//This will convert an integer into infomation that should be placed into a file.
	//index 0 = over.
	//index 1 = dif.
	std::vector<unsigned int> convert_to_info(int convert_int);
	//This will convert data into an integer, given the two variables needed(over and dif). see convert_to_info();
	unsigned int convert_to_u_int(unsigned int over, unsigned int dif);
	//This will return a vector full of the top highest scores.
	std::vector<int> getTopScores(const char* file, unsigned int size);
	//This will update the score list depending on what score is given.
	//It will return the score that it just overwrote.
	//However, if it did not overwrite anything, then it will return -1.
	int updateScores(const char* file, unsigned int new_score, unsigned int size);
	
	
public:
	int addScore(unsigned int new_score);
	//This will reset what has been done last when it was opened.
	//The scores will go back to the way they were before it was opened.
	void revert();
	//This will display the high scores in the given high score folder.
	High_Score(const Vector2 &pos, const char* file, unsigned int new_high_score);
	//This is called when the high score is removed from the stage.
	void onRemovedFromStage();
	//This will reset all the scores to the default.
	void reset();
	//This will actualy update the scores to the display.
	//This will not change the file at all.
	void updateScoreDisplay(const char* file, unsigned int size);
	
	
};
DECLARE_SMART(Menu_M, spMenu_M);
class Menu_M : public Actor {
private:
	
protected:
	
	//This is how fast it moves when opening or closing.
	const int ani_time = 1000;
	//This is a pointer that points that can point to whatever you want.
	void* abstract_ptr;
	//This determines if the menu is open or not.
	bool _is_open = false;
	//This is where it should slide to when opened.
	Vector2 _open_pos;
	//This is where it starts, also where it should slide to when you close the menu.
	Vector2 _start_pos;
public:
	//This function will rid the start_menu off the screen and then delete itself.
	virtual void close();
	//This will open the start menu. Given that you created another menu object.
	virtual void open();
	//This will close the start menu, and call the function given afterward. This is a little slower then normal close().
	virtual void close(const EventCallback &ev);
	//This will open the start menu, and call the function given afterward. This is a little slower then normal open().
	virtual void open(const EventCallback &ev);
	//This will return true if the menu is open; otherwise, it will return false.
	bool is_open();
};

DECLARE_SMART(Pause_Menu, spPause_Menu);
class Pause_Menu : public Menu_M {
private:
	//This will get the music string for the buttons depending on whether or not the music is muted.
	char* get_mb_string();
	//This will get the sound string for the buttons depending on whether or not the sound is muted.
	char* get_sb_string();
	spC_button m_button;
	spC_button s_button;
public:
	void close();
	void open();

	Pause_Menu(const Vector2 &start_pos, const Vector2 &open_pos, void* world_ptr);
};

DECLARE_SMART(Win_Menu, spWin_Menu);
class Win_Menu : public Menu_M {
private:
	//This is the sign that congratulates you for winning the game.
	spSprite congrats;
	//This is a planet that floats around. For decoration.
	spPlanet _planet;
	//This is the background of the final scene.
	spSprite bg;
	std::function<void(Event* ev)> _up;
	std::function<void(Event* ev)> _down;
	//This button will reset the high scores.
	spC_button reset_s_button;
	spHigh_Score score_board;
	void* _gui_ptr;
	bool are_scores_locked = false;
	spC_button _reset_button;
	spC_button start_button;
	//This is a bird that flaps in the center of the screen.
	spSprite bird;
	//This is the file at which the win menu reads scores from.
	char* _file;
public:
	//This overwrites the close function from Menu_M(the mother class).
	//This function will rid the restart_menu off the screen and then delete itself.
	void close();
	// This will close the start menu, and call the function given afterward.This is a little slower then normal close().
	void close(const EventCallback &ev);
	void open(const EventCallback &ev);
	void open();
	Win_Menu(const Vector2 &open_pos, void* world_ptr, void* gui_ptr, unsigned int new_score);
};

DECLARE_SMART(Restart_Menu, spRestart_Menu);
class Restart_Menu : public Menu_M {

private:
	//This is the GUI object.
	void* _gui_ptr;
	//This button will reset the high scores.
	spC_button reset_s_button;
	
	//This is the button that starts the game.
	spC_button start_button;
	//This is the file at which the high scores are read and written from.
	char* _file_name;
public:
	//This will display the top scores you have gotten.
	spHigh_Score score_board;
	//This will let you know if the scores are locked.
	bool are_scores_locked = false;
	//The start_pos is where the menu should start, probably offscreen.
	//The open_pos is where the menu will slide to when you call the open() function.
	Restart_Menu(const Vector2 &start_pos, const Vector2 &open_pos, void* world_ptr, void* gui_ptr, unsigned int new_score);
	//This overwrites the close function from Menu_M(the mother class).
	//This function will rid the restart_menu off the screen and then delete itself.
	void close();
	// This will close the start menu, and call the function given afterward.This is a little slower then normal close().
	void close(const EventCallback &ev);
	void open();
	//This will open the start menu, and call the function given afterward. This is a little slower then normal open().
	void open(const EventCallback &ev);
};
DECLARE_SMART(Start_Menu, spStart_Menu);
class Start_Menu : public Menu_M {
private:
	char* get_mb_string();
	char* get_sb_string();
	//This is a button that is used for muting the music.
	spC_button m_button;
	//This is a button that is used for muting the sound.
	spC_button s_button;
	//This is the logo of the game.
	spLogo _logo;
	//This points to the GUI object.
	void* _gui_ptr;
	//This is the button that starts the game.
	spC_button start_button;
	
public:
	void close();
	void open();
	void close(const EventCallback &ev);
	void open(const EventCallback &ev);
	//The start_pos is where the menu should start, probably offscreen.
	//The open_pos is where the menu will slide to when you call the open() function.
	Start_Menu(const Vector2 &start_pos, const Vector2 &open_pos, void* gui_ptr);
	
};
DECLARE_SMART(Platform_Counter, spPlatform_Counter);
class Platform_Counter : public TextField {
private:
	//This counts how many platforms you have landed on so far.
	unsigned int how_many = 0;
	//This will play an animation.
	void play_ani();
	//This will update the text to display what should be displayed acoording to the "how_many" variable.
	inline void update_text();
public:
	//Default constructor
	Platform_Counter() {};
	
	Platform_Counter(const Vector2 &pos);
	//This function will increase the counter by the given amount.
	void count(int addend);
	//This will tell you how many platforms you landed on.
	unsigned int get_how_many() { return how_many; }
	//This will reset the counter.
	void reset();
};

DECLARE_SMART(GUI, spGUI);
class GUI : public Actor {
	
private:
	//This tells you that the game is paused.
	spPause_Sign _pause_sign;
	//This should point to the world object.
	void* _world_ptr;
	//This is used for fading the screen in and out.
	spFader fader;
	//This is the menu for restarting.
	spRestart_Menu r_menu;
	//This is the start_menu.
	spStart_Menu s_menu;
	//This is the win_menu.
	spWin_Menu w_menu;
	//This is the menu that opens when you pause the game.
	spPause_Menu p_menu;
	spI_control clicker1;
	spI_control clicker2;
	//This counts how many worlds you have gone through.
	spWorld_Counter wc;
	
	//This is a button that is used for pausing the game.
	spP_Button pb;
	//This will toggle the world's pause boolean.
	void toggle_pause();
public:
	//This counts the number of platforms you have landed on in total.
	spPlatform_Counter pc;
	//This will set the world of the world counter.
	void set_world_counter(unsigned int new_world);
	//The void object should be a world object; otherwise, the behavior is undefined.
	GUI(unsigned int *lvl, void* player, void* world_ptr);
	//This will make the game unpaused.
	void unpause();
	//This will pause the game. (the box2d simulation).
	void pause();
	//This will return true if the start menu is open; otherwise, it will return false.
	bool is_start_menu_open();
	//This will return true if the restart menu is open; otherwise, it will return false.
	bool is_restart_menu_open();
	//This will set the color of the platform counter.
	void set_pc_color(Color &new_color);
	//This will increase the platform counter by the given amount.
	void increase_p_counter(int addend);
	//This will reset everything inside the gui object.
	void reset();
	//This will open the restart menu.
	void open_restart_menu(void* world_ptr);
	//This will open the restart menu and callback a given function.
	void open_restart_menu(const EventCallback &ev);
	//This will open the start menu.
	void open_start_menu();
	//This will open the start menu and callback a given function.
	void open_start_menu(const EventCallback &ev);
	//This will close the restart menu.
	void close_restart_menu();
	//This will close the restart menu and callback a given function.
	void close_restart_menu(const EventCallback &ev);
	//This will close the start menu.
	void close_start_menu();
	//This will close the start menu and callback a given function.
	void close_start_menu(const EventCallback &ev);
	//This will close the restart menu.
	void close_win_menu();
	//This will close the restart menu and callback a given function.
	void close_win_menu(const EventCallback &ev);
	//This will open the restart menu.
	void open_win_menu(void* world_ptr);
	//This will open the restart menu and callback a given function.
	void open_win_menu(const EventCallback &ev);

	//This will make the world fade out to black.
	void fadeOut();
	//This will make the world fade out to black, and call the function given.
	void fadeOut(const EventCallback &ev);
	//This will make the world fade in.
	void fadeIn();
};