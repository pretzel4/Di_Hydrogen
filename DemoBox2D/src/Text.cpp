#include "Text.h"
#include "ImageRes.h"
Text::Text(const Vector2 &pos, const std::string &string) {
	setPosition(pos);
	setText(string);
	
	setFont(res::resources.getResFont("num_bitmap"));
}

Text::Text(const Vector2 &pos, int number) {
	std::string s = std::to_string(number);
	setPosition(pos);
	setText(s);
	
	setFont(res::resources.getResFont("num_bitmap"));
}

void Text::setNum(int number) {
	setText(std::to_string(number));
}