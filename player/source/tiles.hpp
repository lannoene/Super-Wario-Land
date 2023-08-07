#pragma once
#include "draw.hpp"
#include "player.hpp"

#define TILESIZE 50

//put walls & stuff here
extern float cameraHorizOffsetPx;

class Tile {
public:
	float x = 0;
	float y = 0;
	int width;
	int height;
	
	union uparam1 {
		int warpId;
		int scrollstopId;
		int dummyVar;
	} param1;
	union uparam2 {
		int destinationWarpId;
		int dummyVar;
	} param2;
	union uparam3 {
		int destinationRoomId;
		int dummyVar;
	} param3;
	
	Tile(int input_x, int input_y, int type);
	virtual ~Tile();
	void draw(SDL_Screen &Scene, int gameFrame);
	int getType(void);
	struct s_flags {
		bool colidable;
		bool semisolid;
		bool breakable;
		bool ladder;
		bool visible;
		bool water;
		bool warp;
	} flags;
	bool colidable;
	bool semisolid;
	bool breakable;
	bool ladder;
	bool visible;
	bool water;
	virtual void update(size_t frame, Player &player);
	float tileVertVect = 0;
	int tileId;
	int roomId;
protected:
	int type;
	int animTimer, animDelay;
};

class PhysicsTile : public Tile {
public:
	PhysicsTile(int input_x, int input_y, int type);
	~PhysicsTile(void);
	void update(size_t frame, Player &player) override;
	
	float tileVertVect = 0;
	int tileId;
private:
	float elasticity = 0.5;
	float tileHorizVect;
	float tileDeceleration[2];
	bool isVertStationary;
};

enum tileType {
	TILE_WARP_BLOCK = -2,
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
	TILE_PURPLE_SMALL_CRACKS,
	TILE_GRASS_LADDER,
	TILE_WATER_MIDDLE,
	TILE_WATER_TOP,
};

struct s_level {
	int x;
	int y;
	int type;
	int param1;
	int param2;
	int param3;
	char numThing[20];
};

struct s_level_2 {
	int x;
	int y;
	int type;
	int param1;
	int param2;
	int param3;
	int roomId;
	char numThing[20];
};