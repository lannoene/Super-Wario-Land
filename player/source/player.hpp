#pragma once

#include "draw.hpp"
#include "audio.hpp"

#define NONE	0
#define LEFT	1
#define RIGHT	2

#define GRAVITY	0.3

class Player {
public:
	Player();
	~Player();
	void update(SDL_Audio &Audio);
	void draw(SDL_Screen &Scene, int gameFrame);
	void jump(SDL_Audio &Audio);
	void moveHoriz(int dir, SDL_Audio &Audio);
	void stopMoving(int dir);
	void setXYPos(int x, int y);
	int getCoinCount(void);
	void enterDoor(void);
private:
	float x, y, horizVect, vertVect;
	int hitboxWidth, hitboxHeight, spriteWidth, spriteHeight, horizSpriteOffset, vertSpriteOffset, animTimer, animDelay, animState, prevAnimState, animDirection;
	bool isTouchingGround;
	int playerState;
	int playerPressedMoveDir;
	float playerAcceleration = 0.3;
	float playerDeceleration[2];
	float playerMaxHorizSpeed = 6;
	float playerMaxVertSpeed = 10;
	float playerJumpForce = 8;
	int coins = 0;
	int walkSoundTimer = 0;
	
	void calcVertPhysics(SDL_Audio &Audio);
	void calcHorizPhysics(SDL_Audio &Audio);
	int playerCheckVertBoundries(float your_x, float your_y, float your_width, float your_height);
	int playerCheckHorizBoundries(float your_x, float your_y, float your_width, float your_height, bool mustColide);
	void collectCoin(int amount);
	void bruh(SDL_Audio &Audio);
};

enum states {
	STANDING = 0,
	JUMPING,
	FALLING
};

enum ANIM_STATES {
	ANIM_STAND = 0,
	ANIM_WALK,
	ANIM_JUMP
};