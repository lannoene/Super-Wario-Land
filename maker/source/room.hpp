#pragma once

#include "squishy_array.hpp"
#include "draw.hpp"
#include "tiles.hpp"

class Room {
public:
	Room();
	~Room();
	void setRoomWidth(int width);
	void setRoomHeight(int height);
	int roomWidth(void);
	int roomHeight(void);
	void setRoomId(int id);
	void drawBackground(SDL_Screen &Scene, float cameraHorizOffsetPx);
	void roomSetMusic(int newMusicId);
	void roomSetBackground(int newBackgroundId);
	int roomGetBackgroundId(void);
	int roomGetMusicId(void);
	
	SquishyArray <Tile*>Room_tiles{0};
private:
	int width, height, roomId;
	int backgroundId = 0;
	int musicId;
};

enum backgroundIds {
	BG_GRASS = 0,
	BG_CAVE,
};

enum bg_musicIds {
	BGM_GRASS = 0,
	BGM_CELLAR,
};