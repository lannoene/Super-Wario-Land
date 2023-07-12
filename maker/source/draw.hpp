#pragma once
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

class SDL_Screen {
public:
	SDL_Screen();
	~SDL_Screen();
	void clearScreen(void);
	void finishDrawing(void);
	void drawText(char* inputText, int x, int y, float textSize);
	void drawImage(int imageId, int x, int y, int width, int height);
	void flipImage(int imageId);
	void setTitle(char* title);
private:
	//window rend vars
	SDL_Window *window = NULL;
	SDL_Surface *screen = NULL;
	SDL_Renderer *rend = NULL;
	//text vars
	TTF_Font* font;
	SDL_Surface* textSurf;
	SDL_Texture* textTexture;
	SDL_Rect textRect;
	const int text_load_size = 50;//px
	void loadTexture(char* filePath);
	void setWindowIcon(char* filePath);
};

enum imageNums {
	IMAGE_BG_TITLESCREEN = 0,
	IMAGE_PLAYER_WARIO,
	IMAGE_BG_LEVEL,
	IMAGE_TILE_TOP_GRASS,
	IMAGE_HITBOX_RECTANGLE,
	IMAGE_TILE_TOP_GRASS_RIGHT,
	IMAGE_TILE_TOP_GRASS_LEFT,
	IMAGE_TILE_MIDDLE_GRASS_MIDDLE,
	IMAGE_TILE_MIDDLE_GRASS_RIGHT,
	IMAGE_TILE_MIDDLE_GRASS_LEFT,
	IMAGE_TILE_BOTTOM_GRASS_MIDDLE,
	IMAGE_TILE_BOTTOM_GRASS_RIGHT,
	IMAGE_TILE_BOTTOM_GRASS_LEFT,
	IMAGE_TILE_CONNECTOR_1,
	IMAGE_TILE_CONNECTOR_2,
	IMAGE_TILE_CONNECTOR_3,
	IMAGE_TILE_CONNECTOR_4,
	IMAGE_UNKNOWN,
	GOLD_COIN_ROTATION_1,
	IMAGE_DOOR_TOP,
	IMAGE_DOOR_BOTTOM,
	IMAGE_SPAWN_POINT,
	IMAGE_TILE_GRASS_DIRT_BG,
	IMAGE_TILE_GRASS_DIRT_BG_SHADOW
};