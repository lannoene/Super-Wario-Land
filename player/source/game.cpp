#include <fstream>

#include "tiles.hpp"
#include "titlescreen.hpp"
#include "player.hpp"
#include "squishy_array.hpp"
#include "tools.hpp"
#include "pause.hpp"
#define TICKS_FOR_NEXT_FRAME (1000 / 1000)

Player wario;

int lastTime = 0;
float cameraHorizOffsetPx = 0;
int cameraVertOffsetPx = 0;

extern base* bptr;
extern gameScreen game;
extern bool showDebugInfo;

SquishyArray <Tile*>Tile_array(0);

int gameFrame = 0;

bool drawGame(SDL_Screen &Scene, SDL_Audio &Audio) {
	if (gameFrame == 0) {
		tools::decodeLevelFileIntoMemory("level.lvl");
		Audio.playMusic(MUSIC_GRASS, -1);
	}
	
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
					case SDL_SCANCODE_ESCAPE:
						wario.stopMoving(LEFT);
						wario.stopMoving(RIGHT);
						pauseScreen(Scene, Audio);
					break;
					case SDL_SCANCODE_X:
						wario.jump(Audio);
					break;
					case SDL_SCANCODE_RIGHT:
						wario.moveHoriz(RIGHT, Audio);
					break;
					case SDL_SCANCODE_LEFT:
						wario.moveHoriz(LEFT, Audio);
					break;
					case SDL_SCANCODE_DOWN:
						wario.pressDown(Audio, gameFrame);
					break;
					case SDL_SCANCODE_D:
						showDebugInfo = !showDebugInfo;
					break;
					case SDL_SCANCODE_UP:
						wario.pressUp(Audio, gameFrame);
					break;
					case SDL_SCANCODE_Z:
						wario.shoulderBash(Audio, gameFrame);
					break;
					default:
					break;
				}
			break;
			case SDL_KEYUP://add if there is a key_up while jumping, the player's velocity gets set to 0
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_RIGHT:
						wario.stopMoving(RIGHT);
					break;
					case SDL_SCANCODE_LEFT:
						wario.stopMoving(LEFT);
					break;
					case SDL_SCANCODE_DOWN:
						wario.releaseDown();
					break;
					case SDL_SCANCODE_UP:
						wario.releaseUp();
					break;
					default:
					break;
				}
			break;
			case SDL_MOUSEBUTTONDOWN:{//handle mouse input
			}
			break;
			default:
			break;
		}
	}
	
	Scene.clearScreen();
	Scene.drawImage(IMAGE_BG_LEVEL, (-floor(cameraHorizOffsetPx/10/1920 + 1)*1920*10 + cameraHorizOffsetPx/10), 0, 1920, 600);
	Scene.drawImage(IMAGE_BG_LEVEL, (-floor(cameraHorizOffsetPx/10/1920 + 1)*1920*10 + cameraHorizOffsetPx/10) + 1920*10, 0, 1920, 600);
	
	for (size_t i = 0; i < Tile_array.length(); i++) {
		Tile_array.data()[i]->update((size_t)gameFrame, wario);
		Tile_array.data()[i]->draw(Scene);
	}
	wario.update(Audio, gameFrame);
	wario.draw(Scene, gameFrame);
	
	Scene.drawRectangle(0, 0, 150, 30, CLR_WHT);
	
	char buffer[50];
	snprintf(buffer, 50, "Coins: %d", wario.getCoinCount());
	Scene.drawText(buffer, 0, 0, 30);
	
	Scene.finishDrawing();
	
	gameFrame++;
	
	return true;
}