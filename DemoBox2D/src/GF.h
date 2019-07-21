#pragma once
#include "Box2D\Box2D.h"
#include "oxygine-framework.h"
#include <iostream>
#include <string>

using namespace oxygine;


namespace GF {
	//Converts degrees to radians.
	float rad(float degree);
	//Converts radians to degrees.
	float degrees(float radians);
	//This returns a global scale.
	float get_G_Scale();
	//Converts a b2Vec2 to Vector2.
	Vector2 convert(const b2Vec2& pos, float SCALE = get_G_Scale());
	//Converts a Vector2 to b2Vec2.
	b2Vec2 convert(const Vector2& pos, float SCALE = get_G_Scale());
	//Converts a single b2 float into a Oxygine_Vector2 float.
	float convert_to_Oxy(float b2, float SCALE = get_G_Scale());
	//Converts a single Oxy float into a b2 float.
	float convert_to_b2(float Oxy, float SCALE = get_G_Scale());
	//This will print out a variable, and will skip to the next line.
	void println_va(int var);
	//This will print out a variable, and will NOT skip to the next line.
	void print_va(int var);
	//This will convert a string to a float.
	float StringtoFloat(std::string string);
	//This will convert the given string into an integer and return it.
	int StringtoInt(std::string string);
	//This will convert the given string into an unsigned int and return it.
	unsigned int StringtoUnint(std::string string);
	//This returns the distance the two cordinates are.
	float getDistance(const Vector2 &position1, const Vector2 &position2);
	//This tells if the index of the vector given is a legal move.
	//returns true if index exists.
	//returns false if index does not exist.
	bool does_exist(int index, int size_of_vector);
	//This gets the average between two numbers.
	float average(float num1, float num2);
	//This will populate an array of size four.
	void populate4(unsigned int ar[4], unsigned int r, unsigned int b, unsigned int g, unsigned int alpha);
	//This will turn the given integer into a string.
	std::string convert_to_string(int number_to_convert);
	//This will flip a vector.
	void flip(std::vector<int> &vec);
	//This will put two fixtures together.
	b2Joint *weld(b2JointDef *jd, void* world_ptr);
	/*This will determine if the tween is NULL or not. If it is, then this function will do nothing; 
	otherwise, it will inflict the given tween upon the given actor.*/
	void inflict(spActor actor, Tween* tween);
	//This will make the actor fade in, given the time and the actor.
	void fadeIn(Actor* actor, unsigned int time, unsigned int max = 255);
	//This will make the actor fade out, given the time and the actor.
	void fadeOut(Actor* actor, unsigned int time, unsigned int min = 0);
	//This will return the hypotnuse of a right triangle given the two legs.
	float pyagor_hypot(float leg1, float leg2);
	
	
}