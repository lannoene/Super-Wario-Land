#pragma once
#include <SDL2/SDL.h>

#include "draw.hpp"
#include "audio.hpp"
#include "map.hpp"

class base {
public:
	virtual bool draw(SDL_Screen &Scene, SDL_Audio &Audio) {
		std::cout << "Starting" << std::endl;
		return true;
	}
};

bool drawTitlescreen(SDL_Screen &Scene);
bool drawGame(SDL_Screen &Scene, SDL_Audio &Audio);

class titleScreenThing : public base {
public:
	bool draw(SDL_Screen &Scene, SDL_Audio &Audio) override {
		return drawTitlescreen(Scene);
	}
};

class gameScreen : public base {
public:
	bool draw(SDL_Screen &Scene, SDL_Audio &Audio) override {
		return drawGame(Scene, Audio);
	}
};

class mapScreen : public base {
public:
	bool draw(SDL_Screen &Scene, SDL_Audio &Audio) override {
		return runMapScreen(Scene, Audio);
	}
};