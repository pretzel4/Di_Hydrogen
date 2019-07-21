#include "Debug.h"
#include "GF.h"
#define spacer "=-="
std::string Debug::to_print;

void Debug::print(char* string) {
	to_print.append(spacer + *string);
}

void Debug::print(int var) {
	to_print.append(spacer + GF::convert_to_string(var));
}

void Debug::publish() {
	logs::messageln(to_print.c_str());
	to_print.clear();
}