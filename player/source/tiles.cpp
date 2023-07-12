#include "player.hpp"
#include "tiles.hpp"

Tile::Tile(int input_x, int input_y, int type) {
	this->x = input_x;
	this->y = input_y;
	this->width = TILESIZE;
	this->height = TILESIZE;
	this->type = type;
	
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
				puts("done");
			}
		break;
	}
}

void Tile::draw(SDL_Screen &Scene) {
	if (this->visible == false)
		return;
	
	switch (type) {
		default:
			Scene.drawImage(IMAGE_UNKNOWN, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_GRASS_TOP:
			Scene.drawImage(IMAGE_TILE_TOP_GRASS, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_GRASS_TOP_RIGHT:
			Scene.drawImage(IMAGE_TILE_TOP_GRASS_RIGHT, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_GRASS_TOP_LEFT:
			Scene.drawImage(IMAGE_TILE_TOP_GRASS_LEFT, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_GRASS_MIDDLE_MIDDLE:
			Scene.drawImage(IMAGE_TILE_MIDDLE_GRASS_MIDDLE, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_GRASS_MIDDLE_RIGHT:
			Scene.drawImage(IMAGE_TILE_MIDDLE_GRASS_RIGHT, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_GRASS_MIDDLE_LEFT:
			Scene.drawImage(IMAGE_TILE_MIDDLE_GRASS_LEFT, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_GRASS_BOTTOM_MIDDLE:
			Scene.drawImage(IMAGE_TILE_BOTTOM_GRASS_MIDDLE, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_GRASS_BOTTOM_RIGHT:
			Scene.drawImage(IMAGE_TILE_BOTTOM_GRASS_RIGHT, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_GRASS_BOTTOM_LEFT:
			Scene.drawImage(IMAGE_TILE_BOTTOM_GRASS_LEFT, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_GRASS_CONNECTOR_TOP_TO_RIGHT:
			Scene.drawImage(IMAGE_TILE_CONNECTOR_1, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_GRASS_CONNECTOR_TOP_TO_LEFT:
			Scene.drawImage(IMAGE_TILE_CONNECTOR_2, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_GRASS_CONNECTOR_BOTTOM_TO_RIGHT:
			Scene.drawImage(IMAGE_TILE_CONNECTOR_3, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_GRASS_CONNECTOR_BOTTOM_TO_LEFT:
			Scene.drawImage(IMAGE_TILE_CONNECTOR_4, x + cameraHorizOffsetPx, y, width, height);
		break;
		case GOLD_COIN:
			Scene.drawImage(GOLD_COIN_ROTATION_1, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_DOOR:
			Scene.drawImage(IMAGE_DOOR_TOP, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_DOOR_BOTTOM:
			Scene.drawImage(IMAGE_DOOR_BOTTOM, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_DIRT_BG:
			Scene.drawImage(IMAGE_TILE_GRASS_DIRT_BG, x + cameraHorizOffsetPx, y, width, height);
		break;
		case TILE_DIRT_BG_SHADOW:
			Scene.drawImage(IMAGE_TILE_GRASS_DIRT_BG_SHADOW, x + cameraHorizOffsetPx, y, width, height);
		break;
	}
}