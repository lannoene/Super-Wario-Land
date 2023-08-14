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
	double getMusicPos(void);
	void setMusicPos(double newPos);
	double getCurrentMusicDuration(void);
	void stopMusic(void);
private:
	SquishyArray <Mix_Chunk*>SDL_SFX_Array{0};
	SquishyArray <Mix_Music*>SDL_Music_Array{0};
	int musicCurrentlyPlaying = -1;
};

enum SFX_ID {
	SFX_JUMP = 0,
	SFX_LAND,
	SFX_COLLECT_COIN,
	SFX_WALK,
	SFX_DASH,
	SFX_BREAK,
	SFX_BUMP,
	SFX_CLIMB,
	SFX_CROUCH_WALK,
	SFX_LAND_IN_WATER,
	SFX_SLOW_SWIM,
};

enum MUSIC_ID {
	MUSIC_GRASS = 0,
	MUSIC_CELLAR,
	MUSIC_VILLAGE,
};