#pragma once

#include <SDL2/SDL_mixer.h>
#include "squishy_array.hpp"

class SDL_Audio {
public:
	SDL_Audio();
	~SDL_Audio();
	void loadSFX(char* filePath);
	void playSFX(int audioId, int channel);
	void loadMusic(char* filePath);
	void playMusic(int musicId, int loopCount);
private:
};

enum SFX_ID {
	SFX_JUMP = 0,
	SFX_LAND,
	SFX_COLLECT_COIN,
	SFX_WALK,
	SFX_DASH,
	SFX_BREAK,
	SFX_BUMP,
	SFX_CLIMB
};

enum MUSIC_ID {
	MUSIC_GRASS
};