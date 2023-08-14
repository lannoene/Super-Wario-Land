#pragma once

#include <SDL2/SDL.h>

#include "draw.hpp"
#include "audio.hpp"
#include "room.hpp"

bool runPauseScreen(SDL_Screen &Scene, SDL_Audio &Audio);

enum BUTTON_IDS {
	NO_BUTTON = 0,
	SLIDER_SFX,
	SLIDER_MUS,
	BUTTON_EXIT,
};