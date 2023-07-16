#pragma once
#include "draw.hpp"

#define TILESIZE 50

//put walls & stuff here
extern float cameraHorizOffsetPx;
extern float cameraVertOffsetPx;

class Tile {
public:
	int x = 0;
	int y = 0;
	int width;
	int height;
	
	union param1 {
		int doorId;
		int scrollstopId;
		int dummyVar = 0;
	} param1;
	union param2 {
		int destinationDoorId;
		int dummyVar = 0;
	} param2;
	union param3 {
		int dummyVar = 0;
	} param3;
	
	Tile(int input_x, int input_y, int type);
	~Tile();
	void draw(SDL_Screen &Scene);
	int getType(void);
	std::string appendOutputToString(void);
	void setParam(int paramId, int value);
private:
	int type;
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
	TILE_DIRT_BG_SHADOW,
	TILE_DIRT_SEMISOLID,
	TILE_LOG_MIDDLE_FAR_LEFT,
	TILE_LOG_MIDDLE_INNER_LEFT,
	TILE_LOG_MIDDLE_INNER_RIGHT,
	TILE_LOG_MIDDLE_FAR_RIGHT,
	TILE_LOG_BOTTOM_FAR_LEFT,
	TILE_LOG_BOTTOM_INNER_LEFT,
	TILE_LOG_BOTTOM_INNER_RIGHT,
	TILE_LOG_BOTTOM_FAR_RIGHT,
	TILE_PURPLE_SMALL_CRACKS
};