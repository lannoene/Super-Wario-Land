#include <fstream>

#include "squishy_array.hpp"
#include "screens.hpp"
#include "editor.hpp"
#include "tiles.hpp"
#include "tools.hpp"
#include "popups.hpp"

float cameraHorizOffsetPx = 0;
float cameraVertOffsetPx = 0;
SquishyArray <Tile*>Tile_array(0);
int currentTile = 0;

bool drawEditor(SDL_Screen &Scene) {
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
					case SDL_SCANCODE_RIGHT:
						cameraHorizOffsetPx -= 50;
					break;
					case SDL_SCANCODE_LEFT:
						cameraHorizOffsetPx += 50;
					break;
					case SDL_SCANCODE_UP:
						cameraVertOffsetPx += 50;
					break;
					case SDL_SCANCODE_DOWN:
						cameraVertOffsetPx -= 50;
					break;
					case SDL_SCANCODE_S: {
						if((event.key.keysym.mod & SDLK_LCTRL)) {
							std::ofstream MyFile("level.lvl");
							std::string fileCont;
							
							for (int i = 0; i < Tile_array.length(); i++) {
								fileCont.append(Tile_array.data()[i]->appendOutputToString());
							}
							MyFile << fileCont;
							MyFile.close();
							puts("saved");
							Scene.setTitle((char*)"Super Wario Land - Editor");
						}
					}
					break;
					case SDL_SCANCODE_G://i could read lines and decode them seperately for different level types like (ground tile/enimies) maybe
						tools::resetLevel();
						tools::decodeLevelFileIntoMemory("level.lvl");
						Scene.setTitle((char*)"Super Wario Land - Editor");
					break;
					case SDL_SCANCODE_R:
						tools::resetLevel();
						Scene.setTitle((char*)"Super Wario Land - Editor");
					break;
					default:
					break;
				}
			break;
			case SDL_MOUSEBUTTONDOWN: {
				int coord_click_x = event.button.x;
				int coord_click_y = event.button.y;
				if (SDL_BUTTON_LEFT == event.button.button) {
					Tile_array.array_push(new Tile((floor((coord_click_x - cameraHorizOffsetPx)/50)*50), floor((coord_click_y - cameraVertOffsetPx)/50)*50, currentTile));
					Tile_array.data()[Tile_array.length() - 1]->param1.dummyVar = -9999;
					Tile_array.data()[Tile_array.length() - 1]->param2.dummyVar = -9999;
					Tile_array.data()[Tile_array.length() - 1]->param3.dummyVar = -9999;
					
					switch (currentTile) {
						case TILE_DOOR: {
							std::string tempDoorIdText = createPopup(Scene, (char*)"Enter door id");
							std::string tempDestDoorIdText = createPopup(Scene, (char*)"Enter destination door id");
							
							int stoiDoorId = 0;
							int stoiDestDoorId = 0;
							try {
								stoiDoorId = std::stoi(tempDoorIdText);
							} catch (std::invalid_argument const&) {
								std::cout << "Invalid number" << std::endl;
								delete Tile_array.data()[Tile_array.length() - 1];
								Tile_array.array_splice(Tile_array.length() - 1, 1);
								break;
							}
							
							try {
								stoiDestDoorId = std::stoi(tempDestDoorIdText);
							} catch (std::invalid_argument const&) {
								std::cout << "Invalid number" << std::endl;
								delete Tile_array.data()[Tile_array.length() - 1];
								Tile_array.array_splice(Tile_array.length() - 1, 1);
								break;
							}
							Tile_array.data()[Tile_array.length() - 1]->param1.doorId = stoiDoorId;
							Tile_array.data()[Tile_array.length() - 1]->param2.destinationDoorId = stoiDestDoorId;
						}
						break;
					}
					
				}
				if (SDL_BUTTON_RIGHT == event.button.button) {
					for (int i = Tile_array.length() - 1; i >= 0; i--) {
						if (Tile_array.data()[i]->x == floor(-cameraHorizOffsetPx + (coord_click_x/50)*50) && Tile_array.data()[i]->y == floor((coord_click_y - cameraVertOffsetPx)/50)*50) {
							delete Tile_array.data()[i];
							Tile_array.array_splice(i, 1);
							break;
						}
					}
				}
				Scene.setTitle((char*)"*Super Wario Land - Editor");
			}
			break;
			case SDL_MOUSEWHEEL: {
				if(event.wheel.y > 0) {
					++currentTile;
				} else if (event.wheel.y < 0) {
					--currentTile;
				}
			}
			break;
		}
	}
	
	Scene.clearScreen();
	
	Scene.drawImage(IMAGE_BG_LEVEL, (-floor(cameraHorizOffsetPx/1920 + 1)*1920 + cameraHorizOffsetPx), 0, 1920, 600);
	Scene.drawImage(IMAGE_BG_LEVEL, (-floor(cameraHorizOffsetPx/1920 + 1)*1920 + cameraHorizOffsetPx) + 1920, 0, 1920, 600);
	
	for (int i = 0; i < Tile_array.length(); i++) {
		Tile_array.data()[i]->draw(Scene);
	}
	
	char buffer[50];
	snprintf(buffer, 50, "X: %f", -cameraHorizOffsetPx);
	Scene.drawText(buffer, 0, 0, 30);
	snprintf(buffer, 50, "Y: %f", -cameraVertOffsetPx);
	Scene.drawText(buffer, 0, 30, 30);
	switch (currentTile) {
		default:
			snprintf(buffer, 50, "Current tile: %d (unknown)", currentTile);
		break;
		case TILE_GRASS_TOP:
			snprintf(buffer, 50, "Current tile: Grass_Top");
		break;
		case TILE_GRASS_TOP_RIGHT:
			snprintf(buffer, 50, "Current tile: Grass_Top_Right");
		break;
		case TILE_GRASS_TOP_LEFT:
			snprintf(buffer, 50, "Current tile: Grass_Top_Left");
		break;
		case TILE_GRASS_MIDDLE_MIDDLE:
			snprintf(buffer, 50, "Current tile: Grass_Middle");
		break;
		case TILE_GRASS_MIDDLE_RIGHT:
			snprintf(buffer, 50, "Current tile: Grass_Middle_Right");
		break;
		case TILE_GRASS_MIDDLE_LEFT:
			snprintf(buffer, 50, "Current tile: Grass_Middle_Left");
		break;
		case TILE_GRASS_BOTTOM_MIDDLE:
			snprintf(buffer, 50, "Current tile: Grass_Bottom");
		break;
		case TILE_GRASS_BOTTOM_RIGHT:
			snprintf(buffer, 50, "Current tile: Grass_Bottom_Right");
		break;
		case TILE_GRASS_BOTTOM_LEFT:
			snprintf(buffer, 50, "Current tile: Grass_Bottom_Left");
		break;
		case TILE_GRASS_CONNECTOR_TOP_TO_RIGHT:
			snprintf(buffer, 50, "Current tile: Grass_Connector_Top_To_Right");
		break;
		case TILE_GRASS_CONNECTOR_TOP_TO_LEFT:
			snprintf(buffer, 50, "Current tile: Grass_Connector_Top_To_Left");
		break;
		case TILE_GRASS_CONNECTOR_BOTTOM_TO_RIGHT:
			snprintf(buffer, 50, "Current tile: Grass_Connector_Bottom_To_Right");
		break;
		case TILE_GRASS_CONNECTOR_BOTTOM_TO_LEFT:
			snprintf(buffer, 50, "Current tile: Grass_Connector_Bottom_To_Left");
		break;
		case TILE_SPAWN_POINT:
			snprintf(buffer, 50, "Current tile: Spawn_Point");
		break;
		case GOLD_COIN:
			snprintf(buffer, 50, "Current tile: Gold_Coin");
		break;
		case TILE_DOOR:
			snprintf(buffer, 50, "Current tile: Door_Top");
		break;
		case TILE_DOOR_BOTTOM:
			snprintf(buffer, 50, "Current tile: Door_Bottom");
		break;
		case TILE_DIRT_BG:
			snprintf(buffer, 50, "Current tile: Dirt_Bg_Tile");
		break;
		case TILE_DIRT_BG_SHADOW:
			snprintf(buffer, 50, "Current tile: Dirt_Bg_Tile_Shadow");
		break;
		case TILE_DIRT_SEMISOLID:
			snprintf(buffer, 50, "Current tile: Dirt_Tile_Semi_Solid");
		break;
		case TILE_LOG_MIDDLE_FAR_LEFT:
			snprintf(buffer, 50, "Current tile: Middle_Log_Tile_Far_Left");
		break;
		case TILE_LOG_MIDDLE_INNER_LEFT:
			snprintf(buffer, 50, "Current tile: Middle_Log_Tile_Inner_Left");
		break;
		case TILE_LOG_MIDDLE_INNER_RIGHT:
			snprintf(buffer, 50, "Current tile: Middle_Log_Tile_Inner_Right");
		break;
		case TILE_LOG_MIDDLE_FAR_RIGHT:
			snprintf(buffer, 50, "Current tile: Middle_Log_Tile_Far_Right");
		break;
		case TILE_LOG_BOTTOM_FAR_LEFT:
			snprintf(buffer, 50, "Current tile: Bottom_Log_Tile_Far_Left");
		break;
		case TILE_LOG_BOTTOM_INNER_LEFT:
			snprintf(buffer, 50, "Current tile: Bottom_Log_Tile_Inner_Left");
		break;
		case TILE_LOG_BOTTOM_INNER_RIGHT:
			snprintf(buffer, 50, "Current tile: Bottom_Log_Tile_Inner_Right");
		break;
		case TILE_LOG_BOTTOM_FAR_RIGHT:
			snprintf(buffer, 50, "Current tile: Bottom_Log_Tile_Far_Right");
		break;
	}
	Scene.drawText(buffer, 0, 60, 30);
	
	
	Scene.finishDrawing();
	return true;
}