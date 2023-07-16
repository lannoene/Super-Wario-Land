#include <iostream>

#include "titlescreen.hpp"

gameScreen game;
base* bptr;

int main(int argc, char* argv[]) {
	SDL_Screen scene;
	SDL_Audio Audio;
	titleScreenThing tlScrn;
	bptr = &tlScrn;
	srand((unsigned)time(0));
	
	while (true) {
		if (bptr->draw(scene, Audio) == false)
			break;
	}
	return 0;
}