
#include "random2.h"
#include <iostream>
#include <cmath>
#define float_divide_low 2
#define float_divide_high 15
using namespace random2;

extern unsigned int random2::g_seed = 0;

void random2::fast_srand(int seed)
{
	
	g_seed = seed;
}

int random2::fastrand()
{
	g_seed = (214013 * g_seed + 2531011);
	
	return (g_seed >> 16) & 0x7FFF;
}

int random2::randomrange(unsigned int lownum, unsigned int highnum) {
	
	int f = (fastrand() % highnum) + lownum;
	return f;
}

int random2::randomrange_complex(int lownum, int highnum) {
	int dif = highnum - lownum;
	dif = (fastrand() % dif) + 0;
	return lownum + dif;
}

bool random2::getBool() {
	return (fastrand() % 2);
}

float random2::randomrange_float(float lownum, float highnum) {
	int f = (fastrand() % int(ceil(highnum))) + int(ceil(lownum));
	lownum += f;
	int f2 = (fastrand() % float_divide_high) + float_divide_low;
	return f + (1 / f2);
}

float random2::range(float range_between, float given_num, float low_num, float high_num) {

	float b = (high_num - low_num) / range_between;
	return given_num / b;
}

int random2::range_complex(unsigned int range_between, int given_num, int low_num, int high_num) {
	int b = (high_num - low_num) / range_between;
	if (getBool()) {
		return -static_cast<int>(given_num / b);
	}
	return static_cast<int>(given_num / b);
}

int random2::range(unsigned int section_divide, int given_num) {
	return static_cast<int>(given_num / section_divide);
}

int random2::range_complex(unsigned int section_divide, int given_num) {
	if (getBool()) {
		return -static_cast<int>(given_num / section_divide);
	}
	return static_cast<int>(given_num / section_divide);
}