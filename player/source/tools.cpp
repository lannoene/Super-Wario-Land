#include <fstream>
#include <string>
#include <iostream>

#include "tools.hpp"
#include "squishy_array.hpp"
#include "tiles.hpp"

extern SquishyArray <SquishyArray<Tile*>*>Room_array;

void tools::decodeLevelFileIntoMemory(std::string levelPath) {
	puts("reading level:\n");
	FILE *inf;
	struct s_level tileStr;
	struct s_level_2 tileStr2;
	inf = fopen(levelPath.c_str(), "rb");
	if (inf == NULL) {
		printf("Error. Failed to open file\n");
		return;
	}
	char magic[5] = {};
	fread(&magic, 1, 4, inf);
	//printf("magic: %s", magic);
	Room_array.array_push(new SquishyArray <Tile*>(0));
	if (memcmp(magic, "LVL1", 4) == 0) {
		puts("Reading level as LVL1:");
		int times = 0;
		while (fread(&tileStr, 1, sizeof(struct s_level), inf)) {
			Room_array.data()[0]->array_push(new Tile(tileStr.x, tileStr.y, tileStr.type));
			
			Room_array.data()[0]->data()[Room_array.data()[0]->shortLen()]->roomId = 0;
		
			switch (tileStr.type) {
				case TILE_DOOR:
					Room_array.data()[0]->data()[Room_array.data()[0]->shortLen()]->param1.dummyVar = tileStr.param1;
					Room_array.data()[0]->data()[Room_array.data()[0]->shortLen()]->param2.dummyVar = tileStr.param2;
					Room_array.data()[0]->data()[Room_array.data()[0]->shortLen()]->param3.dummyVar = tileStr.param3;
				break;
				case TILE_WARP_BLOCK:
					Room_array.data()[0]->data()[Room_array.data()[0]->shortLen()]->param1.dummyVar = tileStr.param1;
				break;
			}
			++times;
			//printf("%d\n", times);
		}
		printf("Took %d times\n", times);
	} else if (memcmp(magic, "LVL2", 4) == 0) {
		puts("Reading level as LVL2:");
		int times = 0;
		while (fread(&tileStr2, 1, sizeof(struct s_level_2), inf)) {
			if (tileStr2.roomId > (int)Room_array.shortLen()) { // rather the length be casted to an int to support negative room ids
				for (int i = 0; i < tileStr2.roomId - (int)Room_array.shortLen(); i++) {
					Room_array.array_push(new SquishyArray <Tile*>(0));
					printf("New room #%d added\n", tileStr2.roomId + i);
				}
			}
			Room_array.data()[tileStr2.roomId]->array_push(new Tile(tileStr2.x, tileStr2.y, tileStr2.type));
			
			Room_array.data()[tileStr2.roomId]->data()[Room_array.data()[tileStr2.roomId]->shortLen()]->roomId = tileStr2.roomId;
			
			switch (tileStr2.type) {
				case TILE_DOOR:
					Room_array.data()[tileStr2.roomId]->data()[Room_array.data()[tileStr2.roomId]->shortLen()]->param1.dummyVar = tileStr2.param1;
					Room_array.data()[tileStr2.roomId]->data()[Room_array.data()[tileStr2.roomId]->shortLen()]->param2.dummyVar = tileStr2.param2;
					Room_array.data()[tileStr2.roomId]->data()[Room_array.data()[tileStr2.roomId]->shortLen()]->param3.dummyVar = tileStr2.param3;
				break;
				case TILE_WARP_BLOCK:
					Room_array.data()[tileStr2.roomId]->data()[Room_array.data()[tileStr2.roomId]->shortLen()]->param1.dummyVar = tileStr2.param1;
				break;
			}
			++times;
			//printf("%d\n", times);
		}
		printf("Took %d times\n", times);
		printf("There are %lld Rooms in this level\n", Room_array.length());
	} else {
		fclose(inf);
		inf = nullptr;
		puts("Reading level as text:");
		SquishyArray <std::string*>LevelArray(0);
		//read file
		std::string rawLevelFile;
		std::string arrayDelimiter = ";";
		std::string elementDelimiter = ",";
		
		std::ifstream LevelFile(levelPath);
		
		std::getline(LevelFile, rawLevelFile);
		LevelFile.close();
		
		//decode file
		//decode arrays
		size_t pos = 0;
		while ((pos = rawLevelFile.find(arrayDelimiter)) != std::string::npos) {
			std::string token;
			LevelArray.array_push(new std::string);
			*LevelArray.data()[LevelArray.shortLen()] = rawLevelFile.substr(0, pos);
			rawLevelFile.erase(0, pos + arrayDelimiter.length());
			
			
		}
		
		//decode array-elements
		SquishyArray <std::string*>LevelEntries(0);
		for (size_t i = 0; i < LevelArray.length(); i++) {
			pos = 0;
			while ((pos = LevelArray.data()[i]->find(elementDelimiter)) != std::string::npos) {
				std::string token;
				LevelEntries.array_push(new std::string);
				*LevelEntries.data()[LevelEntries.shortLen()] = LevelArray.data()[i]->substr(0, pos);
				LevelArray.data()[i]->erase(0, pos + elementDelimiter.length());
			}
			
			
		}

		const int entriesNum = 5;
		for (size_t j = 0; j < LevelEntries.length()/entriesNum; j++) {
			//std::cout << std::stoi(*LevelEntries.data()[j]) << " " << std::stoi(*LevelEntries.data()[j*entriesNum + 1]) << std::endl;
			
			Room_array.data()[0]->array_push(new Tile(std::stoi(*LevelEntries.data()[j*entriesNum]), std::stoi(*LevelEntries.data()[j*entriesNum + 1]), std::stoi(*LevelEntries.data()[j*entriesNum + 2])));
			
		
			switch (std::stoi(*LevelEntries.data()[j*entriesNum + 2])) {
				default:
					Room_array.data()[0]->data()[Room_array.data()[0]->shortLen()]->param1.dummyVar = 0;
					Room_array.data()[0]->data()[Room_array.data()[0]->shortLen()]->param2.dummyVar = 0;
					Room_array.data()[0]->data()[Room_array.data()[0]->shortLen()]->param3.dummyVar = 0;
				break;
				case TILE_DOOR:
				case TILE_WARP_BLOCK:
					Room_array.data()[0]->data()[Room_array.data()[0]->shortLen()]->param1.warpId = std::stoi(*LevelEntries.data()[j*entriesNum + 3]);
					Room_array.data()[0]->data()[Room_array.data()[0]->shortLen()]->param2.destinationWarpId = std::stoi(*LevelEntries.data()[j*entriesNum + 4]);
				break;
			}
		}
	}
	if (inf) fclose(inf);
}

void tools::resetLevel(void) {
	for (size_t j = 0; j < Room_array.length(); j++) {
		for (size_t i = 0; i < Room_array.data()[j]->length(); i++) {
			delete Room_array.data()[j]->data()[i];
		}
		Room_array.data()[j]->array_splice(0, Room_array.data()[j]->length());
	}
	Room_array.array_splice(0, Room_array.length() - 1);
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