#pragma once

#include <iostream>

#include "draw.hpp"
#include "audio.hpp"
#include "room.hpp"

bool drawGame(SDL_Screen &Scene, SDL_Audio &Audio);
void loadLevel(std::string levelPath, SDL_Audio &Audio);
void unloadCurrentLevel(SDL_Audio &Audio);