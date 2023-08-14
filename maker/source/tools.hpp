#pragma once

#include "draw.hpp"
#include "room.hpp"

namespace tools {
	void decodeLevelFileIntoMemory(std::string levelPath, SquishyArray <Room*>&Room_array);
	void resetLevel(SquishyArray <Room*>&Room_array);
	void saveLevel(SDL_Screen &Scene, SquishyArray <Room*>&Room_array);
}

// 1: LVL3 (magic, obviously set size)
// 2: lvl_info_header_struct (set size, one struct)
// 3: lvl_info_room (unknown amount, amount defined in lvl_info_header)
// 4: lvl_info_tile (uknown amount, amount defined in corresponding lvl_info_room [corresponding ids], read through one room's tiles @ a time)

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

bool testFileNameForWin32(std::string fileName);