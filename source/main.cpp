#include <iostream>

#include "titlescreen.hpp"

gameScreen game;
base* bptr;

int main(int argc, char* argv[]) {
	SDL_Screen scene;
	titleScreenThing tlScrn;
	bptr = &tlScrn;
	
	while (true) {
		if (bptr->draw(scene) == false)
			break;
	}
	return 0;
}