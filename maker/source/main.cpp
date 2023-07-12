#include <iostream>
#include "screens.hpp"

base* bptr;

int main(int argc, char* argv[]) {
	SDL_Screen scene;
	editScreen game;
	bptr = &game;
	
	while (true) {
		if (bptr->draw(scene) == false)
			break;
	}
	
	return 0;
}