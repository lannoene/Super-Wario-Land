#include <fstream>
#include <string>
#include <iostream>

#include "tools.hpp"
#include "squishy_array.hpp"
#include "tiles.hpp"

extern SquishyArray <Tile*>Tile_array;

void tools::decodeLevelFileIntoMemory(std::string levelPath) {
	puts("reading level:\n");
	FILE *inf;
	struct s_level tileStr;
	inf = fopen(levelPath.c_str(), "rb");
	if (inf == NULL) {
		printf("Error. Failed to open file");
		return;
	}
	char magic[5] = {};
	fread(&magic, 1, 4, inf);
	//printf("magic: %s", magic);
	if (memcmp(magic, "LVL1", 4)==0) {
		puts("Reading level as LVL1:");
		int times = 0;
		while (fread(&tileStr, 1, sizeof(struct s_level), inf)) {
			Tile_array.array_push(new Tile(tileStr.x, tileStr.y, tileStr.type));
		
			switch (tileStr.type) {
				case TILE_DOOR:
					Tile_array.data()[Tile_array.shortLen()]->param1.dummyVar = tileStr.param1;
					Tile_array.data()[Tile_array.shortLen()]->param2.dummyVar = tileStr.param2;
				break;
				case TILE_WARP_BLOCK:
					Tile_array.data()[Tile_array.shortLen()]->param1.dummyVar = tileStr.param1;
				break;
			}
			++times;
			//printf("%d\n", times);
		}
		printf("Took %d times\n", times);
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
			
			Tile_array.array_push(new Tile(std::stoi(*LevelEntries.data()[j*entriesNum]), std::stoi(*LevelEntries.data()[j*entriesNum + 1]), std::stoi(*LevelEntries.data()[j*entriesNum + 2])));
			
		
			switch (std::stoi(*LevelEntries.data()[j*entriesNum + 2])) {
				default:
					Tile_array.data()[Tile_array.shortLen()]->param1.dummyVar = 0;
					Tile_array.data()[Tile_array.shortLen()]->param2.dummyVar = 0;
					Tile_array.data()[Tile_array.shortLen()]->param3.dummyVar = 0;
				break;
				case TILE_DOOR:
				case TILE_WARP_BLOCK:
					Tile_array.data()[Tile_array.shortLen()]->param1.warpId = std::stoi(*LevelEntries.data()[j*entriesNum + 3]);
					Tile_array.data()[Tile_array.shortLen()]->param2.destinationWarpId = std::stoi(*LevelEntries.data()[j*entriesNum + 4]);
				break;
			}
		}
	}
	if (inf) fclose(inf);
}

void tools::resetLevel(void) {
	for (size_t i = 0; i < Tile_array.length(); i++) {
		delete Tile_array.data()[i];
	}
	Tile_array.array_splice(0, Tile_array.shortLen());
}

bool tools::checkVertBounds(float inputChecking_x, float inputChecking_y, float input_width, float input_height, float inputCheckAgainst_x, float inputCheckAgainst_y, float destination_size_x, float destination_size_y) {
	if (inputChecking_x + input_width > inputCheckAgainst_x && inputChecking_x < inputCheckAgainst_x + destination_size_x && inputChecking_y + input_height >= inputCheckAgainst_y && inputChecking_y < inputCheckAgainst_y + destination_size_y) {
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