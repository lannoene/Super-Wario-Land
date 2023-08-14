#include "player.hpp"
#include "tiles.hpp"
#include "tools.hpp"
#include "room.hpp"

int tileCount = 0;
extern int cameraVertOffsetPx;
extern SquishyArray <Room*>Room_array;

Tile::Tile(int input_x, int input_y, int type) {
	this->x = input_x;
	this->y = input_y;
	this->width = TILESIZE;
	this->height = TILESIZE;
	this->type = type;
	this->semisolid = false;
	this->breakable = false;
	this->ladder = false;
	this->visible = true;
	this->colidable = true;
	this->flags.water = false;
	this->flags.warp = false;
	this->tileId = tileCount;
	++tileCount;
	this->animDelay = 9999999;
	this->animTimer = 0;
	this->param1.dummyVar = 0;
	this->param2.dummyVar = 0;
	this->param3.destinationRoomId = 0;
	
	switch (type) {
		default:
			this->colidable = true;
			this->visible = true;
		break;
		case TILE_SPAWN_POINT:
			this->colidable = false;
			this->visible = false;
		break;
		case TILE_WARP_BLOCK:
			this->colidable = false;
			this->visible = false;
			this->flags.warp = true;
		break;
		case GOLD_COIN:
		case TILE_DOOR_BOTTOM:
		case TILE_DIRT_BG:
		case TILE_DIRT_BG_SHADOW:
		case TILE_WATER_TOP:
		case TILE_DECOR_CELING_LAMP:
			this->colidable = false;
			this->visible = true;
		break;
		case TILE_DIRT_SEMISOLID:
			this->semisolid = true;
		break;
		case TILE_PURPLE_SMALL_CRACKS:
			this->breakable = true;
		break;
		case TILE_GRASS_LADDER:
			this->colidable = false;
			this->ladder = true;
		break;
		case TILE_WATER_MIDDLE:
			this->flags.water = true;
			this->colidable = false; // turn off default colisions
		break;
		case TILE_DOOR:
			this->flags.warp = true;
			this->colidable = false;
			this->visible = true;
		break;
	}
}

Tile::~Tile() {
	
}

int Tile::getType(void) {
	return type;
}

void Tile::update(size_t frame, Player &player) {
	// note: never mess with player roomId as a tile
	switch (type) {
		default:
		break;
		case TILE_SPAWN_POINT:
			if (frame == 0) {

			}
		break;
	}
}

void Tile::draw(SDL_Screen &Scene, int gameFrame) {
	if (this->visible == false)
		return;
	
	if (gameFrame % this->animDelay == 0)
		++this->animTimer;
	
	cameraHorizOffsetPx = std::floor(cameraHorizOffsetPx); // will get refreshed next cycle anyways
	
	switch (type) {
		default:
			Scene.drawImage(IMAGE_UNKNOWN, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GRASS_TOP:
			Scene.drawImage(IMAGE_TILE_TOP_GRASS, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GRASS_TOP_RIGHT:
			Scene.drawImage(IMAGE_TILE_TOP_GRASS_RIGHT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GRASS_TOP_LEFT:
			Scene.drawImage(IMAGE_TILE_TOP_GRASS_LEFT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GRASS_MIDDLE_MIDDLE:
			Scene.drawImage(IMAGE_TILE_MIDDLE_GRASS_MIDDLE, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GRASS_MIDDLE_RIGHT:
			Scene.drawImage(IMAGE_TILE_MIDDLE_GRASS_RIGHT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GRASS_MIDDLE_LEFT:
			Scene.drawImage(IMAGE_TILE_MIDDLE_GRASS_LEFT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GRASS_BOTTOM_MIDDLE:
			Scene.drawImage(IMAGE_TILE_BOTTOM_GRASS_MIDDLE, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GRASS_BOTTOM_RIGHT:
			Scene.drawImage(IMAGE_TILE_BOTTOM_GRASS_RIGHT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GRASS_BOTTOM_LEFT:
			Scene.drawImage(IMAGE_TILE_BOTTOM_GRASS_LEFT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GRASS_CONNECTOR_TOP_TO_RIGHT:
			Scene.drawImage(IMAGE_TILE_CONNECTOR_1, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GRASS_CONNECTOR_TOP_TO_LEFT:
			Scene.drawImage(IMAGE_TILE_CONNECTOR_2, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GRASS_CONNECTOR_BOTTOM_TO_RIGHT:
			Scene.drawImage(IMAGE_TILE_CONNECTOR_3, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GRASS_CONNECTOR_BOTTOM_TO_LEFT:
			Scene.drawImage(IMAGE_TILE_CONNECTOR_4, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case GOLD_COIN:
			Scene.drawImage(GOLD_COIN_ROTATION_1, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_DOOR:
			Scene.drawImage(IMAGE_DOOR_TOP, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_DOOR_BOTTOM:
			Scene.drawImage(IMAGE_DOOR_BOTTOM, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_DIRT_BG:
			Scene.drawImage(IMAGE_TILE_GRASS_DIRT_BG, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_DIRT_BG_SHADOW:
			Scene.drawImage(IMAGE_TILE_GRASS_DIRT_BG_SHADOW, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_DIRT_SEMISOLID:
			Scene.drawImage(IMAGE_TILE_GRASS_SEM_PLT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_LOG_MIDDLE_FAR_LEFT:
			Scene.drawImage(IMAGE_TILE_MIDDLE_LOG_FAR_LEFT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_LOG_MIDDLE_INNER_LEFT:
			Scene.drawImage(IMAGE_TILE_MIDDLE_LOG_INNER_LEFT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_LOG_MIDDLE_INNER_RIGHT:
			Scene.drawImage(IMAGE_TILE_MIDDLE_LOG_INNER_RIGHT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_LOG_MIDDLE_FAR_RIGHT:
			Scene.drawImage(IMAGE_TILE_MIDDLE_LOG_FAR_RIGHT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_LOG_BOTTOM_FAR_LEFT:
			Scene.drawImage(IMAGE_TILE_BOTTOM_LOG_FAR_LEFT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_LOG_BOTTOM_INNER_LEFT:
			Scene.drawImage(IMAGE_TILE_BOTTOM_LOG_INNER_LEFT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_LOG_BOTTOM_INNER_RIGHT:
			Scene.drawImage(IMAGE_TILE_BOTTOM_LOG_INNER_RIGHT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_LOG_BOTTOM_FAR_RIGHT:
			Scene.drawImage(IMAGE_TILE_BOTTOM_LOG_FAR_RIGHT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_PURPLE_SMALL_CRACKS:
			Scene.drawImage(IMAGE_TILE_PURPLE_BLOCK_SMALL_CRACKS, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GRASS_LADDER:
			Scene.drawImage(IMAGE_TILE_GRASS_DIRT_LADDER, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_WATER_MIDDLE:
			Scene.drawImage(IMAGE_TILE_WATER_MIDDLE, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_WATER_TOP:
			this->animDelay = 7;
			switch (this->animTimer) {
				default:
					this->animTimer = 0;
					// fallthrough
				case 0:
					Scene.drawImage(IMAGE_TILE_WATER_TOP_1, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
				break;
				case 1:
					Scene.drawImage(IMAGE_TILE_WATER_TOP_2, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
				break;
				case 2:
					Scene.drawImage(IMAGE_TILE_WATER_TOP_3, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
				break;
				case 3:
					Scene.drawImage(IMAGE_TILE_WATER_TOP_4, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
				break;
			}
		break;
		case TILE_GROUND_CELLAR_TOP:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_TOP, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GROUND_CELLAR_TOP_RIGHT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_TOP_RIGHT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GROUND_CELLAR_TOP_LEFT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_TOP_LEFT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GROUND_CELLAR_MIDDLE_LEFT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_MIDDLE_LEFT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GROUND_CELLAR_MIDDLE:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_MIDDLE_MIDDLE, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GROUND_CELLAR_MIDDLE_RIGHT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_MIDDLE_RIGHT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GROUND_CELLAR_BOTTOM_LEFT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_BOTTOM_LEFT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GROUND_CELLAR_BOTTOM_MIDDLE:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_BOTTOM_MIDDLE, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GROUND_CELLAR_BOTTOM_RIGHT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_BOTTOM_RIGHT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GROUND_CELLAR_CONNECTOR_TOP_TO_RIGHT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_CONNECTOR_1, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GROUND_CELLAR_CONNECTOR_TOP_TO_LEFT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_CONNECTOR_2, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GROUND_CELLAR_CONNECTOR_BOTTOM_TO_RIGHT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_CONNECTOR_3, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_GROUND_CELLAR_CONNECTOR_BOTTOM_TO_LEFT:
			Scene.drawImage(IMAGE_TILE_GROUND_CELLAR_CONNECTOR_4, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_DECOR_CELING_LAMP:
			Scene.drawImage(IMAGE_TILE_DECOR_CELING_LAMP, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
	}
}

PhysicsTile::PhysicsTile(int input_x, int input_y, int type) : Tile(input_x, input_y, type) {
	this->x = input_x;
	this->y = input_y;
	this->width = TILESIZE;
	this->height = TILESIZE;
	this->type = type;
	this->semisolid = false;
	this->breakable = false;
	this->colidable = false;
	this->tileId = tileCount;
	this->tileFallState = JUMPING;
	++tileCount;
	
	switch (type) {
		default:
			this->colidable = true;
			this->visible = true;
		break;
		case TILE_SPAWN_POINT:
			this->colidable = false;
			this->visible = false;
		break;
		case GOLD_COIN:
		case TILE_DOOR:
		case TILE_DOOR_BOTTOM:
		case TILE_DIRT_BG:
		case TILE_DIRT_BG_SHADOW:
			this->colidable = false;
			this->visible = true;
		break;
		case TILE_DIRT_SEMISOLID:
			this->visible = true;
			this->semisolid = true;
			this->colidable = true;
		break;
		case TILE_PURPLE_SMALL_CRACKS:
			this->visible = true;
			this->colidable = true;
			this->breakable = true;
		break;
	}
	if (rand()%2) {
		this->tileHorizVect = 5;
	} else {
		this->tileHorizVect = -5;
	}
	tileDeceleration[false] = 0.02;
	tileDeceleration[true] = 0.1;
	this->tileVertVect = 0;
}

PhysicsTile::~PhysicsTile(void) {
}


void PhysicsTile::update(size_t frame, Player &player) {
	bool tileTouchingGround = false;
	this->tileVertVect -= GRAVITY;
	for (size_t i = 0; i < Room_array.data()[player.getCurrentRoomId()]->Room_tiles.length(); i++) {
		if (tools::checkVertBounds(this->x, this->y - this->tileVertVect, this->width, this->height, Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->x, Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->y, Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->width, Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->height) && Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->tileId != this->tileId) {
			if (Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->colidable == true) {
				if (!Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->semisolid || (Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->semisolid && this->y + (float)this->height <= Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->y)) {
					tileTouchingGround = true;
				}
			}
		}
	}
	
	
	if (this->tileVertVect < 0) { // there's a bug where when a tile hits the top of another tile, it will get teleported to the top of the other tile because it is thinking it is falling, therefore it SHOULD teleport it to the top. the problem is that i actually want it to not do that when it hits the bottom of the tile i want it to bounce off! this can be solved with movement states like i have done with the player already. ¡TODO!
		for (size_t i = 0; i < Room_array.data()[player.getCurrentRoomId()]->Room_tiles.length(); i++) {
			if (tools::checkVertBounds(this->x, this->y - this->tileVertVect, this->width, this->height, Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->x, Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->y, Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->width, Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->height) && Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->tileId != this->tileId) {
				if (Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->colidable == true) {
					if (!Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->semisolid || (Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->semisolid && this->y + (float)this->height <= Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->y)) {
						this->tileVertVect = -this->tileVertVect*this->elasticity;
						this->y = Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->y - this->height;
						if (this->tileVertVect < 1 && this->tileVertVect > -1) {
							this->tileVertVect = 0;
						}
					}
				}
			}
		}
	} else if (this->tileVertVect > 0 && !tileTouchingGround) {
		for (size_t i = 0; i < Room_array.data()[player.getCurrentRoomId()]->Room_tiles.length(); i++) {
			if (tools::checkVertBounds(this->x, this->y - this->tileVertVect, this->width, this->height, Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->x, Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->y, Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->width, Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->height)) {
				if (Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->colidable == true && Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->tileId != this->tileId) {
					if (!tileTouchingGround && !Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->semisolid) {
						//vertVect = -(Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[intersectingBottomForCeling]->y - this->y + Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[intersectingBottomForCeling]->height) - 1;
						this->tileVertVect = 0;
						this->y = Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->y + Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->height;
					}
				}
			}
		}
	}
	
	if (this->tileVertVect > 0) {
		this->tileFallState =JUMPING; 
	} else if (this->tileVertVect < 0) {
		this->tileFallState = FALLING;
	} else if (tileTouchingGround) {
		this->tileFallState = STANDING;
	}
	
	
	if (this->tileHorizVect > 0)
		this->tileHorizVect -= this->tileDeceleration[tileTouchingGround];
	else if (this->tileHorizVect < 0)
		this->tileHorizVect -= -this->tileDeceleration[tileTouchingGround];
	
	if (this->tileHorizVect < this->tileDeceleration[tileTouchingGround] && this->tileHorizVect > -this->tileDeceleration[tileTouchingGround])
		this->tileHorizVect = 0;
	
	for (size_t i = 0; i < Room_array.data()[player.getCurrentRoomId()]->Room_tiles.length(); i++) {
		if (tools::checkHorizBounds(this->x + this->tileHorizVect, this->y, this->width, this->height, Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->x, Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->y, Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->width, Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->height)) {
			if (this->tileHorizVect > 0 && !Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->semisolid && Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->colidable) {
				//horizVect = Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[intersectingTileId]->x - this->x - this->hitboxWidth;
				this->tileHorizVect = -this->tileHorizVect*this->elasticity;
				this->x = Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->x - this->width;
			} else if (this->tileHorizVect < 0 && !Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->semisolid && Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->colidable) {
				//horizVect = Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[intersectingTileId]->x + Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[intersectingTileId]->width - this->x;
				this->tileHorizVect = -this->tileHorizVect*this->elasticity;
				this->x = Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->x + Room_array.data()[player.getCurrentRoomId()]->Room_tiles.data()[i]->width;
			}
		}
	}
	
	this->y -= this->tileVertVect;
	this->x += this->tileHorizVect;
}