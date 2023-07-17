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
	void update(SDL_Audio &Audio, int gameFrame);
	void draw(SDL_Screen &Scene, int gameFrame);
	void jump(SDL_Audio &Audio);
	void moveHoriz(int dir, SDL_Audio &Audio);
	void stopMoving(int dir);
	void setXYPos(int x, int y);
	int getCoinCount(void);
	void shoulderBash(SDL_Audio &Audio, int gameFrame);
	void pressDown(SDL_Audio &Audio, int gameFrame);
	void releaseDown(void);
	void pressUp(SDL_Audio &Audio, int gameFrame);
private:
	float x, y, horizVect, vertVect;
	int hitboxWidth, hitboxHeight, spriteWidth, spriteHeight, horizSpriteOffset, vertSpriteOffset, animTimer, animDelay, animState, prevAnimState, animDirection, moveStartTime, moveState;
	bool isTouchingGround;
	bool releasingCrouch;
	int playerGroundState;
	int playerPressedMoveDir;
	float playerAcceleration = 0.3;
	float playerDeceleration[2];
	float playerMaxHorizSpeed = 6;
	float playerMaxVertSpeed = 10;
	float playerJumpForce = 8;
	int coins = 0;
	int soundTimer = 0;
	
	void calcVertPhysics(SDL_Audio &Audio, int gameFrame);
	void calcHorizPhysics(SDL_Audio &Audio, int gameFrame);
	int playerCheckVertBoundries(float your_x, float your_y, float your_width, float your_height);
	int playerCheckHorizBoundries(float your_x, float your_y, float your_width, float your_height, bool mustColide);
	void collectCoin(int amount);
	inline void userMoveHoriz(SDL_Audio &Audio);
	inline void calcShoulderBash(SDL_Audio &Audio, int gameFrame);
	inline void enterDoor(void);
	inline void userCrouch(SDL_Audio &Audio, int gameFrame);
	inline void userGroundPound(SDL_Audio &Audio);
};

enum GROUND_STATES {
	STANDING = 0,
	JUMPING,
	FALLING
};

enum ANIM_STATES {
	ANIM_STAND = 0,
	ANIM_WALK,
	ANIM_JUMP,
	ANIM_BASH,
	ANIM_BASH_JUMP,
	ANIM_GROUND_POUND,
	ANIM_CROUCH_STAND,
	ANIM_CROUCH_WALK
};

enum MOVE_STATES {
	NORMAL, // or standing/jumping/walking idk
	SHOULDER_BASH,
	GROUND_POUND,
	ROLL,
	BUMP,
	CROUCH
};