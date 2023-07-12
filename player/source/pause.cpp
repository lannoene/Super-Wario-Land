#include "pause.hpp"

void pauseScreen(SDL_Screen &Scene, SDL_Audio &Audio) {
	
	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					//return "";
				break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode) {
						default:
						break;
						case SDL_SCANCODE_ESCAPE:
							return;
						break;
						case SDL_SCANCODE_G:
							if (Mix_Volume(-1, -1) == MIX_MAX_VOLUME) {
								Mix_Volume(-1, 0);
							} else {
								Mix_Volume(-1, MIX_MAX_VOLUME);
							}
						break;
						case SDL_SCANCODE_H:
							if (Mix_VolumeMusic(-1) == MIX_MAX_VOLUME) {
								Mix_VolumeMusic(0);
							} else {
								Mix_VolumeMusic(MIX_MAX_VOLUME);
							}
						break;
					}
				break;
				case SDL_MOUSEBUTTONDOWN: {
					int coord_click_x = event.button.x;
					int coord_click_y = event.button.y;
					if (SDL_BUTTON_LEFT == event.button.button) {
						//add buttons lol
					}
				}
			}
		}
		
		Scene.clearScreen();
	
		Scene.drawText((char*)"Settings!", 350, 0, 30);
		char buffer[50];
		snprintf(buffer, 50, "SFX: %d", Mix_Volume(-1, -1));
		Scene.drawText(buffer, 0, 30, 30);
		snprintf(buffer, 50, "Music: %d", Mix_VolumeMusic(-1));
		Scene.drawText(buffer, 0, 60, 30);
		Scene.finishDrawing();
	}
}