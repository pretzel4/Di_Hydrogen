#include "prime.h"
#include "PlayButton.h"
#include "oxygine-framework.h"
#include "ImageRes.h"
#include "World.h"
#include "Wall.h"
#include "Tile_M.h"
#include <iostream>
#include <string>
#include "Platform.h"
#include "Background.h"
#include "Particle.h"
#include "GUI_Decor_M.h"
#include "Ground.h"
#include "Noise.h"
#include "Debug.h"

using namespace oxygine;
void prime_update() {
	
	Particle::tick();
	GUI_Decor_M::tick();
	Noise::update();
	//Debug::publish();
}

void prime_preinit(){
	Background::pre_init();
}

void prime_init() {
	
	
	//You need to initialize the noise namespace before initializing the resources.
	Noise::init();
	//Loading the resources.
	res::load();
	

	//Defining the world intrusive_ptr
	

	

	//Creating the world
	//Everything that needs the world to initialize is within the world constructor.
	spWorld world = new World;
	getStage()->addChild(world);
	
	
	
}

void prime_destroy() {
	res::free();
	Noise::clean();
	Platform::stop_tweens();
}