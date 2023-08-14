#include "draw.hpp"
#include "squishy_array.hpp"

SquishyArray <SDL_Texture*>sdl_image(0);

SDL_Screen::SDL_Screen() {
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Super Wario Land - Editor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	setWindowIcon((char*)"romfs/misc/icon.png");
	
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
	loadTexture((char*)"romfs/misc/spawn_point.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_dirt_bg_tile.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_dirt_bg_tile_shadow.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_dirt_semisol_platform.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_log_middle_left.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_log_middle_kinda_left.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_log_middle_kinda_right.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_log_middle_right.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_log_bottom_left.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_log_bottom_kinda_left.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_log_bottom_kinda_right.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_log_bottom_right.png");
	loadTexture((char*)"romfs/texture/grass/purple_block_small_cracks.png");
	loadTexture((char*)"romfs/texture/grass/level_ground_grass_ladder.png");
	loadTexture((char*)"romfs/texture/grass/level_water_inside.png");
	loadTexture((char*)"romfs/texture/grass/level_water_top1.png");
	loadTexture((char*)"romfs/misc/warp_blk.png");
	loadTexture((char*)"romfs/bg/cave_bg.png");
	loadTexture((char*)"romfs/texture/cellar/cellar_ground_top.png");
	loadTexture((char*)"romfs/texture/cellar/cellar_ground_top_right.png");
	loadTexture((char*)"romfs/texture/cellar/cellar_ground_top_left.png");
	loadTexture((char*)"romfs/texture/cellar/cellar_ground_middle_left.png");
	loadTexture((char*)"romfs/texture/cellar/cellar_ground_middle_middle.png");
	loadTexture((char*)"romfs/texture/cellar/cellar_ground_middle_right.png");
	loadTexture((char*)"romfs/texture/cellar/cellar_ground_bottom_left.png");
	loadTexture((char*)"romfs/texture/cellar/cellar_ground_bottom_middle.png");
	loadTexture((char*)"romfs/texture/cellar/cellar_ground_bottom_right.png");
	loadTexture((char*)"romfs/texture/cellar/cellar_ground_connector_top_to_right.png");
	loadTexture((char*)"romfs/texture/cellar/cellar_ground_connector_top_to_left.png");
	loadTexture((char*)"romfs/texture/cellar/cellar_ground_connector_bottom_to_right.png");
	loadTexture((char*)"romfs/texture/cellar/cellar_ground_connector_bottom_to_left.png");
	loadTexture((char*)"romfs/texture/cellar/cellar_decor_celeing_lamp.png");
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
	SDL_UpdateWindowSurface(window);
}

void SDL_Screen::loadTexture(char* filePath) {
	SDL_Surface* image_sur;
	image_sur = IMG_Load(filePath);
	if (image_sur == nullptr)
		printf("could not load texture: %s\n", filePath);
	SDL_Texture *t = SDL_CreateTextureFromSurface(rend, image_sur);
	sdl_image.array_push(t);
	SDL_FreeSurface(image_sur);
}

void SDL_Screen::setTitle(char* title) {
	SDL_SetWindowTitle(window, title);
}

void SDL_Screen::setWindowIcon(char* filePath) {
	SDL_Surface* loadedSurface = IMG_Load(filePath);
	SDL_SetWindowIcon(window, loadedSurface);
	SDL_FreeSurface(loadedSurface);
}

void SDL_Screen::drawRectangle(int x, int y, int width, int height, int color) {
	SDL_Rect rect = {x, y, width, height};
	switch (color) {
		case CLR_BLU:
			SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0xff, SDL_ALPHA_OPAQUE);
		break;
		case CLR_WHT:
			SDL_SetRenderDrawColor(rend, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
		break;
		case CLR_RED:
			SDL_SetRenderDrawColor(rend, 0xff, 0x00, 0x00, SDL_ALPHA_OPAQUE);
		break;
		case CLR_GRN:
			SDL_SetRenderDrawColor(rend, 0x00, 0xff, 0x00, SDL_ALPHA_OPAQUE);
		break;
		case CLR_BLK:
			SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
		break;
		case CLR_YLW:
			SDL_SetRenderDrawColor(rend, 0xff, 0xff, 0x00, SDL_ALPHA_OPAQUE);
		break;
		case CLR_GRY:
			SDL_SetRenderDrawColor(rend, 0x80, 0x80, 0x80, SDL_ALPHA_OPAQUE);
		break;
	}
	SDL_RenderFillRect(rend, &rect);
}

void SDL_Screen::drawLineRectangle(int x, int y, int width, int height, int color) {
	SDL_Rect rect = {x, y, width, height};
	switch (color) {
		case CLR_BLU:
			SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0xff, SDL_ALPHA_OPAQUE);
		break;
		case CLR_WHT:
			SDL_SetRenderDrawColor(rend, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
		break;
		case CLR_RED:
			SDL_SetRenderDrawColor(rend, 0xff, 0x00, 0x00, SDL_ALPHA_OPAQUE);
		break;
		case CLR_GRN:
			SDL_SetRenderDrawColor(rend, 0x00, 0xff, 0x00, SDL_ALPHA_OPAQUE);
		break;
		case CLR_BLK:
			SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
		break;
		case CLR_YLW:
			SDL_SetRenderDrawColor(rend, 0xff, 0xff, 0x00, SDL_ALPHA_OPAQUE);
		break;
		case CLR_GRY:
			SDL_SetRenderDrawColor(rend, 0x80, 0x80, 0x80, SDL_ALPHA_OPAQUE);
		break;
	}
	SDL_RenderDrawRect(rend, &rect);
}

int SDL_Screen::imageWidth(int imageId) {
	int x;
	SDL_QueryTexture(sdl_image.data()[imageId], NULL, NULL, &x, NULL);
	return x;
}
int SDL_Screen::imageHeight(int imageId) {
	int y;
	SDL_QueryTexture(sdl_image.data()[imageId], NULL, NULL, NULL, &y);
	return y;
}

void SDL_Screen::toggleWindowFullscreen(void) {

	if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) {
		SDL_SetWindowFullscreen(window, 0);
		SDL_ShowCursor(true);
	} else {
		SDL_ShowCursor(false);
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}

}