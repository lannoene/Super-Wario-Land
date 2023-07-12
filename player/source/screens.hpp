#pragma once
#include "draw.hpp"

class base {
public:
	virtual bool draw(SDL_Screen &Scene) {
		std::cout << "Starting" << std::endl;
		return true;
	}
};

bool drawEditor(SDL_Screen &Scene);

class editScreen : public base {
public:
	bool draw(SDL_Screen &Scene) override {
		return drawEditor(Scene);
	}
};