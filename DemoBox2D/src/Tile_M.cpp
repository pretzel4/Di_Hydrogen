#include "Tile_M.h"
//typedef intrusive_ptr<Tile_M> spTile_M;
//std::vector<spTile_M> Tile_M::_Cache;
spWorld Tile_M::world_ptr;



void Tile_M::init(World *World_Ptr) {
	world_ptr = World_Ptr;
}

Tile_M::Tile_M() {

}