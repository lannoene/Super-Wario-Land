#include <fstream>
#include <string>
#include <iostream>

#include "tools.hpp"
#include "squishy_array.hpp"
#include "tiles.hpp"

extern SquishyArray <Tile*>Tile_array;

void tools::decodeLevelFileIntoMemory(std::string levelPath) {
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
		*LevelArray.data()[LevelArray.length() - 1] = rawLevelFile.substr(0, pos);
		rawLevelFile.erase(0, pos + arrayDelimiter.length());
		
		
	}
	
	//decode array-elements
	for (int i = 0; i < LevelArray.length(); i++) {
		std::cout << *LevelArray.data()[i] << std::endl;
	}
	
	
	SquishyArray <std::string*>LevelEntries(0);
	for (size_t i = 0; i < LevelArray.length(); i++) {
		pos = 0;
		while ((pos = LevelArray.data()[i]->find(elementDelimiter)) != std::string::npos) {
			std::string token;
			LevelEntries.array_push(new std::string);
			*LevelEntries.data()[LevelEntries.length() - 1] = LevelArray.data()[i]->substr(0, pos);
			LevelArray.data()[i]->erase(0, pos + elementDelimiter.length());
		}
		
		
	}
	
	const int entriesNum = 5; // >=3 <<<<----- a face!!!!! =3
	for (int j = 0; j < LevelEntries.length()/entriesNum; j++) {
		std::cout << std::stoi(*LevelEntries.data()[j]) << " " << std::stoi(*LevelEntries.data()[j*entriesNum + 1]) << std::endl;
		Tile_array.array_push(new Tile(std::stoi(*LevelEntries.data()[j*entriesNum]), std::stoi(*LevelEntries.data()[j*entriesNum + 1]), std::stoi(*LevelEntries.data()[j*entriesNum + 2])));
	
		switch (std::stoi(*LevelEntries.data()[j*entriesNum + 2])) {
			default:
				Tile_array.data()[Tile_array.length() - 1]->param1.dummyVar = 0;
				Tile_array.data()[Tile_array.length() - 1]->param2.dummyVar = 0;
				Tile_array.data()[Tile_array.length() - 1]->param3.dummyVar = 0;
			break;
			case TILE_DOOR:
				Tile_array.data()[Tile_array.length() - 1]->param1.doorId = std::stoi(*LevelEntries.data()[j*entriesNum + 3]);
				Tile_array.data()[Tile_array.length() - 1]->param2.destinationDoorId = std::stoi(*LevelEntries.data()[j*entriesNum + 4]);
			break;
		}
	}
}

void tools::resetLevel(void) {
	for (int i = 0; i < Tile_array.length(); i++) {
		delete Tile_array.data()[i];
	}
	Tile_array.array_splice(0, Tile_array.length());
}