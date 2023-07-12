#include "draw.hpp"
SquishyArray <SDL_Texture*>sdl_image(0);

SDL_Screen::SDL_Screen() {
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Super Wario Land", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	screen = SDL_GetWindowSurface(window);
	rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	TTF_Init();
	font = TTF_OpenFont("romfs/font/Arial.ttf", text_load_size);
	
	if (font == NULL) {
		puts("font is null");
	}
	
	loadTexture((char*)"romfs/bg/wario-land-4-titlescreen.png");
	loadTexture((char*)"romfs/sprite/wario_player_stand.png");
	loadTexture((char*)"romfs/bg/level_bg.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_grass.png");
	loadTexture((char*)"romfs/misc/hitbox_rectangle.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_grass_top_right.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_grass_top_left.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_grass_middle_middle.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_grass_middle_right.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_grass_middle_left.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_grass_bottom_middle.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_grass_bottom_right.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_grass_bottom_left.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_grass_connector_top_right.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_grass_connector_top_left.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_grass_connector_bottom_right.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_grass_connector_bottom_left.png");
	loadTexture((char*)"romfs/misc/unknown_image_template.png");
	loadTexture((char*)"romfs/misc/coin_rot1.png");
	loadTexture((char*)"romfs/misc/door_top.png");
	loadTexture((char*)"romfs/misc/door_bottom.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_dirt_bg_tile.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_dirt_bg_tile_shadow.png");
	loadTexture((char*)"romfs/sprite/wario_player_fall.png");
	loadTexture((char*)"romfs/sprite/wario_player_walk1.png");
	loadTexture((char*)"romfs/sprite/wario_player_walk2.png");
	loadTexture((char*)"romfs/sprite/wario_player_walk3.png");
	loadTexture((char*)"romfs/sprite/wario_player_walk4.png");
	
}

SDL_Screen::~SDL_Screen() {
	TTF_Quit();
	SDL_Quit();
}

void SDL_Screen::clearScreen(void) {
	SDL_SetRenderDrawColor(rend, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(rend);
}

void SDL_Screen::drawImage(int imageId, int x, int y, int width, int height) {
	SDL_Rect rect = {x, y, width, height};
	SDL_RenderCopy(rend, sdl_image.data()[imageId], NULL, &rect);
}

void SDL_Screen::drawText(char* inputText, int x, int y, float textSize) {
	textSurf = TTF_RenderText_Solid(font, inputText, (SDL_Color){0,0,0});

	float textMultiple = textSurf->h/textSize;
	
	int textWidth = textSurf->w/textMultiple;
	int textHeight = textSize;
	
	textRect = (SDL_Rect){x, y, textWidth, textHeight};
	textTexture = SDL_CreateTextureFromSurface(rend, textSurf);
	SDL_FreeSurface(textSurf);
	
	SDL_RenderCopy(rend, textTexture, NULL, &textRect);
	SDL_DestroyTexture(textTexture);
}

void SDL_Screen::finishDrawing(void) {
	SDL_RenderPresent(rend);
	SDL_GL_SwapWindow(window);
}

void SDL_Screen::loadTexture(char* filePath) {
	SDL_Surface* image_sur;
	image_sur = IMG_Load(filePath);
	SDL_Texture *t = SDL_CreateTextureFromSurface(rend, image_sur);
	sdl_image.array_push(t);
	SDL_FreeSurface(image_sur);
}

void SDL_Screen::drawRectangle(int x, int y, int width, int height) {
	SDL_Rect rect = {x, y, width, height};
	SDL_RenderFillRect(rend, &rect);

}

void SDL_Screen::drawImageWithDir(int imageId, int x, int y, int width, int height, bool flip) {
	SDL_RendererFlip flipType;
	if (flip)
		flipType = SDL_FLIP_HORIZONTAL;
	else
		flipType = SDL_FLIP_NONE;
	
	SDL_Rect rect = {x, y, width, height};
	
	SDL_RenderCopyEx(rend, sdl_image.data()[imageId], NULL/*src rect.. could be useful for spritesheet*/, &rect, 0, NULL, flipType);

}