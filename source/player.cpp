#include "draw.hpp"
#include "player.hpp"
#include "game.hpp"
#include "squishy_array.hpp"

extern float cameraHorizOffsetPx;
extern SquishyArray <Tile*>Tile_array;

Player::Player() {
	this->x = 0;
	this->y = 0;
	this->hitboxWidth = 49;
	this->hitboxHeight = 99;
	this->horizHitboxOffset = 25;
	this->spriteWidth = 100;
	this->spriteHeight = 100;
	this->horizVect = 0;
	this->vertVect = 0;
	this->isTouchingGround = false;
	this->playerState = STANDING;
}

Player::~Player() {
	
}

bool checkVertBounds(float inputChecking_x, float inputChecking_y, float input_width, float input_height, float inputCheckAgainst_x, float inputCheckAgainst_y, float destination_size_x, float destination_size_y) {
	if (inputChecking_x + input_width > inputCheckAgainst_x && inputChecking_x < inputCheckAgainst_x + destination_size_x && inputChecking_y + input_height >= inputCheckAgainst_y && inputChecking_y <= inputCheckAgainst_y + destination_size_y) {
		return true;
	} else {
		return false;
	}
}

bool checkHorizBounds(float inputChecking_x, float inputChecking_y, float input_width, float input_height, float inputCheckAgainst_x, float inputCheckAgainst_y, float destination_size_x, float destination_size_y) {
	inputChecking_y += 1;
	destination_size_y -= 1;//so that it doesn't misfire when you are bumping your head on the celing
	if (inputChecking_x + input_width > inputCheckAgainst_x && inputChecking_x < inputCheckAgainst_x + destination_size_x && inputChecking_y + input_height > inputCheckAgainst_y && inputChecking_y < inputCheckAgainst_y + destination_size_y) {
		return true;
	} else {
		return false;
	}
}

void Player::update() {
	if (playerCheckVertBoundries(this->x + this->horizHitboxOffset, this->y, this->hitboxWidth, this->hitboxHeight) != -1 && playerState == FALLING) {
		isTouchingGround = true;
	} else {
		isTouchingGround = false;
	}
	
	calcVertPhysics();
	calcHorizPhysics();
}

int Player::playerCheckVertBoundries(float your_x, float your_y, float your_width, float your_height) {
	for (int i = 0; i < Tile_array.length(); i++) {
		if (checkVertBounds(your_x, your_y, your_width, your_height, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
			return i;//the wall you intersected with
		}
	}
	return -1;//random number which can never naturally appear, so it's chosen as "no intersection"
}

int Player::playerCheckHorizBoundries(float your_x, float your_y, float your_width, float your_height) {
	for (int i = 0; i < Tile_array.length(); i++) {
		if (checkHorizBounds(your_x, your_y, your_width, your_height, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
			return i;//the wall you intersected with
		}
	}
	return -1;
}

bool showDebugInfo = false;

void Player::draw(SDL_Screen &Scene) {
	if (showDebugInfo == true) {
		Scene.drawImage(IMAGE_HITBOX_RECTANGLE, x + horizHitboxOffset + cameraHorizOffsetPx, y, hitboxWidth, hitboxHeight);
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "%f", this->horizVect);
		Scene.drawText((char*)buffer, 0, 0, 30);
		snprintf(buffer, sizeof(buffer), "%f", this->vertVect);
		Scene.drawText((char*)buffer, 0, 30, 30);
		snprintf(buffer, sizeof(buffer), "%f", this->x + this->horizHitboxOffset);
		Scene.drawText((char*)buffer, 0, 60, 30);
		snprintf(buffer, sizeof(buffer), "%f", this->y);
		Scene.drawText((char*)buffer, 0, 90, 30);
	}
	Scene.drawImage(IMAGE_PLAYER_WARIO, x + cameraHorizOffsetPx, y, spriteWidth, spriteHeight);
}

void Player::calcVertPhysics() {
	if (isTouchingGround == false && vertVect > -playerMaxVertSpeed) {
		vertVect -= GRAVITY;
		if (vertVect < 0) {
			playerState = FALLING;
		}
	} else if (isTouchingGround == true) {
		vertVect = 0;
	}
	
	int intersetingRet = playerCheckVertBoundries(this->x + this->horizHitboxOffset, this->y - vertVect, this->hitboxWidth, this->hitboxHeight);
	
	if (isTouchingGround == false && intersetingRet != -1 && playerState == FALLING) {
		vertVect = -(Tile_array.data()[intersetingRet]->y - this->y - hitboxHeight);
	} else if (isTouchingGround == false && intersetingRet != -1 && playerState == JUMPING) {
		vertVect = -(Tile_array.data()[intersetingRet]->y - this->y + Tile_array.data()[intersetingRet]->height);
	}
	
	y -= vertVect;
}

void Player::calcHorizPhysics() {
	if (this->playerPressedMoveDir == NONE) {
		if (horizVect > 0) {
			horizVect += -this->playerDeceleration;
		} else if (horizVect < 0) {
			horizVect += this->playerDeceleration;
		}
		
		//correct for any float rounding errors on the program's part
		if (horizVect < playerDeceleration && horizVect > 0) {
			horizVect = 0;
		} else if (horizVect > -playerDeceleration && horizVect < 0) {
			horizVect = 0;
		}
	}
	
	if (playerPressedMoveDir == RIGHT) {
		if (horizVect < playerMaxHorizSpeed) {
			horizVect += playerAcceleration;
		} else {
			horizVect = playerMaxHorizSpeed;
		}
	} else if (playerPressedMoveDir == LEFT) {
		if (horizVect > -playerMaxHorizSpeed) {
			horizVect += -playerAcceleration;
		} else {
			horizVect = -playerMaxHorizSpeed;
		}
	}
	
	if (playerCheckHorizBoundries(this->x + this->horizHitboxOffset + horizVect, this->y - 1, this->hitboxWidth, this->hitboxHeight) != -1) {
		int intersectingTileId = playerCheckHorizBoundries(this->x + this->horizHitboxOffset + horizVect, this->y - 1, this->hitboxWidth + this->horizHitboxOffset, this->hitboxHeight);
		
		if (horizVect > 0) {
			horizVect = Tile_array.data()[intersectingTileId]->x - this->x - this->hitboxWidth - this->horizHitboxOffset;
		} else if (horizVect < 0) {
			horizVect = Tile_array.data()[intersectingTileId]->x + Tile_array.data()[intersectingTileId]->width - this->x - this->horizHitboxOffset;
		}
	}
	
	if (this->x > 300) {
		cameraHorizOffsetPx -= horizVect;
	} else {
		cameraHorizOffsetPx = 0;
	}
	
	x += horizVect;
}

void Player::jump() {
	if (this->isTouchingGround == true) {
		vertVect = playerJumpForce;
		isTouchingGround = false;
		playerState = JUMPING;
	}
}

void Player::moveHoriz(int dir) {
	this->playerPressedMoveDir = dir;
}

void Player::stopMoving(int dir) {
	if (this->playerPressedMoveDir == dir) {
		this->playerPressedMoveDir = NONE;
	}
}