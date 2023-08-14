#include <fstream>
#include <string>
#include <iostream>

#include "tools.hpp"
#include "squishy_array.hpp"
#include "tiles.hpp"
#include "room.hpp"
#include "player.hpp"

void tools::decodeLevelFileIntoMemory(std::string levelPath, SquishyArray <Room*>&Room_array) {
	puts("reading level:\n");
	FILE *inf;
	inf = fopen(levelPath.c_str(), "rb");
	if (inf == NULL) {
		printf("Error. Failed to open file\n");
		Room_array.array_push(new Room);
		return;
	}
	char magic[5] = {};
	fread(&magic, 1, 4, inf);
	//printf("magic: %s", magic);
	if (memcmp(magic, "LVL1", 4) == 0) {
		puts("Reading level as LVL1:");
		Room_array.array_push(new Room);
		struct s_level tileStr;
		int times = 0;
		while (fread(&tileStr, 1, sizeof(struct s_level), inf)) {
			Room_array.data()[0]->Room_tiles.array_push(new Tile(tileStr.x, tileStr.y, tileStr.type));
			switch (tileStr.type) {
				case TILE_DOOR:
					Room_array.data()[0]->Room_tiles.data()[Room_array.data()[0]->Room_tiles.shortLen()]->param1.dummyVar = tileStr.param1;
					Room_array.data()[0]->Room_tiles.data()[Room_array.data()[0]->Room_tiles.shortLen()]->param2.dummyVar = tileStr.param2;
				break;
				case TILE_WARP_BLOCK:
					Room_array.data()[0]->Room_tiles.data()[Room_array.data()[0]->Room_tiles.shortLen()]->param1.dummyVar = tileStr.param1;
				break;
			}
			++times;
			//printf("%d\n", times);
		}
		printf("Took %d times\n", times);
	} else if (memcmp(magic, "LVL2", 4) == 0) {
		puts("Reading level as LVL2:");
		Room_array.array_push(new Room);
		puts("LVL2 files are not currently supported.");
		return;
		/*struct s_level_2 tileStr2;
		int times = 0;
		while (fread(&tileStr2, 1, sizeof(struct s_level_2), inf)) {
			if (tileStr2.roomId > Room_array_temp.shortLen()) {
				for (int i = 0; i < tileStr2.roomId - (int)Room_array_temp.shortLen(); i++) {
					Room_array_temp.array_push(new SquishyArray <Tile*>(0));
					printf("New room #%d added\n", tileStr2.roomId + i);
				}
			}
			Room_array_temp.data()[tileStr2.roomId]->array_push(new Tile(tileStr2.x, tileStr2.y, tileStr2.type));
		
			switch (tileStr2.type) {
				case TILE_DOOR:
					Room_array_temp.data()[tileStr2.roomId]->data()[Room_array_temp.data()[tileStr2.roomId]->shortLen()]->param1.dummyVar = tileStr2.param1;
					Room_array_temp.data()[tileStr2.roomId]->data()[Room_array_temp.data()[tileStr2.roomId]->shortLen()]->param2.dummyVar = tileStr2.param2;
					Room_array_temp.data()[tileStr2.roomId]->data()[Room_array_temp.data()[tileStr2.roomId]->shortLen()]->param3.dummyVar = tileStr2.param3;
				break;
				case TILE_WARP_BLOCK:
					Room_array_temp.data()[tileStr2.roomId]->data()[Room_array_temp.data()[tileStr2.roomId]->shortLen()]->param1.dummyVar = tileStr2.param1;
				break;
				case TILE_SPAWN_POINT:
					Room_array_temp.data()[tileStr2.roomId]->data()[Room_array_temp.data()[tileStr2.roomId]->shortLen()]->param2.dummyVar = tileStr2.param2;
					Room_array_temp.data()[tileStr2.roomId]->data()[Room_array_temp.data()[tileStr2.roomId]->shortLen()]->param3.dummyVar = tileStr2.param3;
				break;
			}
			++times;
			//printf("%d\n", times);
		}
		printf("Took %d times\n", times);
		printf("There are %d Rooms in this level\n", Room_array_temp.length());*/
	} else if (memcmp(magic, "LVL3", 4) == 0) {
		struct lvl_info_header levelHeaderInp;
		fread(&levelHeaderInp, 1, sizeof(struct lvl_info_header), inf);
		printf("Number of rooms: %d\n", levelHeaderInp.numOfRooms);
		SquishyArray <lvl_info_room>roomHeadersArray(0);
		
		for (size_t i = 0; i < levelHeaderInp.numOfRooms; i++) {
			struct lvl_info_room roomHeaderInp;
			fread(&roomHeaderInp, 1, sizeof(struct lvl_info_room), inf);
			printf("Room %lld has %d tiles\n", i, roomHeaderInp.numOfTiles);
			Room_array.array_push(new Room);
			Room_array.data()[Room_array.shortLen()]->setRoomWidth(roomHeaderInp.width);
			Room_array.data()[Room_array.shortLen()]->setRoomHeight(roomHeaderInp.height);
			Room_array.data()[Room_array.shortLen()]->roomSetMusic(roomHeaderInp.songId);
			Room_array.data()[Room_array.shortLen()]->roomSetBackground(roomHeaderInp.backgroundId);
			printf("New room ID: %lld added\n", Room_array.shortLen());
			roomHeadersArray.array_push(roomHeaderInp);
		}
		
		for (size_t j = 0; j < Room_array.length(); j++) {
			for (size_t i = 0; i < roomHeadersArray.data()[j].numOfTiles; i++) {
				struct lvl_info_tile tileStruct;
				fread(&tileStruct, 1, sizeof(struct lvl_info_tile), inf);
				Room_array.data()[j]->Room_tiles.array_push(new Tile(tileStruct.x, tileStruct.y, tileStruct.type));
				Room_array.data()[j]->Room_tiles.data()[Room_array.data()[j]->Room_tiles.shortLen()]->roomId = j;
				switch (tileStruct.type) {
					case TILE_DOOR:
						Room_array.data()[j]->Room_tiles.data()[Room_array.data()[j]->Room_tiles.shortLen()]->param1.dummyVar = tileStruct.param1;
						Room_array.data()[j]->Room_tiles.data()[Room_array.data()[j]->Room_tiles.shortLen()]->param2.dummyVar = tileStruct.param2;
						Room_array.data()[j]->Room_tiles.data()[Room_array.data()[j]->Room_tiles.shortLen()]->param3.dummyVar = tileStruct.param3;
					break;
					case TILE_WARP_BLOCK:
						Room_array.data()[j]->Room_tiles.data()[Room_array.data()[j]->Room_tiles.shortLen()]->param1.dummyVar = tileStruct.param1;
					break;
					case TILE_SPAWN_POINT:
						Room_array.data()[j]->Room_tiles.data()[Room_array.data()[j]->Room_tiles.shortLen()]->param2.dummyVar = tileStruct.param2;
						Room_array.data()[j]->Room_tiles.data()[Room_array.data()[j]->Room_tiles.shortLen()]->param3.dummyVar = tileStruct.param3;
					break;
				}
			}
		}
	} else {
		puts("Unknown file type");
	}
	if (inf) fclose(inf);
}

void tools::resetLevel(SquishyArray <Room*>&Room_array) {
	for (int j = 0; j < Room_array.length(); j++) {
		for (int i = 0; i < Room_array.data()[j]->Room_tiles.length(); i++) {
			delete Room_array.data()[j]->Room_tiles.data()[i];
		}
		Room_array.data()[j]->Room_tiles.array_splice(0, Room_array.data()[j]->Room_tiles.length());
	}
	Room_array.array_splice(0, Room_array.length());
}

bool tools::checkVertBounds(float inputChecking_x, float inputChecking_y, float input_width, float input_height, float inputCheckAgainst_x, float inputCheckAgainst_y, float destination_size_x, float destination_size_y) {
	if (inputChecking_x + input_width > inputCheckAgainst_x && inputChecking_x < inputCheckAgainst_x + destination_size_x && inputChecking_y + input_height >= inputCheckAgainst_y && inputChecking_y <= inputCheckAgainst_y + destination_size_y) {
		return true;
	} else {
		return false;
	}
}

bool tools::checkHorizBounds(float inputChecking_x, float inputChecking_y, float input_width, float input_height, float inputCheckAgainst_x, float inputCheckAgainst_y, float destination_size_x, float destination_size_y) {
	if (inputChecking_x + input_width >= inputCheckAgainst_x && inputChecking_x <= inputCheckAgainst_x + destination_size_x && inputChecking_y + input_height > inputCheckAgainst_y && inputChecking_y < inputCheckAgainst_y + destination_size_y) {
		return true;
	} else {
		return false;
	}
}

void setPlayerPosToSpawnPoint(SquishyArray <Room*>&Room_array) {

}