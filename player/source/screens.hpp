#pragma once
#include "draw.hpp"
#include "room.hpp"
#include "audio.hpp"
#include "map.hpp"
#include "pause.hpp"
#include "game.hpp"
#include "titlescreen.hpp"

class base {
public:
	virtual bool draw(SDL_Screen &Scene, SDL_Audio &Audio) {
		std::cout << "Starting" << std::endl;
		return true;
	}
};

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

class pauseScreen : public base {
public:
	bool draw(SDL_Screen &Scene, SDL_Audio &Audio) override {
		return runPauseScreen(Scene, Audio);
	}
};

void changeScreen(int newScreen);

bool runScreen(SDL_Screen &Scene, SDL_Audio &Audio);

enum screens {
	SCR_TITLE = 0,
	SCR_GAME,
	SCR_PAUSE,
	SCR_MAP,
};