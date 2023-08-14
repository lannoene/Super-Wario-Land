#pragma once
#include "room.hpp"

namespace tools {
	void decodeLevelFileIntoMemory(std::string levelPath, SquishyArray <Room*>&Room_array);
	void resetLevel(SquishyArray <Room*>&Room_array);
	bool checkVertBounds(float inputChecking_x, float inputChecking_y, float input_width, float input_height, float inputCheckAgainst_x, float inputCheckAgainst_y, float destination_size_x, float destination_size_y);
	bool checkHorizBounds(float inputChecking_x, float inputChecking_y, float input_width, float input_height, float inputCheckAgainst_x, float inputCheckAgainst_y, float destination_size_x, float destination_size_y);
	void setPlayerPosToSpawnPoint(SquishyArray <Room*>&Room_array);
}

struct lvl_info_header {
	int numOfRooms;
	
};

struct lvl_info_room {
	int numOfTiles;
	int roomId;
	int width, height;
	int songId;
	int backgroundId;
};

struct lvl_info_tile {
	int x;
	int y;
	int type;
	int param1;
	int param2;
	int param3;
	int roomId;
	char numThing[20];
};