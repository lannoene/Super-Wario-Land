#pragma once

#include "draw.hpp"

namespace tools {
	void decodeLevelFileIntoMemory(std::string levelPath);
	void resetLevel(void);
	void saveLevel(SDL_Screen &Scene);
}

bool testFileNameForWin32(std::string fileName);