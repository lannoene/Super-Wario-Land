#include <iostream>

#include "screens.hpp"

int main(int argc, char* argv[]) {
	SDL_Screen Scene;
	SDL_Audio Audio;
	changeScreen(SCR_TITLE);
	srand((unsigned)time(0));
	
	float base = 50;
	float height = 20;
	float yourBasePos = 20;
	float elevationInRad = std::atan(height/base);
	
	float yourYcoord = yourBasePos * std::tan(elevationInRad);
	std::cout << yourYcoord << std::endl;
	
	while (true) {
		if (runScreen(Scene, Audio) == false)
			break;
	}
	return 0;
}
