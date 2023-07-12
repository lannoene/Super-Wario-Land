#pragma once
#include "draw.hpp"
#include "player.hpp"

#define TILESIZE 50

//put walls & stuff here
extern float cameraHorizOffsetPx;

class Tile {
public:
	int x = 0;
	int y = 0;
	int width;
	int height;
	
	union uparam1 {
		int doorId;
		int scrollstopId;
		int dummyVar;
	} param1;
	union uparam2 {
		int destinationDoorId;
		int dummyVar;
	} param2;
	union uparam3 {
		int dummyVar;
	} param3;
	
	Tile(int input_x, int input_y, int type);
	~Tile();
	void draw(SDL_Screen &Scene);
	int getType(void);
	bool colidable;
	void update(size_t frame, Player &player);
private:
	int type;
	bool visible;
};

enum tileType {
	TILE_SPAWN_POINT = -1,
	TILE_GRASS_TOP = 0,
	TILE_GRASS_TOP_RIGHT,
	TILE_GRASS_TOP_LEFT,
	TILE_GRASS_MIDDLE_MIDDLE,
	TILE_GRASS_MIDDLE_RIGHT,
	TILE_GRASS_MIDDLE_LEFT,
	TILE_GRASS_BOTTOM_MIDDLE,
	TILE_GRASS_BOTTOM_RIGHT,
	TILE_GRASS_BOTTOM_LEFT,
	TILE_GRASS_CONNECTOR_TOP_TO_RIGHT,
	TILE_GRASS_CONNECTOR_TOP_TO_LEFT,
	TILE_GRASS_CONNECTOR_BOTTOM_TO_RIGHT,
	TILE_GRASS_CONNECTOR_BOTTOM_TO_LEFT,
	GOLD_COIN,
	TILE_DOOR,
	TILE_DOOR_BOTTOM,
	TILE_DIRT_BG,
	TILE_DIRT_BG_SHADOW
};