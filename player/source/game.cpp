#include <fstream>

#include "tiles.hpp"
#include "screens.hpp"
#include "player.hpp"
#include "squishy_array.hpp"
#include "tools.hpp"
#include "pause.hpp"
#include "room.hpp"

Player wario;

SquishyArray <Room*>Room_array(0);

int lastTime = 0;
float cameraHorizOffsetPx = 0;
int cameraVertOffsetPx = 0;

extern bool showDebugInfo;

int gameFrame = 0;

bool drawGame(SDL_Screen &Scene, SDL_Audio &Audio) {
	
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				tools::resetLevel(Room_array);
				return false;
			break;
			case SDL_KEYDOWN:
				if (event.key.repeat)
					break;
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_ESCAPE:
						wario.stopMoving(LEFT);
						wario.stopMoving(RIGHT);
						if (Scene.isFullscreen())
							Scene.showCursor(true);
						changeScreen(SCR_PAUSE);
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
					case SDL_SCANCODE_J:
						if (wario.getCurrentRoomId() > 0)
							wario.setCurrentRoomId(wario.getCurrentRoomId() - 1);
					break;
					case SDL_SCANCODE_K:
						if (wario.getCurrentRoomId() < Room_array.shortLen())
							wario.setCurrentRoomId(wario.getCurrentRoomId() + 1);
					break;
					case SDL_SCANCODE_F11:
						Scene.toggleWindowFullscreen();
					break;
					case SDL_SCANCODE_R:
						/*tools::resetLevel(Room_array);
						bptr = &mapScr;
						gameFrame = 0;
						return true;*/
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
	
	Room_array.data()[wario.getCurrentRoomId()]->drawBackground(Scene, cameraHorizOffsetPx, cameraVertOffsetPx);
	
	for (size_t i = 0; i < Room_array.data()[wario.getCurrentRoomId()]->Room_tiles.length(); i++) { // update & draw current room's tiles
		if (i < Room_array.data()[wario.getCurrentRoomId()]->Room_tiles.length())
			Room_array.data()[wario.getCurrentRoomId()]->Room_tiles.data()[i]->update((size_t)gameFrame, wario);
		if (i < Room_array.data()[wario.getCurrentRoomId()]->Room_tiles.length())
			Room_array.data()[wario.getCurrentRoomId()]->Room_tiles.data()[i]->draw(Scene, (size_t)gameFrame);
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

void loadLevel(std::string levelPath, SDL_Audio &Audio) {
	tools::decodeLevelFileIntoMemory(levelPath, Room_array);
	
	int spawnPointTileId = 0;
	int spawnPointTileRoom = 0;
	
	for (size_t j = 0; j < Room_array.length(); j++) {
		for (size_t i = 0; i < Room_array.data()[j]->Room_tiles.length(); i++) {
			if (Room_array.data()[j]->Room_tiles.data()[i]->getType() == TILE_SPAWN_POINT) {
				spawnPointTileId = i;
				spawnPointTileRoom = j;
			}
		}
	}
	
	for (size_t i = 0; i < Room_array.data()[Room_array.data()[spawnPointTileRoom]->Room_tiles.data()[spawnPointTileId]->param3.destinationRoomId]->Room_tiles.length(); i++) {
		if (Room_array.data()[Room_array.data()[spawnPointTileRoom]->Room_tiles.data()[spawnPointTileId]->param3.destinationRoomId]->Room_tiles.data()[i]->param1.warpId == Room_array.data()[spawnPointTileRoom]->Room_tiles.data()[spawnPointTileId]->param2.destinationWarpId && Room_array.data()[Room_array.data()[spawnPointTileRoom]->Room_tiles.data()[spawnPointTileId]->param3.destinationRoomId]->Room_tiles.data()[i]->flags.warp) {
			wario.setCurrentRoomId(Room_array.data()[spawnPointTileRoom]->Room_tiles.data()[spawnPointTileId]->param3.destinationRoomId);
			wario.setXYPos(Room_array.data()[Room_array.data()[spawnPointTileRoom]->Room_tiles.data()[spawnPointTileId]->param3.destinationRoomId]->Room_tiles.data()[i]->x, Room_array.data()[Room_array.data()[spawnPointTileRoom]->Room_tiles.data()[spawnPointTileId]->param3.destinationRoomId]->Room_tiles.data()[i]->y);
			break;
		}
	}
	Room_array.data()[wario.getCurrentRoomId()]->roomPlayBackgroundMusic(Audio);
}

void unloadCurrentLevel(SDL_Audio &Audio) {
	tools::resetLevel(Room_array);
	Audio.stopMusic();
}