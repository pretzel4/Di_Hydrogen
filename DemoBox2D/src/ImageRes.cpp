#include "ImageRes.h"

namespace res {
	Resources resources;
	void load() {
		resources.loadXML("res.xml");
	}
	void free() {
		resources.free();
	}
}