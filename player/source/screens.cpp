#include "screens.hpp"

base* bptr;
mapScreen map;
gameScreen game; // just for testing before the map is finished
titleScreenThing tlScrn;
pauseScreen pause;

void changeScreen(int newScreen) {
	switch (newScreen) {
		case SCR_TITLE:
			bptr = &tlScrn;
		break;
		case SCR_MAP:
			bptr = &map;
		break;
		case SCR_GAME:
			bptr = &game;
		break;
		case SCR_PAUSE:
			bptr = &pause;
		break;
	}
}

bool runScreen(SDL_Screen &Scene, SDL_Audio &Audio) {
	if (!bptr->draw(Scene, Audio)) {
		return false;
	}
	return true;
}