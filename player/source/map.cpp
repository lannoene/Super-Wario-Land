#include "map.hpp"
#include "screens.hpp"
#include "game.hpp"

int level;
extern int gameFrame;

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
						gameFrame = 0;
						switch (level) {
							default:
							break;
							case 0: // funny loading
								loadLevel("level1.lvl", Audio);
								changeScreen(SCR_GAME);
							break;
							case 1:
								loadLevel("level2.lvl", Audio);
								changeScreen(SCR_GAME);
							break;
							case 2:
								loadLevel("level3.lvl", Audio);
								changeScreen(SCR_GAME);
							break;
						}
					break;
					case SDL_SCANCODE_F11:
						Scene.toggleWindowFullscreen();
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