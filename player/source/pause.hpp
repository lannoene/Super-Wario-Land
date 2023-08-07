#pragma once

#include <SDL2/SDL.h>

#include "draw.hpp"
#include "audio.hpp"

bool pauseScreen(SDL_Screen &Scene, SDL_Audio &Audio);

enum BUTTON_IDS {
	NO_BUTTON = 0,
	SLIDER_SFX,
	SLIDER_MUS,
};