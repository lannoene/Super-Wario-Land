#include <fstream>
#include <cmath>

#include "squishy_array.hpp"
#include "screens.hpp"
#include "editor.hpp"
#include "tiles.hpp"
#include "tools.hpp"
#include "popups.hpp"
#include "room.hpp"

#define LEFT	false
#define RIGHT	true

float cameraHorizOffsetPx = 0;
float cameraVertOffsetPx = 0;
SquishyArray <SquishyArray<Tile*>*>Room_array_temp(0);
SquishyArray <Room*>Room_array(0);
int currentRoom = 0;
int maxRooms = 0;
int currentTile = 0;
bool mousePushed = false;
bool mouseButton;
int coord_click_x;
int coord_click_y;
bool hasChangedWithoutSaving = false;
int gameFrame = 0;

bool drawEditor(SDL_Screen &Scene) {
	if (gameFrame == 0) {
		Room_array_temp.array_push(new SquishyArray <Tile*>(0));
		Room_array.array_push(new Room);
		
	}
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				if (hasChangedWithoutSaving && yesNoPopup(Scene, (char*)"Your level has unsaved changes. Save changes before exiting?"))
					tools::saveLevel(Scene, Room_array);
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
							tools::saveLevel(Scene, Room_array);
							Scene.setTitle((char*)("Super Wario Land - Editor"));
							hasChangedWithoutSaving = false;
						}
					}
					break;
					case SDL_SCANCODE_G:
						tools::resetLevel(Room_array);
						tools::decodeLevelFileIntoMemory(textPopup(Scene, (char*)"Level file name").c_str(), Room_array);
						Scene.setTitle((char*)"Super Wario Land - Editor");
						hasChangedWithoutSaving = false;
					break;
					case SDL_SCANCODE_R:
						tools::resetLevel(Room_array);
						Room_array.array_push(new Room);
						Scene.setTitle((char*)"Super Wario Land - Editor");
						hasChangedWithoutSaving = false;
					break;
					case SDL_SCANCODE_N:
						Room_array.array_push(new Room);
						puts("new area added");
					break;
					case SDL_SCANCODE_J:
						if (currentRoom > 0)
							--currentRoom;
					break;
					case SDL_SCANCODE_K:
						if (currentRoom < Room_array.shortLen())
							++currentRoom;
					break;
					case SDL_SCANCODE_E: {
						std::string tempWidth = textPopup(Scene, (char*)"Enter room width");
						std::string tempHeight = textPopup(Scene, (char*)"Enter room height");
						
						int stoiWidth;
						int stoiHeight;
						
						try {
							stoiWidth = std::stoi(tempWidth);
							stoiHeight = std::stoi(tempHeight);
						} catch (std::invalid_argument const&) {
							std::cout << "Invalid number" << std::endl;
							break;
						}
						Room_array.data()[currentRoom]->setRoomWidth(stoiWidth);
						Room_array.data()[currentRoom]->setRoomHeight(stoiHeight);
					}
					break;
					case SDL_SCANCODE_B: {
						std::string tempBgId = textPopup(Scene, (char*)"Enter background id");
						std::string tempBgMusId = textPopup(Scene, (char*)"Enter background music id");
						
						int stoiBgId;
						int stoiBgmId;
						
						try {
							stoiBgId = std::stoi(tempBgId);
							stoiBgmId = std::stoi(tempBgMusId);
						} catch (std::invalid_argument const&) {
							std::cout << "Invalid number" << std::endl;
							break;
						}
						
						Room_array.data()[currentRoom]->roomSetBackground(stoiBgId);
						Room_array.data()[currentRoom]->roomSetMusic(stoiBgmId);
					}
					break;
					case SDL_SCANCODE_F11:
						Scene.toggleWindowFullscreen();
					break;
					case SDL_SCANCODE_L:
						cameraHorizOffsetPx = 0;
						cameraVertOffsetPx = 0;
					break;
					default:
					break;
				}
			break;
			case SDL_MOUSEBUTTONDOWN: {
				coord_click_x = event.button.x;
				coord_click_y = event.button.y;
				if (SDL_BUTTON_LEFT == event.button.button) {
					hasChangedWithoutSaving = true;
					mousePushed = true;
					mouseButton = LEFT;
					Room_array.data()[currentRoom]->Room_tiles.array_push(new Tile((floor((coord_click_x - cameraHorizOffsetPx)/50)*50), floor((coord_click_y - cameraVertOffsetPx)/50)*50, currentTile));
					Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()]->param1.dummyVar = -9999;
					Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()]->param2.dummyVar = -9999;
					Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()]->param3.dummyVar = -9999;
					
					switch (currentTile) {
						case TILE_DOOR: {
							mousePushed = false;
							std::string tempDoorIdText = textPopup(Scene, (char*)"Enter door's warp id");
							std::string tempDestDoorIdText = textPopup(Scene, (char*)"Enter destination warp id");
							std::string tempDestRoomIdText = textPopup(Scene, (char*)"Enter destination room id");
							
							int stoiDoorId = 0;
							int stoiDestDoorId = 0;
							int stoiDestRoomId = 0;
							try {
								stoiDoorId = std::stoi(tempDoorIdText);
								stoiDestDoorId = std::stoi(tempDestDoorIdText);
								stoiDestRoomId = std::stoi(tempDestRoomIdText);
							} catch (std::invalid_argument const&) {
								std::cout << "Invalid number" << std::endl;
								delete Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()];
								Room_array.data()[currentRoom]->Room_tiles.array_splice(Room_array.data()[currentRoom]->Room_tiles.shortLen(), 1);
								break;
							}
							
							if (stoiDoorId < 100 && stoiDestDoorId < 100) {
								Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()]->param1.warpId = stoiDoorId;
								Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()]->param2.destinationWarpId = stoiDestDoorId;
								Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()]->param3.destinationRoomId = stoiDestRoomId;
							} else {
								delete Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()];
								Room_array.data()[currentRoom]->Room_tiles.array_splice(Room_array.data()[currentRoom]->Room_tiles.shortLen(), 1);
								std::cout << "Warp ids must be numbered below 100" << std::endl;
							}
						}
						break;
						case TILE_WARP_BLOCK: {
							mousePushed = false;
							
							int stoiWarpId = 0;
							try {
								stoiWarpId = std::stoi(textPopup(Scene, (char*)"Enter warp id"));
							} catch (std::invalid_argument const&) {
								std::cout << "Invalid number" << std::endl;
								delete Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()];
								Room_array.data()[currentRoom]->Room_tiles.array_splice(Room_array.data()[currentRoom]->Room_tiles.shortLen(), 1);
								break;
							}
							if (stoiWarpId < 100)
								Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()]->param1.warpId = stoiWarpId;
							else {
								delete Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()];
								Room_array.data()[currentRoom]->Room_tiles.array_splice(Room_array.data()[currentRoom]->Room_tiles.shortLen(), 1);
								std::cout << "Warp ids must be numbered below 100" << std::endl;
							}
						}
						break;
						case TILE_SPAWN_POINT: {
							mousePushed = false;
							
							int stoiDestWarpId = 0;
							int stoiDestRoomId = 0;
							
							std::string tempDestWarpIdText = textPopup(Scene, (char*)"Enter destination warp id");
							std::string tempDestRoomIdText = textPopup(Scene, (char*)"Enter destination room id");
							
							try {
								stoiDestWarpId = std::stoi(tempDestWarpIdText);
								stoiDestRoomId = std::stoi(tempDestRoomIdText);
							} catch (std::invalid_argument const&) {
								std::cout << "Invalid number" << std::endl;
								delete Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()];
								Room_array.data()[currentRoom]->Room_tiles.array_splice(Room_array.data()[currentRoom]->Room_tiles.shortLen(), 1);
								break;
							}
							Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()]->param2.destinationWarpId = stoiDestWarpId;
							Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()]->param3.destinationRoomId = stoiDestRoomId;
						}
						break;
					}
					Scene.setTitle((char*)"*Super Wario Land - Editor");
				}
				if (SDL_BUTTON_RIGHT == event.button.button) {
					mousePushed = true;
					mouseButton = RIGHT;
					for (int i = Room_array.data()[currentRoom]->Room_tiles.shortLen(); i >= 0; i--) {
						if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->x == floor(-cameraHorizOffsetPx + (coord_click_x/50)*50) && Room_array.data()[currentRoom]->Room_tiles.data()[i]->y == floor((coord_click_y - cameraVertOffsetPx)/50)*50) {
							hasChangedWithoutSaving = true;
							delete Room_array.data()[currentRoom]->Room_tiles.data()[i];
							Room_array.data()[currentRoom]->Room_tiles.array_splice(i, 1);
							Scene.setTitle((char*)"*Super Wario Land - Editor");
							break;
						}
					}
				}
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
			case SDL_MOUSEMOTION: {
				if (mousePushed && mouseButton == LEFT) {
					coord_click_x += event.motion.xrel;
					coord_click_y += event.motion.yrel;
					
					bool tileThere = false;
				
					for (int i = Room_array.data()[currentRoom]->Room_tiles.shortLen(); i >= 0; i--) {
						if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->x == floor(-cameraHorizOffsetPx + (coord_click_x/50)*50) && Room_array.data()[currentRoom]->Room_tiles.data()[i]->y == floor((coord_click_y - cameraVertOffsetPx)/50)*50 && Room_array.data()[currentRoom]->Room_tiles.data()[i]->getType() == currentTile) {
							tileThere = true;
							break;
						}
					}
					
					if (!tileThere && coord_click_x < 800 && coord_click_y < 600 && coord_click_x >= 0 && coord_click_y >= 0) {
						Room_array.data()[currentRoom]->Room_tiles.array_push(new Tile((floor((coord_click_x - cameraHorizOffsetPx)/50)*50), floor((coord_click_y - cameraVertOffsetPx)/50)*50, currentTile));
						Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()]->param1.dummyVar = 0;
						Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()]->param2.dummyVar = 0;
						Room_array.data()[currentRoom]->Room_tiles.data()[Room_array.data()[currentRoom]->Room_tiles.shortLen()]->param3.dummyVar = 0;
						hasChangedWithoutSaving = true;
						Scene.setTitle((char*)"*Super Wario Land - Editor");
					}
				} else if (mousePushed && mouseButton == RIGHT) {
					coord_click_x += event.motion.xrel;
					coord_click_y += event.motion.yrel;
					
					for (int i = Room_array.data()[currentRoom]->Room_tiles.shortLen(); i >= 0; i--) {
						if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->x == floor(-cameraHorizOffsetPx + (coord_click_x/50)*50) && Room_array.data()[currentRoom]->Room_tiles.data()[i]->y == floor((coord_click_y - cameraVertOffsetPx)/50)*50) {
							hasChangedWithoutSaving = true;
							Scene.setTitle((char*)"*Super Wario Land - Editor");
							delete Room_array.data()[currentRoom]->Room_tiles.data()[i];
							Room_array.data()[currentRoom]->Room_tiles.array_splice(i, 1);
							break;
						}
					}
					
				} else {
					SDL_GetMouseState(&coord_click_x, &coord_click_y);
				}
			}
			break;
			case SDL_MOUSEBUTTONUP:
				mousePushed = false;
			break;
		}
	}
	
	Scene.clearScreen();
	
	Room_array.data()[currentRoom]->drawBackground(Scene, cameraHorizOffsetPx);
	
	for (int i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) {
		Room_array.data()[currentRoom]->Room_tiles.data()[i]->draw(Scene);
	}
	Scene.drawLineRectangle(0 + cameraHorizOffsetPx, 0 + cameraVertOffsetPx, Room_array.data()[currentRoom]->roomWidth(), Room_array.data()[currentRoom]->roomHeight(), CLR_RED);
	
	switch (currentTile) {
		default:
			Scene.drawImage(IMAGE_UNKNOWN, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GRASS_TOP:
			Scene.drawImage(IMAGE_TILE_TOP_GRASS, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GRASS_TOP_RIGHT:
			Scene.drawImage(IMAGE_TILE_TOP_GRASS_RIGHT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GRASS_TOP_LEFT:
			Scene.drawImage(IMAGE_TILE_TOP_GRASS_LEFT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GRASS_MIDDLE_MIDDLE:
			Scene.drawImage(IMAGE_TILE_MIDDLE_GRASS_MIDDLE, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GRASS_MIDDLE_RIGHT:
			Scene.drawImage(IMAGE_TILE_MIDDLE_GRASS_RIGHT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GRASS_MIDDLE_LEFT:
			Scene.drawImage(IMAGE_TILE_MIDDLE_GRASS_LEFT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GRASS_BOTTOM_MIDDLE:
			Scene.drawImage(IMAGE_TILE_BOTTOM_GRASS_MIDDLE, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GRASS_BOTTOM_RIGHT:
			Scene.drawImage(IMAGE_TILE_BOTTOM_GRASS_RIGHT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GRASS_BOTTOM_LEFT:
			Scene.drawImage(IMAGE_TILE_BOTTOM_GRASS_LEFT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GRASS_CONNECTOR_TOP_TO_RIGHT:
			Scene.drawImage(IMAGE_TILE_CONNECTOR_1, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GRASS_CONNECTOR_TOP_TO_LEFT:
			Scene.drawImage(IMAGE_TILE_CONNECTOR_2, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GRASS_CONNECTOR_BOTTOM_TO_RIGHT:
			Scene.drawImage(IMAGE_TILE_CONNECTOR_3, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GRASS_CONNECTOR_BOTTOM_TO_LEFT:
			Scene.drawImage(IMAGE_TILE_CONNECTOR_4, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case GOLD_COIN:
			Scene.drawImage(GOLD_COIN_ROTATION_1, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_DOOR:
			Scene.drawImage(IMAGE_DOOR_TOP, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_DOOR_BOTTOM:
			Scene.drawImage(IMAGE_DOOR_BOTTOM, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_SPAWN_POINT:
			Scene.drawImage(IMAGE_SPAWN_POINT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_DIRT_BG:
			Scene.drawImage(IMAGE_TILE_GRASS_DIRT_BG, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_DIRT_BG_SHADOW:
			Scene.drawImage(IMAGE_TILE_GRASS_DIRT_BG_SHADOW, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_DIRT_SEMISOLID:
			Scene.drawImage(IMAGE_TILE_GRASS_SEM_PLT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_LOG_MIDDLE_FAR_LEFT:
			Scene.drawImage(IMAGE_TILE_MIDDLE_LOG_FAR_LEFT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_LOG_MIDDLE_INNER_LEFT:
			Scene.drawImage(IMAGE_TILE_MIDDLE_LOG_INNER_LEFT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_LOG_MIDDLE_INNER_RIGHT:
			Scene.drawImage(IMAGE_TILE_MIDDLE_LOG_INNER_RIGHT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_LOG_MIDDLE_FAR_RIGHT:
			Scene.drawImage(IMAGE_TILE_MIDDLE_LOG_FAR_RIGHT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_LOG_BOTTOM_FAR_LEFT:
			Scene.drawImage(IMAGE_TILE_BOTTOM_LOG_FAR_LEFT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_LOG_BOTTOM_INNER_LEFT:
			Scene.drawImage(IMAGE_TILE_BOTTOM_LOG_INNER_LEFT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_LOG_BOTTOM_INNER_RIGHT:
			Scene.drawImage(IMAGE_TILE_BOTTOM_LOG_INNER_RIGHT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_LOG_BOTTOM_FAR_RIGHT:
			Scene.drawImage(IMAGE_TILE_BOTTOM_LOG_FAR_RIGHT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_PURPLE_SMALL_CRACKS:
			Scene.drawImage(IMAGE_TILE_PURPLE_BLOCK_SMALL_CRACKS, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GRASS_LADDER:
			Scene.drawImage(IMAGE_TILE_GRASS_DIRT_LADDER, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_WATER_MIDDLE:
			Scene.drawImage(IMAGE_TILE_WATER_MIDDLE, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_WATER_TOP:
			Scene.drawImage(IMAGE_TILE_WATER_TOP, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_WARP_BLOCK:
			Scene.drawImage(IMAGE_TILE_WARP_BLOCK, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GROUND_CELLAR_TOP:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_TOP, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GROUND_CELLAR_TOP_RIGHT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_TOP_RIGHT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GROUND_CELLAR_TOP_LEFT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_TOP_LEFT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GROUND_CELLAR_MIDDLE_LEFT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_MIDDLE_LEFT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GROUND_CELLAR_MIDDLE:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_MIDDLE_MIDDLE, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GROUND_CELLAR_MIDDLE_RIGHT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_MIDDLE_RIGHT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GROUND_CELLAR_BOTTOM_LEFT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_BOTTOM_LEFT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GROUND_CELLAR_BOTTOM_MIDDLE:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_BOTTOM_MIDDLE, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GROUND_CELLAR_BOTTOM_RIGHT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_BOTTOM_RIGHT, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GROUND_CELLAR_CONNECTOR_TOP_TO_RIGHT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_CONNECTOR_1, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GROUND_CELLAR_CONNECTOR_TOP_TO_LEFT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_CONNECTOR_2, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GROUND_CELLAR_CONNECTOR_BOTTOM_TO_RIGHT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_CONNECTOR_3, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_GROUND_CELLAR_CONNECTOR_BOTTOM_TO_LEFT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_CONNECTOR_4, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
		case TILE_DECOR_CELING_LAMP:
			Scene.drawImage(IMAGE_TILE_DECOR_CELING_LAMP, floor((coord_click_x)/50)*50, floor((coord_click_y)/50)*50, 50, 50);
		break;
	}
	
	char buffer[50];
	snprintf(buffer, 50, "X: %f", -cameraHorizOffsetPx);
	Scene.drawText(buffer, 0, 0, 30);
	snprintf(buffer, 50, "Y: %f", -cameraVertOffsetPx);
	Scene.drawText(buffer, 0, 30, 30);
	snprintf(buffer, 50, "Current room: %d", currentRoom);
	Scene.drawText(buffer, 0, 90, 30);
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
		case TILE_PURPLE_SMALL_CRACKS:
			snprintf(buffer, 50, "Current tile: Breakable_Small_Cracked");
		break;
		case TILE_GRASS_LADDER:
			snprintf(buffer, 50, "Current tile: Dirt_Style_Ladder");
		break;
		case TILE_WATER_MIDDLE:
			snprintf(buffer, 50, "Current tile: Water_Middle");
		break;
		case TILE_WATER_TOP:
			snprintf(buffer, 50, "Current tile: Water_Top (not colidable)");
		break;
		case TILE_WARP_BLOCK:
			snprintf(buffer, 50, "Current tile: Warp_Block");
		break;
		case TILE_GROUND_CELLAR_TOP:
			snprintf(buffer, 50, "Current tile: Cellar_Ground_Top");
		break;
		case TILE_GROUND_CELLAR_TOP_RIGHT:
			snprintf(buffer, 50, "Current tile: Cellar_Ground_Top_Right");
		break;
		case TILE_GROUND_CELLAR_TOP_LEFT:
			snprintf(buffer, 50, "Current tile: Cellar_Ground_Top_Left");
		break;
		case TILE_GROUND_CELLAR_MIDDLE_LEFT:
			snprintf(buffer, 50, "Current tile: Cellar_Ground_Middle_Left");
		break;
		case TILE_GROUND_CELLAR_MIDDLE:
			snprintf(buffer, 50, "Current tile: Cellar_Ground_Middle");
		break;
		case TILE_GROUND_CELLAR_MIDDLE_RIGHT:
			snprintf(buffer, 50, "Current tile: Cellar_Ground_Middle_Right");
		break;
		case TILE_GROUND_CELLAR_BOTTOM_LEFT:
			snprintf(buffer, 50, "Current tile: Cellar_Ground_Bottom_Left");
		break;
		case TILE_GROUND_CELLAR_BOTTOM_MIDDLE:
			snprintf(buffer, 50, "Current tile: Cellar_Ground_Bottom");
		break;
		case TILE_GROUND_CELLAR_BOTTOM_RIGHT:
			snprintf(buffer, 50, "Current tile: Cellar_Ground_Bottom_Right");
		break;
		case TILE_GROUND_CELLAR_CONNECTOR_TOP_TO_RIGHT:
			snprintf(buffer, 50, "Current tile: Cellar_Ground_Connector_Top_To_Right");
		break;
		case TILE_GROUND_CELLAR_CONNECTOR_TOP_TO_LEFT:
			snprintf(buffer, 50, "Current tile: Cellar_Ground_Connector_Top_To_Left");
		break;
		case TILE_GROUND_CELLAR_CONNECTOR_BOTTOM_TO_RIGHT:
			snprintf(buffer, 50, "Current tile: Cellar_Ground_Connector_Bottom_To_Right");
		break;
		case TILE_GROUND_CELLAR_CONNECTOR_BOTTOM_TO_LEFT:
			snprintf(buffer, 50, "Current tile: Cellar_Ground_Connector_Bottom_To_Left");
		break;
		case TILE_DECOR_CELING_LAMP:
			snprintf(buffer, 50, "Current tile: Cellar_Decor_Celing_Lamp");
		break;
	}
	Scene.drawText(buffer, 0, 60, 30);
	
	
	Scene.finishDrawing();
	
	gameFrame++;
	return true;
}