#include "map.hpp"
#include "titlescreen.hpp"
#include "tools.hpp"

extern base* bptr;
gameScreen game;
int level;

bool runMapScreen(SDL_Screen &Scene, SDL_Audio &Audio) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				return false;
			break;
			case SDL_KEYDOWN:
				if (event.key.repeat)
					break;
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_RIGHT:
						++level;
					break;
					case SDL_SCANCODE_LEFT:
						--level;
					break;
					case SDL_SCANCODE_RETURN:
						switch (level) {
							default:
							break;
							case 0: // baking XDDD
								tools::decodeLevelFileIntoMemory("level.lvl");
								bptr = &game;
							break;
							case 1:
								tools::decodeLevelFileIntoMemory("level1.lvl");
								bptr = &game;
							break;
							case 2:
								tools::decodeLevelFileIntoMemory("level2.lvl");
								bptr = &game;
							break;
						}
					break;
					default:
					break;
				}
			break;
		}
	}
	
	Scene.clearScreen();
	Scene.drawRectangle(0, 0, 100, 100, CLR_YLW);
	char buffer[50];
	snprintf(buffer, 50, "Level: %d", level);
	Scene.drawText(buffer, 0, 0, 30);
	
	Scene.finishDrawing();
	
	return true;
}