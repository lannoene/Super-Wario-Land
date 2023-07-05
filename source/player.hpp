#define NONE	0
#define LEFT	1
#define RIGHT	2

class Player {
public:
	Player();
	~Player();
	void update();
	void draw(SDL_Screen &Scene);
	void jump();
	void moveHoriz(int dir);
	void stopMoving(int dir);
private:
	float x, y, horizVect, vertVect;
	int hitboxWidth, hitboxHeight, horizHitboxOffset, spriteWidth, spriteHeight;
	bool isTouchingGround;
	int playerState;
	int playerPressedMoveDir;
	float playerAcceleration = 0.01;
	float playerDeceleration = 0.01;
	float playerMaxHorizSpeed = 1;
	float playerMaxVertSpeed = 2.5;
	float playerJumpForce = 1.5;
	
	void calcVertPhysics();
	void calcHorizPhysics();
	int playerCheckVertBoundries(float your_x, float your_y, float your_width, float your_height);
	int playerCheckHorizBoundries(float your_x, float your_y, float your_width, float your_height);
};

enum states {
	STANDING = 0,
	JUMPING,
	FALLING
};