#include <iostream>

#include "titlescreen.hpp"

base* bptr;

int main(int argc, char* argv[]) {
	SDL_Screen scene;
	SDL_Audio Audio;
	titleScreenThing tlScrn;
	bptr = &tlScrn;
	srand((unsigned)time(0));
	
	float base = 50;
	float height = 20;
	float yourBasePos = 20;
	float elevationInRad = std::atan(height/base);
	
	float yourYcoord = yourBasePos * std::tan(elevationInRad);
	std::cout << yourYcoord << std::endl;
	
	while (true) {
		if (bptr->draw(scene, Audio) == false)
			break;
		
		
	}
	return 0;
}
