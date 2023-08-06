#include <iostream>
#include <format>

#include "tiles.hpp"

Tile::Tile(int input_x, int input_y, int type) {
	this->x = input_x;
	this->y = input_y;
	this->width = TILESIZE;
	this->height = TILESIZE;
	this->type = type;
}

Tile::~Tile() {
	
}

int Tile::getType(void) {
	return type;
}

void Tile::draw(SDL_Screen &Scene) {
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
		case TILE_DOOR: {
			Scene.drawImage(IMAGE_DOOR_TOP, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
			Scene.drawRectangle(x + cameraHorizOffsetPx, y + cameraVertOffsetPx, 25, 20, CLR_WHT);
			char doorWarpId[5];
			snprintf(doorWarpId, 5, "%d", this->param1.warpId);
			Scene.drawText(doorWarpId, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, 20);
		}
		break;
		case TILE_DOOR_BOTTOM:
			Scene.drawImage(IMAGE_DOOR_BOTTOM, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_SPAWN_POINT:
			Scene.drawImage(IMAGE_SPAWN_POINT, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
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
			Scene.drawImage(IMAGE_TILE_WATER_TOP, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
		break;
		case TILE_WARP_BLOCK: {
			Scene.drawImage(IMAGE_TILE_WARP_BLOCK, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, width, height);
			Scene.drawRectangle(x + cameraHorizOffsetPx, y + cameraVertOffsetPx, 25, 20, CLR_WHT);
			char doorWarpId[5];
			snprintf(doorWarpId, 5, "%d", this->param1.warpId);
			Scene.drawText(doorWarpId, x + cameraHorizOffsetPx, y + cameraVertOffsetPx, 20);
		}
		break;
	}
}

std::string Tile::appendOutputToString(void) {
	struct s_level {
		int x;
		int y;
		int type;
		int param1;
		int param2;
		int param3;
	};
	struct s_level levelStr(this->x, this->y, this->type, this->param1.dummyVar, this->param2.dummyVar, this->param3.dummyVar);
	FILE *of;
	of = fopen("levelStr.lvl", "a");
	fwrite(&levelStr, sizeof(struct s_level), 1, of);
	fclose(of);
	return std::to_string(this->x)+","+std::to_string(this->y)+","+std::to_string(this->type)+","+std::to_string(this->param1.dummyVar)+","+std::to_string(this->param2.dummyVar)+",;";
}