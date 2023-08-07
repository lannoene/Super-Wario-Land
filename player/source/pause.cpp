#include "pause.hpp"

bool pauseScreen(SDL_Screen &Scene, SDL_Audio &Audio) {
	bool mousePushed = false;
	int coord_click_x;
	int coord_click_y;
	int clickedButton;
	while (true) {
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
							return true;
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
						case SDL_SCANCODE_DOWN:
							Mix_Volume(-1, Mix_Volume(-1, -1) - 1);
						break;
						case SDL_SCANCODE_UP:
							Mix_Volume(-1, Mix_Volume(-1, -1) + 1);
						break;
					}
				break;
				case SDL_MOUSEBUTTONDOWN: {
					coord_click_x = event.button.x;
					coord_click_y = event.button.y;
					if (SDL_BUTTON_LEFT == event.button.button) {
						mousePushed = true;
						if (coord_click_x > 0 && coord_click_x < 0 + 512 && coord_click_y >= 30 && coord_click_y < 30 + 30) {
							Mix_Volume(-1, coord_click_x*MIX_MAX_VOLUME/512);
							clickedButton = SLIDER_SFX;
							
						} else if (coord_click_x > 0 && coord_click_x < 0 + 512 && coord_click_y >= 60 && coord_click_y < 60 + 30) {
							Mix_VolumeMusic(coord_click_x*MIX_MAX_VOLUME/512);
							clickedButton = SLIDER_MUS;
						} else {
							clickedButton = NO_BUTTON;
						}
					}
				}
				break;
				case SDL_MOUSEBUTTONUP:
					mousePushed = false;
				break;
				case SDL_MOUSEMOTION: {
					if (mousePushed) {
						coord_click_x += event.motion.xrel;
						coord_click_y += event.motion.yrel;
						if (coord_click_x > 0 && coord_click_x <= 0 + 513 && coord_click_y >= 30 && coord_click_y < 30 + 30 && clickedButton == SLIDER_SFX) {
							Mix_Volume(-1, coord_click_x*MIX_MAX_VOLUME/512);
						} else if (coord_click_x > 0 && coord_click_x < 0 + 513 && coord_click_y >= 60 && coord_click_y < 60 + 30 && clickedButton == SLIDER_MUS) {
							Mix_VolumeMusic(coord_click_x*MIX_MAX_VOLUME/512);
						}
					}
				}
				break;
			}
		}
		
		Scene.clearScreen();
	
		Scene.drawText((char*)"Settings!", 350, 0, 30);
		
		float floatMusVol = Mix_Volume(-1, -1);
		floatMusVol = floatMusVol/MIX_MAX_VOLUME*512;
		Scene.drawRectangle(0, 30, floatMusVol, 30, CLR_GRY);
		
		floatMusVol = Mix_VolumeMusic(-1);
		floatMusVol = floatMusVol/MIX_MAX_VOLUME*512;
		Scene.drawRectangle(0, 60, floatMusVol, 30, CLR_GRY);
		
		char buffer[50];
		snprintf(buffer, 50, "SFX: %d", (int)(((float)Mix_Volume(-1, -1)/128)*100));
		Scene.drawText(buffer, 0, 30, 30);
		
		snprintf(buffer, 50, "Music: %d", (int)(((float)Mix_VolumeMusic(-1)/128)*100));
		Scene.drawText(buffer, 0, 60, 30);
		Scene.finishDrawing();
	}
	return true;
}