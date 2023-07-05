#pragma once
#define GRAVITY	0.01
#include "draw.hpp"

//put walls & stuff here
extern float cameraHorizOffsetPx;

class Tile {
public:
	int x = 0;
	int y = 0;
	int width = 1;
	int height = 1;
	Tile(int input_x, int input_y, int input_width, int input_height) {
		this->x = input_x;
		this->y = input_y;
		this->width = input_width;
		this->height = input_height;
	}
	~Tile() {
		
	}
	void draw(SDL_Screen &Scene) {
		Scene.drawImage(IMAGE_TILE_TOP_GRASS, x + cameraHorizOffsetPx, y, width, height);
	}
private:
};