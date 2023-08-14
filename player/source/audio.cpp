#include "audio.hpp"

SDL_Audio::SDL_Audio() {
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OPUS | MIX_INIT_OGG);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 6, 4096);
	
	loadSFX((char*)"romfs/audio/sfx/jump.wav");
	loadSFX((char*)"romfs/audio/sfx/land.wav");
	loadSFX((char*)"romfs/audio/sfx/coin.wav");
	loadSFX((char*)"romfs/audio/sfx/walk.wav");
	loadSFX((char*)"romfs/audio/sfx/dash_normal.wav");
	loadSFX((char*)"romfs/audio/sfx/break.wav");
	loadSFX((char*)"romfs/audio/sfx/bump.wav");
	loadSFX((char*)"romfs/audio/sfx/climb.wav");
	loadSFX((char*)"romfs/audio/sfx/crouch_walk.wav");
	loadSFX((char*)"romfs/audio/sfx/land_in_water.wav");
	loadSFX((char*)"romfs/audio/sfx/swimming.wav");
	loadMusic((char*)"romfs/audio/bgm/bgm_grass.mp3");
	loadMusic((char*)"romfs/audio/bgm/bgm_cellar.mp3");
	loadMusic((char*)"romfs/audio/bgm/bgm_village.mp3");
}

SDL_Audio::~SDL_Audio() {
	for (size_t i = 0; i < SDL_SFX_Array.length(); i++) {
		Mix_FreeChunk(SDL_SFX_Array.data()[i]);
	}
	for (size_t i = 0; i < SDL_Music_Array.length(); i++) {
		Mix_FreeMusic(SDL_Music_Array.data()[i]);
	}
	Mix_CloseAudio();
	Mix_Quit();
}

void SDL_Audio::loadSFX(char* filePath) {
	SDL_SFX_Array.array_push(Mix_LoadWAV(filePath));
}

void SDL_Audio::playSFX(int audioId, int channel) {
	Mix_PlayChannel(channel, SDL_SFX_Array.data()[audioId], 0);
}

void SDL_Audio::loadMusic(char* filePath) {
	SDL_Music_Array.array_push(Mix_LoadMUS(filePath));
	if (SDL_Music_Array.data()[SDL_Music_Array.shortLen()] == NULL)
		printf("Could not load music: %s", filePath);
}

void SDL_Audio::playMusic(int musicId, int loopCount) {
	if (musicId != musicCurrentlyPlaying || !Mix_PlayingMusic())
	Mix_PlayMusic(SDL_Music_Array.data()[musicId], loopCount);
	musicCurrentlyPlaying = musicId;
}

double SDL_Audio::getMusicPos(void) {
	return Mix_GetMusicPosition(SDL_Music_Array.data()[musicCurrentlyPlaying]);
}

void SDL_Audio::setMusicPos(double newPos) {
	Mix_SetMusicPosition(newPos);
}

double SDL_Audio::getCurrentMusicDuration(void) {
	return Mix_MusicDuration(SDL_Music_Array.data()[musicCurrentlyPlaying]);
}

void SDL_Audio::stopMusic(void) {
	Mix_HaltMusic();
}