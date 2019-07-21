#pragma once
#include "oxygine-framework.h"
#include <iostream>
using namespace oxygine;
namespace Debug {
	extern std::string to_print;
	//This will add an input to be printed.
	void print(char* string);
	//This will add an input to be printed.
	void print(int var);
	//This will print everything that was printed during that frame.
	//This should be called every frame.
	void publish();

}