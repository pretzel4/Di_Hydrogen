#include "GUI.h"
#include "ImageRes.h"
#include "World.h"
#include "Ship_M.h"
#include "GF.h"
#include "Noise.h"
#define a_max 127
#define binary_data_needed_per_slot 2
#define how_many_scores_should_be_displayed 5
#define max_score_text_margin_ani 30
#define file_size 0//(binary_data_needed_per_slot * how_many_scores_should_be_displayed) / 2
#define score_text_margin 10
#define Default_Platform_Text "Platforms: "
#define Default_World_Text "World: "
#define Default_Text_Color Color::DarkOrange
#define default_button_height getStage()->getHeight() - (getStage()->getHeight() / 6)
#define m_button_on "Music on"
#define m_button_off "Music off"
#define s_button_on "Sound on"
#define s_button_off "Sound off"

void Pause_Menu::close() {
	
	if (_is_open == true) {
		removeTweens();
		_is_open = false;
		spTween t = addTween(Actor::TweenPosition(_start_pos), ani_time, 1, false, 0, Tween::ease_inExpo);
		t->detachWhenDone(true);
	}
}

void Pause_Menu::open() {
	
	if (_is_open == false) {
		removeTweens();
		_is_open = true;
		//m_button->setText(get_mb_string());
		s_button->setText(get_sb_string());
		spTween t = addTween(Actor::TweenPosition(_open_pos), ani_time, 1, false, 0, Tween::ease_inExpo);
	}

}

char* Pause_Menu::get_mb_string() {
	if (Noise::get_music_state()) {
		return m_button_off;
	}
	else {
		return m_button_on;
	}
}

char* Pause_Menu::get_sb_string() {
	if (Noise::get_sound_state()) {
		return s_button_off;
	}
	else {
		return s_button_on;
	}
}

Pause_Menu::Pause_Menu(const Vector2 &start_pos, const Vector2 &open_pos, void* world_ptr) {
	abstract_ptr = world_ptr;
	setPosition(start_pos);
	_start_pos = start_pos;
	_open_pos = open_pos;
	std::function<void()> m_lamb = [&]() { 
		if (Noise::toggle_music_state()) {
			m_button->setText(m_button_off);
		}
		else {
			m_button->setText(m_button_on);
		}
	};
	std::function<void()> s_lamb = [&]() { 
		Noise::play_ID(Noise::st::Button_Click, Noise::player::sfx, oxygine::PlayOptions(), true);
		if (Noise::toggle_sound_state()) {
			s_button->setText(s_button_off);
		}
		else {
			s_button->setText(s_button_on);
		}
	};
	//m_button = new C_button(Vector2(getStage()->getWidth() / 4, default_button_height), get_mb_string(), m_lamb);
	s_button = new C_button(Vector2(getStage()->getWidth() - (getStage()->getWidth() / 4), default_button_height), get_sb_string(), s_lamb, false);
	//addChild(m_button);
	addChild(s_button);
}

void Pause_Sign::activate() {
	removeTweens();
	_state = true;
	//_text->addTween(Actor::TweenRotation(MATH_PI * 2), 1000, 1);
	//_text->addTween(Actor::TweenScale(1.5f), 500, 1);
	_text->addTween(Actor::TweenAlpha(255), 1000, 1);
}

void Pause_Sign::deactivate() {
	removeTweens();
	_state = false;
	//_text->addTween(Actor::TweenRotation(-MATH_PI * 2), 1000, 1);
	//_text->addTween(Actor::TweenScale(0.0f), 500, 1);
	_text->addTween(Actor::TweenAlpha(0), 1000, 1);
	//_text->setAlpha(0);
}

void Pause_Sign::set_state(bool new_state) {
	switch (new_state) {
	case 0:
		deactivate();
		break;
	case 1:
		activate();
	}
}

bool Pause_Sign::get_state() {
	return _state;
}

Pause_Sign::Pause_Sign(const Vector2 &position) : _state(false) {
	setPosition(position);
	_text = new Text(Vector2(getStage()->getWidth() / 3.3f , -200), "Paused");
	
	_text->setColor(Default_Text_Color);
	_text->setFontSize(75);
	_text->setRotation(MATH_PI / 5);
	//_text->setAnchor(0.5f, 0.5f);
	addChild(_text);
	_text->setAlpha(0);
	
	//deactivate();
}

void Logo::tilt_right_done(Event *ev) {
	addTween(Actor::TweenRotation(-(MATH_PI / 8)), 3000, 1, false, 0, Tween::EASE::ease_inOutCubic)->setDoneCallback(CLOSURE(this, &Logo::tilt_left_done));
}

void Logo::tilt_left_done(Event *ev) {
	addTween(Actor::TweenRotation((MATH_PI / 8)), 3000, 1, false, 0, Tween::EASE::ease_inOutCubic)->setDoneCallback(CLOSURE(this, &Logo::tilt_right_done));
}

Logo::Logo(const Vector2 &position) {
	setPosition(position);
	setResAnim(res::resources.getResAnim("Logo"));
	addTween(Actor::TweenRotation((MATH_PI / 8)), 3000, 1, false, 0, Tween::EASE::ease_inOutCubic)->setDoneCallback(CLOSURE(this, &Logo::tilt_right_done));
}

Fader::Fader(const RectF &rect, spActor to_attach_to, bool should_start_black) : _fade_speed(2000) {
	setPosition(rect.pos);
	setSize(rect.getSize());
	setColor(Color::Black);
	if (should_start_black == true) {
		setAlpha(255);
	}
	else {
		setAlpha(0);
	}
	_tat = to_attach_to;
	to_attach_to->addChild(this);
}

void Fader::fade255() {
	setAlpha(0);
	_tat->addChild(this);
	addTween(Actor::TweenAlpha(255), _fade_speed);

}

void Fader::fade255(const EventCallback &ev) {
	setAlpha(0);
	_tat->addChild(this);
	spTween t = addTween(Actor::TweenAlpha(255), _fade_speed);
	t->addDoneCallback(ev);
}

void Fader::fade0() {
	setAlpha(255);
	spTween t = addTween(Actor::TweenAlpha(0), _fade_speed);
	t->detachWhenDone();
}

void World_Counter::reset() {
	update_text(0);
}

World_Counter::World_Counter(const Vector2 &position, unsigned int starting_world) {
	setPosition(position);
	setColor(Default_Text_Color);
	setText(Default_World_Text + GF::convert_to_string(starting_world));
	
	setFont(res::resources.getResFont("num_bitmap"));
}

void World_Counter::update_text(unsigned int world_number) {
	setText(Default_World_Text + GF::convert_to_string(world_number));
}

void World_Counter::play_ani() {
	//spTweenQueue tq = new TweenQueue;
	//tq->add(Actor::TweenScale(2), 200, 1);
	//tq->add(Actor::TweenScale(1), 200, 1);
	//addTween(tq);
}

void World_Counter::setWorld(unsigned int world) {
	play_ani();
	update_text(world);
}

void High_Score::find_replace(const char* file, unsigned int size, unsigned int find_score, unsigned int new_score) {
	oxygine::file::buffer data;
	oxygine::file::read(file, data);
	//data.resize(size);
	std::string s = data.getString();
	for (int i = s.size() - 1; i > 0; i -= 2) {
		unsigned int score = convert_to_u_int(s[i], s[i - 1]);
		if (score == find_score) {
			std::vector<unsigned int> conversion_data = convert_to_info(new_score);
			data.data[i] = conversion_data[0];
			data.data[i - 1] = conversion_data[1];
			break;
		}
	}
	oxygine::file::write(file, data);

}

void High_Score::set(const char* file, std::vector<unsigned int> scores, unsigned int size) {
	oxygine::file::buffer b;
	
	//b.resize(size);
	for (unsigned int i = scores.size() - 1; i > 0; i--) {
		std::vector<unsigned int> to_overwrite = convert_to_info(scores[i]);
		b.push_back(to_overwrite[1]);
		b.push_back(to_overwrite[0]);
	}
	oxygine::file::write(file, b);
}

std::vector<unsigned int> High_Score::convert_to_info(int convert_int) {
	std::vector<unsigned int> r = {0,0};
	unsigned int over = ceil(convert_int / a_max) + 1;
	unsigned int dif = (a_max * over) - convert_int;
	r[0] = over; //This is the number you should multiply by a_max to know how many a_maxs can fit into the convert_int.
	r[1] = dif; //This is the difference between the over and the convert_int. subtract this from over to find the data.
	return r;
}

unsigned int High_Score::convert_to_u_int(unsigned int over, unsigned int dif) {
	
	return (over * a_max) - dif;
}

int High_Score::updateScores(const char* file, unsigned int new_score, unsigned int size) {
	int return_int = -1; //This is returned at the end of the function.
	oxygine::file::buffer data;
	oxygine::file::read(file, data);
	//data.resize(size);
	std::string s = data.getString();
	for (int i = s.size() - 1; i > 0; i -= 2) {
		unsigned int score = convert_to_u_int(s[i], s[i - 1]);
		unsigned int next_score;
		if (i == 1) {
			next_score = new_score + 1;
		}
		else {
			next_score = convert_to_u_int(s[i - 2], s[i - 3]);
		}
		
		//This is called when it finds the best place to put the new score.
		if (new_score > score && new_score < next_score) {
			return_int = score;
			std::vector<unsigned int> new_data = convert_to_info(new_score);
			data.data[i] = new_data[0];
			data.data[i - 1] = new_data[1];
			
			break;
		}
		
	}
	oxygine::file::write(file, data);
	return return_int;
}

std::vector<int> High_Score::getTopScores(const char* file, unsigned int size) {
	std::vector<int> r_scores;
	//int number_of_platforms = 200;
	//std::vector<unsigned int> w_data = convert_to_info(number_of_platforms);
	
	//int high_score = w_data[0];
	oxygine::file::buffer data;
	//data.resize(size);
	//read entire file into buffer with stdio flags "rb" (Read Binary)
	oxygine::file::read(file, data);
	//128 max num.
	//If read num >= 128, then it will be negative.
	std::string s = data.getString();
	for (int i = s.size() - 1; i > 0; i -= 2) {
		int score = convert_to_u_int(s[i], s[i - 1]);
		r_scores.push_back(score);
	}
	//Putting data into the file.
	//data.push_back(w_data[0]);
	//data.push_back(w_data[1]);
	//Reading the data back.
	//std::string s = data.getString();
	//high_score = convert_to_u_int(w_data[0], w_data[1]);
	
	//write file back from buffer with stdio flags "wb" (Write Binary)
	oxygine::file::write(file, data);
	//int file_num = 120;
	
	
	return r_scores;
}

int High_Score::addScore(unsigned int new_score) {
	old_score = updateScores(_file, new_score, 0);
	return old_score;
}

High_Score::High_Score(const Vector2 &pos, const char* file, unsigned int new_high_score) : recent_score(-1), old_score(-1), _file(file) {
	
	
	recent_score = new_high_score;
	//old_score = updateScores(file, new_high_score, 0);
	_file = file;
	//updateScores(file, new_high_score, how_many_scores_should_be_displayed * binary_data_needed_per_slot);
	setPosition(pos);
	std::string txt = "High Scores:\n";
	setMultiline(true);
	setLinesOffset(score_text_margin);
	setFontSize(12);
	setColor(Default_Text_Color);
	updateScoreDisplay(file, file_size);
	//std::vector<int> scores = getTopScores(file, file_size);
	//Flipping the vector so that the highest scores appear at the top of the list.
	//GF::flip(scores);
	//for (unsigned int i = 0; i < how_many_scores_should_be_displayed + 1; i++) {
	//	txt.append(GF::convert_to_string(i) +  ": " + GF::convert_to_string(scores[i]) + "\n");
	//}
	setFont(res::resources.getResFont("num_bitmap"));
	setText(txt);
	setPriority(30000);
}



void High_Score::onRemovedFromStage() {

	//for (unsigned int i = 0; i < scores.size(); i++) {
	//	scores[i]->detach();
	//	scores[i].~intrusive_ptr();
	//}
}

void High_Score::updateScoreDisplay(const char* file, unsigned int size) {
	
	std::string txt = "High Scores:\n";
	std::vector<int> scores = getTopScores(file, size);
	//Flipping the vector so that the highest scores appear at the top of the list.
	GF::flip(scores);
	for (unsigned int i = 0; i < how_many_scores_should_be_displayed + 1; i++) {
		txt.append(GF::convert_to_string(i) + ": " + GF::convert_to_string(scores[i]) + "\n");
	}
	setText(txt);
}

void High_Score::reset() {
	set(_file, { 0,0,1,3,7,9,11,13 }, file_size);
	updateScoreDisplay(_file, file_size);
}

void High_Score::revert() {
	find_replace(_file, file_size, recent_score, old_score);
}

bool Menu_M::is_open() {
	return _is_open;
}

void Menu_M::close() {
	if (_is_open == true) {
		_is_open = false;
		spTween t = addTween(Actor::TweenPosition(_start_pos), ani_time, 1, false, 0, Tween::ease_inExpo);
		t->detachWhenDone(true);
	}
}

void Menu_M::open() {
	if (_is_open == false) {
		_is_open = true;
		spTween t = addTween(Actor::TweenPosition(_open_pos), ani_time, 1, false, 0, Tween::ease_inExpo);
	}
	
}

void Menu_M::close(const EventCallback &ev) {
	if (_is_open == true) {
		_is_open = false;
		spTween t = addTween(Actor::TweenPosition(_start_pos), ani_time, 1, false, 0, Tween::EASE::ease_inOutBounce);
		t->detachWhenDone(true);
		t->addDoneCallback(ev);
	}
	
}

void Menu_M::open(const EventCallback &ev) {
	if (_is_open == false) {
		_is_open = true;
		spTween t = addTween(Actor::TweenPosition(_open_pos), ani_time, 1, false, 0, Tween::EASE::ease_inOutBounce);
		t->addDoneCallback(ev);
	}
}

Win_Menu::Win_Menu(const Vector2 &open_pos, void* world_ptr, void* gui_ptr, unsigned int new_score) : _gui_ptr(gui_ptr) {
	abstract_ptr = world_ptr;
	_open_pos = open_pos;
	_gui_ptr = gui_ptr;
	_up = [&](Event* ev) {
		spTween t3 = bird->addTween(Actor::TweenPosition(Vector2(getStage()->getWidth() / 2, (getStage()->getHeight() / 2) + 30)), 4000, 1, false, 0);
		t3->addDoneCallback(_down);
	};
	_down = [&](Event* ev) {
		spTween t2 = bird->addTween(Actor::TweenPosition(Vector2(getStage()->getWidth() / 2, (getStage()->getHeight() / 2) - 30)), 4000, 1, false, 0);
		t2->addDoneCallback(_up);
	};
	//_up = up;
	//_down = down;
	bg = new Sprite;
	bg->setResAnim(res::resources.getResAnim("Space_Bg"));
	bg->setPosition(Vector2(0, 0));
	addChild(bg);
	_planet = new Planet(Vector2(200, 200), 0);
	addChild(_planet);
	congrats = new Sprite;
	congrats->setResAnim(res::resources.getResAnim("Congrats"));
	congrats->setPosition(getStage()->getWidth() / 1.9f, getStage()->getHeight() / 20);
	congrats->setRotation(MATH_PI / 8);
	addChild(congrats);
	bird = new Sprite;

	bird->addTween(Sprite::TweenAnim(res::resources.getResAnim("Final_Bird_Flapping")), 500, -1);
	bird->setPosition(getStage()->getSize() / 2);
	
	spTween t = bird->addTween(Actor::TweenPosition(Vector2(getStage()->getWidth() / 2, (getStage()->getHeight() / 2) - 30)), 1, 1, false, 0);
	t->addDoneCallback(_up);
	
	addChild(bird);
	setPosition(open_pos);
	std::function<void()> b;
	b = [&]() {are_scores_locked = true; close(); start_button->lock();
		static_cast<World*>(abstract_ptr)->set_RESET_ALL(); 
		static_cast<GUI*>(_gui_ptr)->unpause(); 
		static_cast<World*>(abstract_ptr)->unpause();  
	};
	start_button = new C_button(Vector2(getStage()->getWidth() / 2, getStage()->getHeight() - (getStage()->getHeight() / 6)), "Play Again", b);
	start_button->unlock();
	addChild(start_button);
	attachTo(start_button);
	_file = "High_scores.txt";
	score_board = new High_Score(Vector2(50, 200), _file, new_score);
	addChild(score_board);
	score_board->setPriority(1000);
	std::function<void()> lamb;
	lamb = [&]() {
		score_board->reset();
	
	};
	reset_s_button = new C_button(Vector2(getStage()->getWidth() - (getStage()->getWidth() / 6), getStage()->getHeight() - (getStage()->getHeight() / 6)), "Reset Scores", lamb);
	addChild(reset_s_button);
	setPriority(10005);
	setAlpha(0);
}

void Win_Menu::close() {
	if (_is_open == true) {
		if (are_scores_locked == false) {
			score_board->revert();
		}
		static_cast<GUI*>(_gui_ptr)->fadeIn();
		static_cast<World*>(abstract_ptr)->stop_winning();
		_is_open = false;
		spTween t = addTween(Actor::TweenAlpha(0), ani_time, 1, false, 0);
		t->detachWhenDone(true);
	}
}

void Win_Menu::close(const EventCallback &ev) {
	if (_is_open == true) {
		if (are_scores_locked == false) {
			score_board->revert();
		}
		static_cast<GUI*>(_gui_ptr)->fadeIn();
		static_cast<World*>(abstract_ptr)->stop_winning();
		_is_open = false;
		spTween t = addTween(Actor::TweenAlpha(0), ani_time, 1, false, 0);
		t->detachWhenDone(true);
		t->addDoneCallback(ev);
	}

}

void Win_Menu::open() {
	if (_is_open == false) {
		score_board->addScore(static_cast<GUI*>(_gui_ptr)->pc->get_how_many());
		_is_open = true;
		spTween t = addTween(Actor::TweenAlpha(255), ani_time * 3, 1, false, 0);
		score_board->updateScoreDisplay(_file, file_size);
		static_cast<World*>(abstract_ptr)->pause();
		//spTween t = addTween(Actor::TweenPosition(_open_pos), ani_time * 3, 1, false, 0, Tween::EASE::ease_inCubic);
	}

}

void Win_Menu::open(const EventCallback &ev) {
	if (_is_open == false) {
		score_board->addScore(static_cast<GUI*>(_gui_ptr)->pc->get_how_many());
		_is_open = true;
		spTween t = addTween(Actor::TweenAlpha(255), ani_time * 3, 1, false, 0);
		t->addDoneCallback(ev);
		score_board->updateScoreDisplay(_file, file_size);
		static_cast<World*>(abstract_ptr)->pause();
		
	}
}

Restart_Menu::Restart_Menu(const Vector2 &start_pos, const Vector2 &open_pos, void* world_ptr, void* gui_ptr, unsigned int new_score) : are_scores_locked(false) {
	
	abstract_ptr = world_ptr;
	_gui_ptr = gui_ptr;
	setPosition(start_pos);
	_start_pos = start_pos;
	_open_pos = open_pos;
	std::function<void()> b;
	b = [&]() {are_scores_locked = true; close(); start_button->lock(); 
	static_cast<World*>(abstract_ptr)->set_RESET_ALL(); static_cast<GUI*>(_gui_ptr)->unpause(); };
	start_button = new C_button(Vector2(getStage()->getWidth() / 2, default_button_height), "Try Again", b);
	start_button->unlock();
	addChild(start_button);
	//attachTo(start_button);
	_file_name = "High_scores.txt";
	score_board = new High_Score(Vector2(50, 200), _file_name, new_score);
	addChild(score_board);
	score_board->setPriority(1000);
	std::function<void()> lamb;
	lamb = [&]() {
		score_board->reset();
	};
	reset_s_button = new C_button(Vector2(getStage()->getWidth() - (getStage()->getWidth() / 6), (getStage()->getHeight() / 16)), "Reset Scores", lamb);
	addChild(reset_s_button);
}

void Restart_Menu::close() {
	if (_is_open == true) {
		if (are_scores_locked == false) {
			score_board->revert();
		}
		_is_open = false;
		spTween t = addTween(Actor::TweenPosition(_start_pos), ani_time, 1, false, 0, Tween::ease_inExpo);
		//t->detachWhenDone(true);
	}
}

void Restart_Menu::close(const EventCallback &ev) {
	if (_is_open == true) {
		if (are_scores_locked == false) {
			score_board->revert();
		}
		_is_open = false;
		spTween t = addTween(Actor::TweenPosition(_start_pos), ani_time, 1, false, 0, Tween::EASE::ease_inOutBounce);
		//t->detachWhenDone(true);
		t->addDoneCallback(ev);
	}

}

void Restart_Menu::open() {
	if (_is_open == false) {
		score_board->addScore(static_cast<GUI*>(_gui_ptr)->pc->get_how_many());
		are_scores_locked = false;
		_is_open = true;
		start_button->unlock();
		spTween t = addTween(Actor::TweenPosition(_open_pos), ani_time, 1, false, 0, Tween::ease_inExpo);
		score_board->updateScoreDisplay(_file_name, file_size);
	}

}

//This will open the start menu, and call the function given afterward. This is a little slower then normal open().
void Restart_Menu::open(const EventCallback &ev) {
	if (_is_open == false) {
		score_board->addScore(static_cast<GUI*>(_gui_ptr)->pc->get_how_many());
		are_scores_locked = false;
		_is_open = true;
		start_button->unlock();
		spTween t = addTween(Actor::TweenPosition(_open_pos), ani_time, 1, false, 0, Tween::ease_inExpo);
		score_board->updateScoreDisplay(_file_name, file_size);
		t->addDoneCallback(ev);
	}
}

char* Start_Menu::get_mb_string() {
	if (Noise::get_music_state()) {
		return m_button_off;
	}
	else {
		return m_button_on;
	}
}

char* Start_Menu::get_sb_string() {
	if (Noise::get_sound_state()) {
		return s_button_off;
	}
	else {
		return s_button_on;
	}
}

void Start_Menu::close() {
	if (_is_open == true) {
		_is_open = false;
		spTween t = addTween(Actor::TweenPosition(_start_pos), ani_time, 1, false, 0, Tween::ease_inExpo);
		t->detachWhenDone(true);
	}
}

void Start_Menu::open() {
	if (_is_open == false) {
		_is_open = true;
		//m_button->setText(get_mb_string());
		s_button->setText(get_sb_string());
		spTween t = addTween(Actor::TweenPosition(_open_pos), ani_time, 1, false, 0, Tween::ease_inExpo);
	}

}

void Start_Menu::close(const EventCallback &ev) {
	if (_is_open == true) {
		_is_open = false;
		spTween t = addTween(Actor::TweenPosition(_start_pos), ani_time, 1, false, 0, Tween::EASE::ease_inOutBounce);
		t->detachWhenDone(true);
		t->addDoneCallback(ev);
	}

}

void Start_Menu::open(const EventCallback &ev) {
	if (_is_open == false) {
		_is_open = true;
		m_button->setText(get_mb_string());
		s_button->setText(get_sb_string());
		spTween t = addTween(Actor::TweenPosition(_open_pos), ani_time, 1, false, 0, Tween::EASE::ease_inOutBounce);
		t->addDoneCallback(ev);
	}
}

Start_Menu::Start_Menu(const Vector2 &start_pos, const Vector2 &open_pos, void* gui_ptr) {
	_logo = new Logo(Vector2(getStage()->getWidth() / 2, getStage()->getHeight() / 3));
	_logo->setAnchor(0.5f, 0.5f);
	addChild(_logo);
	std::function<void()> m_lamb = [&]() { 
		if (Noise::toggle_music_state()) {
			m_button->setText(m_button_off);
		}
		else {
			m_button->setText(m_button_on);
		}
	};
	std::function<void()> s_lamb = [&]() { 
		Noise::play_ID(Noise::st::Button_Click, Noise::player::sfx, oxygine::PlayOptions(), true);
		if (Noise::toggle_sound_state()) {
			s_button->setText(s_button_off);
		}
		else {
			s_button->setText(s_button_on);
		}
	};
	s_button = new C_button(Vector2(getStage()->getWidth() - (getStage()->getWidth() / 4), getStage()->getHeight() - (getStage()->getHeight() / 6)), get_sb_string(), s_lamb, false);
	//m_button = new C_button(Vector2(getStage()->getWidth() / 4, getStage()->getHeight() - (getStage()->getHeight() / 6)), get_mb_string(), m_lamb);
	addChild(s_button);
	//addChild(m_button);
	setPosition(start_pos);
	_gui_ptr = gui_ptr;
	_start_pos = start_pos;
	_open_pos = open_pos;
	std::function<void()> b;
	b = [&]() {close(); Ship_M::objects[0]->set_is_waiting(false); static_cast<GUI*>(_gui_ptr)->unpause(); start_button->lock(); };
	start_button = new C_button(Vector2(getStage()->getWidth() / 2, getStage()->getHeight() - (getStage()->getHeight() / 6)), "Start", b);
	start_button->unlock();
	addChild(start_button);
	attachTo(start_button);
}

Platform_Counter::Platform_Counter(const Vector2 &pos) : how_many(0){
	setPosition(pos);
	setText(Default_Platform_Text + GF::convert_to_string(how_many));
	setFont(res::resources.getResFont("num_bitmap"));
}

void Platform_Counter::play_ani() {
	//spTweenQueue tq = new TweenQueue;
	//tq->add(Actor::TweenScale(2), 200, 1);
	//tq->add(Actor::TweenScale(1), 200, 1);
	//addTween(tq);
}

void Platform_Counter::count(int addend) {
	how_many += addend;
	play_ani();
	update_text();
}

void Platform_Counter::update_text() {
	setText(Default_Platform_Text + GF::convert_to_string(how_many));
}

void Platform_Counter::reset() {
	how_many = 0;
	update_text();
}

void GUI::toggle_pause() {
	//static_cast<World*>(_world_ptr)
}

GUI::GUI(unsigned int *lvl, void* player, void* world_ptr) {
	
	
	//clicker2 = new I_control(I_control::types::right);
	_world_ptr = world_ptr;
	World* w_ptr = static_cast<World*>(_world_ptr);
	wc = new World_Counter(Vector2(0, 0), w_ptr->getworldNum());
	addChild(wc);
	wc->setPriority(10000);
	pc = new Platform_Counter(Vector2(0, wc->getTextRect().getHeight()));
	pc->setColor(Default_Text_Color);
	pc->attachTo(getStage());
	addChild(pc);
	pc->setPriority(10000);
	
	clicker1 = new I_control(I_control::types::left, player);
	//clicker1->addClickListener(CLOSURE(player, &Ship_M::click_engine_left));
	clicker2 = new I_control(I_control::types::right, player);
	//clicker2->addClickListener(CLOSURE(player, &Ship_M::click_engine_right));
	addChild(clicker1);
	addChild(clicker2);
	_pause_sign = new Pause_Sign(Vector2(getStage()->getWidth() / 2, getStage()->getHeight() / 3));
	addChild(_pause_sign);
	//Creating the pause button.
	std::function<void()> b;
	b = [&]() {
		if (!w_menu->is_open() && !r_menu->is_open() && !s_menu->is_open()) {
			World* w_ptr = static_cast<World*>(_world_ptr);
			w_ptr->pause_toggle();
			_pause_sign->set_state(w_ptr->get_is_paused());
			if (w_ptr->get_is_paused()) {
				p_menu->open();
				addChild(p_menu);
			}
			else {
				p_menu->close();
			}
		}
	};
	//This is the force_lambda, it is called when the world is reset.
	std::function<void()> force_lamb;
	force_lamb = [&]() {
		World* w_ptr = static_cast<World*>(_world_ptr);
		static_cast<World*>(_world_ptr)->unpause();
		_pause_sign->set_state(w_ptr->get_is_paused());
		if (w_ptr->get_is_paused()) {
			p_menu->open();
			addChild(p_menu);
		}
		else {
			p_menu->close();
		}
	};

	pb = new P_Button(Vector2(getStage()->getWidth() - 8, 8), b, force_lamb);//new C_button(Vector2(getStage()->getWidth() - (getStage()->getWidth() / 6), 0), "Pause", b);
	addChild(pb);
	
	s_menu = new Start_Menu(Vector2(getStage()->getWidth(), 0), Vector2(0, 0), this);
	r_menu = new Restart_Menu(Vector2(getStage()->getWidth(), 0), Vector2(0, 0), world_ptr, this, pc->get_how_many());
	w_menu = new Win_Menu(Vector2(0, 0), world_ptr, this, pc->get_how_many());
	p_menu = new Pause_Menu(Vector2(getStage()->getWidth(), 0), Vector2(0, 0), world_ptr);
	addChild(p_menu);
	addChild(w_menu);
	s_menu->attachTo(getStage());
	addChild(s_menu);
	addChild(r_menu);
	//r_menu->attachTo(getStage());
	//addChild(r_menu);
	s_menu->open();
	fader = new Fader(RectF(getPosition(), getStage()->getSize()), this, false);
	//fader->fade0();
	setPriority(10000);
	//r_menu->open();
}

void GUI::unpause() {
	pb->reset();
}



void GUI::increase_p_counter(int addend) {
	pc->count(addend);
}

void GUI::reset() {
	pc->reset();
	wc->reset();
}

bool GUI::is_start_menu_open() {
	return s_menu->is_open();
}

bool GUI::is_restart_menu_open() {
	return r_menu->is_open();
}

void GUI::open_restart_menu(void* world_ptr) {
	
	if (r_menu->is_open() == false) {
		
		//r_menu = new Restart_Menu(Vector2(getStage()->getWidth(), 0), Vector2(0, 0), world_ptr, this, pc->get_how_many());
		//addChild(r_menu);
		r_menu->open();
		//addChild(r_menu);
	}
	
}

void GUI::open_restart_menu(const EventCallback &ev) {
	r_menu->open(ev);
}

void GUI::open_start_menu() {
	s_menu->open();
}

void GUI::open_start_menu(const EventCallback &ev) {
	s_menu->open(ev);
}

void GUI::close_restart_menu() {
	r_menu->close();
}

void GUI::close_restart_menu(const EventCallback &ev) {
	r_menu->close(ev);
}

void GUI::close_start_menu() {
	s_menu->close();
}

void GUI::close_start_menu(const EventCallback &ev) {
	s_menu->close(ev);
}

void GUI::open_win_menu(void* world_ptr) {
	if (r_menu->is_open() == false) {

		w_menu = new Win_Menu(Vector2(0, 0), world_ptr, this, pc->get_how_many());
		addChild(w_menu);
		w_menu->open();
	}
	
}

void GUI::open_win_menu(const EventCallback &ev) {
	w_menu->open(ev);
}

void GUI::close_win_menu() {
	w_menu->close();
}

void GUI::close_win_menu(const EventCallback &ev) {
	w_menu->close(ev);
}

void GUI::set_pc_color(Color &new_color) {
	pc->setColor(new_color);
}

void GUI::fadeOut() {
	fader->fade255();
}

void GUI::fadeOut(const EventCallback &ev) {
	fader->fade255(ev);
}

void GUI::fadeIn() {
	fader->fade0();
}

void GUI::pause() {
	pb->pause();
}

void GUI::set_world_counter(unsigned int new_world) {
	wc->setWorld(new_world);
}