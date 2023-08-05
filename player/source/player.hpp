#pragma once

#include "draw.hpp"
#include "audio.hpp"

#define NONE	0
#define LEFT	1
#define RIGHT	2
#define UP  	3
#define DOWN	4

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
	void releaseUp(void);
private:
	float x, y, horizVect, vertVect;
	int hitboxWidth, hitboxHeight, spriteWidth, spriteHeight, horizSpriteOffset, vertSpriteOffset, animTimer, animDelay, animState, prevAnimState, animDirection, moveStartTime, moveState;
	bool isTouchingGround;
	bool releasingCrouch;
	bool jumpingOutOfWater = false;
	int playerGroundState;
	int playerPressedMoveDir;
	float playerAcceleration = 0.3;
	float playerDeceleration[2];
	float playerMaxHorizSpeed = 6;
	float playerMaxVertSpeed = 10;
	float playerJumpForce = 8;
	float maxHorizSwimSpeed = 3;
	float maxVertSwimSpeed = 3;
	float waterAcceleration = 0.3;
	float waterDeceleration = 0.3;
	int coins = 0;
	int soundTimer = 0;
	int vertCameraOffsetOffset = 250; //it's the offset of the offset!!!
	int moveDir = NONE;
	
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
	void climbLadder(int dir);
	void enterLadder(void);
	void enterWater(SDL_Audio &Audio, int gameFrame);
	void exitWater(void);
	void swim(SDL_Audio &Audio, int gameFrame);
	void changeSwimDirection(int dir);
	void stopSwimDir(int dir);
	void jumpOutOfWater(void);
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
	ANIM_CROUCH_WALK,
	ANIM_CLIMB_STILL,
	ANIM_CLIMB_MOVE,
	ANIM_SWIM_DEEP_STILL,
	ANIM_SWIM_DEEP_MOVE,
	ANIM_SWIM_TOP,
};

enum MOVE_STATES {
	NORMAL, // or standing/jumping/walking idk
	SHOULDER_BASH,
	GROUND_POUND,
	CROUCH,
	CLIMB,
	SWIM
};