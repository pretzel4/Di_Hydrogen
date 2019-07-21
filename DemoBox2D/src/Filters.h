#pragma once
#include "oxygine-framework.h"
using namespace oxygine;
class Filters {

protected:
	enum filters {
		DEFAULT_CAT = 0x0001,
		DEFAULT_MASK = 0xFFFF,
		BLACK_HOLE = 0x01,
		CHAINSAW = 0x02,
		SHIP_PART = 0x04,
		PLATFORM = 0x08,
		FORCE_FIELD = 0x16,
		SPIKES = 0x32,
		ST = 0x64,
		PARTICLE = 0x128 
	};
};