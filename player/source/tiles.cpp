#include "player.hpp"
#include "tiles.hpp"
#include "tools.hpp"

int tileCount = 0;
extern int cameraVertOffsetPx;

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
	switch (type) {
		default:
		break;
		case TILE_SPAWN_POINT:
			if (frame == 0) {
				player.setXYPos(this->x, this->y);
			}
		break;
	}
}

void Tile::draw(SDL_Screen &Scene, int gameFrame) {
	if (this->visible == false)
		return;
	
	if (gameFrame % this->animDelay == 0)
		++this->animTimer;
	
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
}

PhysicsTile::~PhysicsTile(void) {
}

extern SquishyArray <Tile*>Tile_array;

void PhysicsTile::update(size_t frame, Player &player) {
	bool tileTouchingGround = false;
	for (size_t i = 0; i < Tile_array.length(); i++) {
		if (tools::checkVertBounds(this->x, this->y - this->tileVertVect, this->width, this->height, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height) && Tile_array.data()[i]->tileId != this->tileId) {
			if (Tile_array.data()[i]->colidable == true) {
				if (!Tile_array.data()[i]->semisolid || (Tile_array.data()[i]->semisolid && this->y + (float)this->height <= Tile_array.data()[i]->y)) {
					this->tileVertVect = -this->tileVertVect*this->elasticity;
					this->y = Tile_array.data()[i]->y - this->height;
					if (this->tileVertVect < 1 && this->tileVertVect > -1) {
						this->tileVertVect = 0;
					}
					tileTouchingGround = true;
				}
			}
		}
	}
	
	for (size_t i = 0; i < Tile_array.length(); i++) {
		if (tools::checkVertBounds(this->x, this->y - this->tileVertVect - 1, this->width, this->height, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
			if (Tile_array.data()[i]->colidable == true && Tile_array.data()[i]->tileId != this->tileId) {
				if (!tileTouchingGround && !Tile_array.data()[i]->semisolid) {
					//vertVect = -(Tile_array.data()[intersectingBottomForCeling]->y - this->y + Tile_array.data()[intersectingBottomForCeling]->height) - 1;
					this->tileVertVect = 0;
					this->y = Tile_array.data()[i]->y + Tile_array.data()[i]->height + 1;
				}
			}
		}
	}
	
	if (!tileTouchingGround)
		this->tileVertVect -= GRAVITY;
	
	
	
	if (this->tileHorizVect > 0)
		this->tileHorizVect -= this->tileDeceleration[tileTouchingGround];
	else if (this->tileHorizVect < 0)
		this->tileHorizVect -= -this->tileDeceleration[tileTouchingGround];
	
	if (this->tileHorizVect < this->tileDeceleration[tileTouchingGround] && this->tileHorizVect > -this->tileDeceleration[tileTouchingGround])
		this->tileHorizVect = 0;
	
	for (size_t i = 0; i < Tile_array.length(); i++) {
		if (tools::checkHorizBounds(this->x + this->tileHorizVect, this->y, this->width, this->height, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
			if (this->tileHorizVect > 0 && !Tile_array.data()[i]->semisolid && Tile_array.data()[i]->colidable) {
				//horizVect = Tile_array.data()[intersectingTileId]->x - this->x - this->hitboxWidth;
				this->tileHorizVect = -this->tileHorizVect*this->elasticity;
				this->x = Tile_array.data()[i]->x - this->width;
			} else if (this->tileHorizVect < 0 && !Tile_array.data()[i]->semisolid && Tile_array.data()[i]->colidable) {
				//horizVect = Tile_array.data()[intersectingTileId]->x + Tile_array.data()[intersectingTileId]->width - this->x;
				this->tileHorizVect = -this->tileHorizVect*this->elasticity;
				this->x = Tile_array.data()[i]->x + Tile_array.data()[i]->width;
			}
		}
	}
	
	this->y -= this->tileVertVect;
	this->x += this->tileHorizVect;
}