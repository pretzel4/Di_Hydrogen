#pragma once
#include "oxygine-framework.h"
#include "Particle.h"
#include "World.h"
using namespace oxygine;

/*This class can make an object that will constantly spew particles at a given location.*/
DECLARE_SMART(P_System, spP_System);
class P_System : public Actor {
private:
	//This does basic setup.
	void setup(const Vector2 &pos, const Vector2 &direction, unsigned int type, spWorld world_ptr, bool is_active);
	//This will return a vector given the angle and amplitude(hypotnuse length).
	Vector2 convert_a_to_v(float angle, float amplitude);
	//This is how strong the P_System is.
	float _amplitude = -1;
	//This is the angle.
	float _angle = -1;
	//This is a pointer to the world object.
	spWorld _world_ptr;
	
	//This is the direction at which the particles are fire.
	Vector2 _direction;
	//This is the variation; the randomization of how the particles are fired. 
	//This effects the direction and amplitude.
	float _variation = 0;
	//Type of particles.
	unsigned int _type = 0;
	//This keeps track of how long ago it created a particle.
	float ticks = 0;
	//This tells how many ticks there needs to be before creating another particle.
	float max_ticks = 1;
	//This is called every frame.
	void doUpdate(const UpdateState &us);
	//This will create a particle.
	void createParticle(const Vector2 &pos, const Vector2 &direction, unsigned int type);
	//This determines if the P_System is active.
	bool _is_active = true;
public:
	//This class can make an object that will constantly spew particles at a given location.
	P_System(const Vector2 &pos, float angle_radians, float amplitude, unsigned int type, spWorld world_ptr, bool is_active = true);
	//This class can make an object that will constantly spew particles at a given location.
	P_System(const Vector2 &pos, const Vector2 &direction, unsigned int type, spWorld world_ptr, bool is_active = true);
	
	/*-=-Setting-=-*/


	//This will activate the P_System, allowing it to spew particles.
	void set_is_active(bool is_active);
	//This will change type of the P_System.
	void set_type(unsigned int new_type);
	//This will set the angle.
	void set_angle(float new_angle);
	//This will set the amplitude.
	void set_amplitude(float new_amplitude);
	//This will set the amplitude and the angle.
	//The x should be the angle, and the y should be the amplitude.
	void set_a_and_a(const Vector2 &angle_amplitude);
	//This will set the direction.
	void set_direction(const Vector2 &new_direction);

	/*-=-Getting-=-*/


	//This will tell you if the P_System is active or not.
	bool get_is_active();
	//This will tell you what type it is.
	unsigned int get_type();
	//This will return the angle, however, if the angle has not been set, or set_direction() was called, it will return -1.
	float get_angle();
	//This will return the amplitude, however, if the angle has not been set, or set_direction() was called, it will return -1.
	float get_amplitude();
	//This will get the direction.
	Vector2 get_direction();
};