#pragma once
#include <SDL2/SDL.h>

#include "draw.hpp"

class base {
public:
	virtual bool draw(SDL_Screen &Scene) {
		std::cout << "Starting" << std::endl;
		return true;
	}
};

bool drawTitlescreen(SDL_Screen &Scene);
bool drawGame(SDL_Screen &Scene);

class titleScreenThing : public base {
public:
	bool draw(SDL_Screen &Scene) override {
		return drawTitlescreen(Scene);
	}
};

class gameScreen : public base {
public:
	bool draw(SDL_Screen &Scene) override {
		return drawGame(Scene);
	}
};