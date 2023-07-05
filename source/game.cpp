#include "game.hpp"
#include "titlescreen.hpp"
#include "player.hpp"
#include "squishy_array.hpp"
#define TICKS_FOR_NEXT_FRAME (1000 / 30)

Player wario;

int lastTime = 0;
float cameraHorizOffsetPx = 0;

extern base* bptr;
extern gameScreen game;
extern bool showDebugInfo;

SquishyArray <Tile*>Tile_array(0);

int gameFrame = 0;

bool drawGame(SDL_Screen &Scene) {
	while (lastTime - SDL_GetTicks() < TICKS_FOR_NEXT_FRAME) {
        SDL_Delay(1);
    }
	
	if (gameFrame == 2) {
		for (int i = 0; i < 100; i++) 
			Tile_array.array_push(new Tile(i*50, 550, 50, 50));//these will cause a memory leak if not freed
	}
	
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				return false;
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_ESCAPE:
						//return false;
					break;
					case SDL_SCANCODE_X:
						wario.jump();
					break;
					case SDL_SCANCODE_RIGHT:
						wario.moveHoriz(RIGHT);
					break;
					case SDL_SCANCODE_LEFT:
						wario.moveHoriz(LEFT);
					break;
					case SDL_SCANCODE_D:
						showDebugInfo = !showDebugInfo;
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
					default:
					break;
				}
			break;
			case SDL_MOUSEBUTTONDOWN://handle mouse input
				if (SDL_BUTTON_LEFT == event.button.button) {
					int coord_click_x = event.button.x;
					int coord_click_y = event.button.y;
					
					Tile_array.array_push(new Tile((floor((coord_click_x - cameraHorizOffsetPx)/50)*50), (coord_click_y/50)*50, 50, 50));
				}
			break;
			default:
			break;
		}
	}
	
	Scene.clearScreen();
	Scene.drawImage(IMAGE_BG_LEVEL, (-floor(cameraHorizOffsetPx/1920 + 1)*1920 + cameraHorizOffsetPx), 0, 1920, 600);
	Scene.drawImage(IMAGE_BG_LEVEL, (-floor(cameraHorizOffsetPx/1920 + 1)*1920 + cameraHorizOffsetPx) + 1920, 0, 1920, 600);
	
	for (int i = 0; i < Tile_array.length(); i++) {
		Tile_array.data()[i]->draw(Scene);
	}
	
	wario.update();
	wario.draw(Scene);
	
	Scene.finishDrawing();
	
    lastTime = SDL_GetTicks();
	
	gameFrame++;
	
	return true;
}