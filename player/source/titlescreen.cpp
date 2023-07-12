#include "titlescreen.hpp"
#include "draw.hpp"
#include "squishy_array.hpp"

extern base* bptr;//really messy but i couldnt figure out how to not extern it & stuff
extern gameScreen game;

bool drawTitlescreen(SDL_Screen &Scene) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				return false;
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
					default:
					break;
					case SDL_SCANCODE_ESCAPE:
						return false;
					break;
					case SDL_SCANCODE_SPACE:
						bptr = &game;
					break;
				}
			break;
			case SDL_MOUSEBUTTONDOWN://handle mouse input
			break;
			default:
			break;
		}
	}
	
	Scene.clearScreen();
	
	Scene.drawImage(IMAGE_BG_TITLESCREEN, -100, 25, 960, 540);
	Scene.drawText((char*)"Super Wario Land", 160, 220, 70);
	Scene.drawText((char*)"Press space to continue", 140, 300, 60);

	Scene.finishDrawing();
	return true;
}