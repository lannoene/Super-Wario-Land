#include "room.hpp"

Room::Room() {
	this->width = 1000;
	this->height = 600;
}
Room::~Room() {
	
}


void Room::setRoomWidth(int newWidth) {
	this->width = newWidth;
}
void Room::setRoomHeight(int newHeight) {
	this->height = newHeight;
}
int Room::roomWidth(void) {
	return width;
}
int Room::roomHeight(void) {
	return height;
}

void Room::setRoomId(int id) {
	this->roomId = id;
}

void Room::drawBackground(SDL_Screen &Scene, float cameraHorizOffsetPx, int cameraVertOffsetPx) {
	switch (this->backgroundId) {
		case BG_GRASS:
			Scene.drawImage(IMAGE_BG_LEVEL, (-floor(cameraHorizOffsetPx/10/1920 + 1)*1920*10 + cameraHorizOffsetPx/10), 0, 1920, 600);
			Scene.drawImage(IMAGE_BG_LEVEL, (-floor(cameraHorizOffsetPx/10/1920 + 1)*1920*10 + cameraHorizOffsetPx/10) + 1920*10, 0, 1920, 600);
		break;
		case BG_CAVE:
			int newImageWidth = Scene.imageWidth(IMAGE_BG_CAVE) * 600/Scene.imageHeight(IMAGE_BG_CAVE);
			Scene.drawImage(IMAGE_BG_CAVE, (-floor(cameraHorizOffsetPx/newImageWidth + 1)*newImageWidth + cameraHorizOffsetPx), cameraVertOffsetPx, newImageWidth, 600);
			Scene.drawImage(IMAGE_BG_CAVE, (-floor(cameraHorizOffsetPx/newImageWidth + 1)*newImageWidth + cameraHorizOffsetPx) + newImageWidth, cameraVertOffsetPx, newImageWidth, 600);
			Scene.drawImage(IMAGE_BG_CAVE, (-floor(cameraHorizOffsetPx/newImageWidth + 1)*newImageWidth + cameraHorizOffsetPx), 600 + cameraVertOffsetPx, newImageWidth, 600);
			Scene.drawImage(IMAGE_BG_CAVE, (-floor(cameraHorizOffsetPx/newImageWidth + 1)*newImageWidth + cameraHorizOffsetPx) + newImageWidth, 600 + cameraVertOffsetPx, newImageWidth, 600);
		break;
	}
}

void Room::roomSetMusic(int newMusicId) {
	this->musicId = newMusicId;
}

void Room::roomSetBackground(int newBackgroundId) {
	this->backgroundId = newBackgroundId;
}

int Room::roomGetBackgroundId(void) {
	return this->backgroundId;
}

int Room::roomGetMusicId(void) {
	return this->musicId;
}

void Room::roomPlayBackgroundMusic(SDL_Audio &Audio) {
	Audio.playMusic(this->musicId, -1);
}