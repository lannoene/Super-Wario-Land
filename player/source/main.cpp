#include <iostream>

#include "titlescreen.hpp"

gameScreen game;
base* bptr;

int main(int argc, char* argv[]) {
	SDL_Screen scene;
	SDL_Audio Audio;
	titleScreenThing tlScrn;
	bptr = &tlScrn;
	
	while (true) {
		if (bptr->draw(scene, Audio) == false)
			break;
	}
	return 0;
}