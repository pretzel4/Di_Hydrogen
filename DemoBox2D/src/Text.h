#pragma once
#include "oxygine-framework.h"
using namespace oxygine;
DECLARE_SMART(Text, spText);
class Text : public TextField {
private:


public:
	Text(const Vector2 &pos, const std::string &string);
	Text(const Vector2 &pos, int number);
	//This will convert the given number into a string and set it to the text's string.
	void setNum(int number);
};
