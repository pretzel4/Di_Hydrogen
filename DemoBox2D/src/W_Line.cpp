#include "W_Line.h"
#include "Text.h"
#define width_of_line 10
W_Line::W_Line(World *world_ptr, const Vector2 &ref_pos) {
	setPosition(world_ptr->get_boundary_left() - ref_pos.x, ref_pos.y);
	setAnchor(0, 0.5f);
	print = new Text(Vector2(0, -20), "END OF LEVEL");
	char* txt = " END OF LEVEL ";
	std::string str;
	int number_of_repeats = (round(world_ptr->get_boundary_size() / print->getTextRect().getWidth())) + 1;
	for (unsigned int i = 0; i < number_of_repeats; i++) {
		str.append(txt);
	}
	print->setText(str);
	logs::message("size_Data:");
	
	
	

	print->setSize(Vector2(world_ptr->get_boundary_size(), 40));
	addChild(print);
	setAlpha(100);
	setSize(Vector2(world_ptr->get_boundary_size(), 10));
	setColor(Color::Crimson);
	attachTo(getStage());
}

void W_Line::destroy() {
	print->detach();
	detach();
}