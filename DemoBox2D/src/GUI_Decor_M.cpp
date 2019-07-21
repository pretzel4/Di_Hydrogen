#include "GUI_Decor_M.h"
std::vector<spActor> GUI_Decor_M::_objects;
std::vector<spActor> GUI_Decor_M::_to_delete;


void GUI_Decor_M::clear_cache() {
	for (unsigned int i = 0; i < _to_delete.size(); i++) {
		_to_delete[i]->detach();
		//_to_delete[i].~intrusive_ptr();
		//Finding and destroying the pointer in the _objects vector.
		for (unsigned int b = 0; b < _objects.size(); b++) {
			_objects.erase(_objects.begin() + b);
			break;
		}

	}
	_to_delete.clear();
}

void GUI_Decor_M::tick() {
	clear_cache();
}

void GUI_Decor_M::tween_d_check(Event *ev) {
	d_check();
}

void GUI_Decor_M::d_check() {
	if (getParent() != NULL) {
		detach();
	}
}

void GUI_Decor_M::fadeOut(unsigned int milliseconds) {
	if (getParent() != NULL) {
		spTween s = addTween(Actor::TweenAlpha(0), milliseconds);
		s->setDoneCallback(CLOSURE(this, &GUI_Decor_M::tween_d_check));
	}
	
	
}

void GUI_Decor_M::fadeIn(Actor* to_attach, unsigned int milliseconds) {
	
	attachTo(to_attach);
	addTween(Actor::TweenAlpha(255), milliseconds);
}