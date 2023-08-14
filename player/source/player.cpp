#include <cmath>

#include "draw.hpp"
#include "player.hpp"
#include "tiles.hpp"
#include "squishy_array.hpp"
#include "tools.hpp"
#include "room.hpp"

extern float cameraHorizOffsetPx;
extern int cameraVertOffsetPx;
extern SquishyArray <Room*>Room_array; // todo: remove this extern and change a bunch of functions to take room_array as an input

Player::Player() {
	this->x = 0;
	this->y = 0;
	this->hitboxWidth = 45;
	this->hitboxHeight = 95;
	this->horizSpriteOffset = 25;
	this->vertSpriteOffset = 5;
	this->spriteWidth = 100;
	this->spriteHeight = 100;
	this->horizVect = 0;
	this->vertVect = 0;
	this->isTouchingGround = false;
	this->playerGroundState = STANDING;
	this->prevAnimState = ANIM_STAND;
	this->animState = ANIM_STAND;
	this->animTimer = 0;
	this->animDelay = 5;//frames
	this->animDirection = RIGHT;
	this->moveState = NORMAL;
	playerDeceleration[false] = 0.3;
	playerDeceleration[true] = 0.6;
	this->releasingCrouch = false;
}

Player::~Player() {
}

int currentRoom = 0;

void Player::update(SDL_Audio &Audio, int gameFrame) {
	for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) {
		if (tools::checkHorizBounds(x, y, hitboxWidth, hitboxHeight, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
			if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->getType() == GOLD_COIN) {
				switch (Room_array.data()[currentRoom]->Room_tiles.data()[i]->getType()) {
					default:
					break;
					case GOLD_COIN:
						delete Room_array.data()[currentRoom]->Room_tiles.data()[i];
						Room_array.data()[currentRoom]->Room_tiles.array_splice(i, 1);
						this->collectCoin(1);
						Audio.playSFX(SFX_COLLECT_COIN, 1);
					break;
				}
			}
		}
	}
	
	enterWater(Audio, gameFrame);
	calcVertPhysics(Audio, gameFrame);
	calcHorizPhysics(Audio, gameFrame);
	
	this->prevAnimState = this->animState;
}

int Player::playerCheckVertBoundries(float your_x, float your_y, float your_width, float your_height) {
	for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) {
		if (tools::checkVertBounds(your_x, your_y, your_width, your_height, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height) && Room_array.data()[currentRoom]->Room_tiles.data()[i]->colidable == true) {
			return i;//the wall you intersected with
		}
	}
	return -1;//random number which can never naturally appear, so it's chosen as "no intersection"
}

int Player::playerCheckHorizBoundries(float your_x, float your_y, float your_width, float your_height, bool mustColide) {
	for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) {
		if (tools::checkHorizBounds(your_x, your_y, your_width, your_height, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
			if (mustColide == true && Room_array.data()[currentRoom]->Room_tiles.data()[i]->colidable == true) {
				return i;//the wall you intersected with
			} else if (mustColide == false) {
				return i;
			}
		}
	}
	return -1;
}

bool showDebugInfo = false;

void Player::draw(SDL_Screen &Scene, int gameFrame) {
	if (showDebugInfo == true) {
		Scene.drawRectangle(this->x + cameraHorizOffsetPx, this->y + cameraVertOffsetPx, this->hitboxWidth, this->hitboxHeight, CLR_BLU);
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "%f", this->horizVect);
		Scene.drawText((char*)buffer, 0, 30, 30);
		snprintf(buffer, sizeof(buffer), "%f", this->vertVect);
		Scene.drawText((char*)buffer, 0, 60, 30);
		snprintf(buffer, sizeof(buffer), "%f", this->x);
		Scene.drawText((char*)buffer, 0, 90, 30);
		snprintf(buffer, sizeof(buffer), "%f", this->y);
		Scene.drawText((char*)buffer, 0, 120, 30);
		snprintf(buffer, sizeof(buffer), "%d", this->playerGroundState);
		Scene.drawText((char*)buffer, 0, 150, 30);
		snprintf(buffer, sizeof(buffer), "%d", this->animState);
		Scene.drawText((char*)buffer, 0, 180, 30);
		snprintf(buffer, sizeof(buffer), "%d", this->moveState);
		Scene.drawText((char*)buffer, 0, 210, 30);
	}
	
	if (this->prevAnimState != this->animState)
		this->animTimer = 0;
	
	if (gameFrame % this->animDelay == 0) 
		++this->animTimer;
	
	switch (this->animState) {
		case ANIM_STAND:
			Scene.drawImageWithDir(IMAGE_PLAYER_WARIO, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
		break;
		case ANIM_JUMP:
			Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_FALL, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
		break;
		case ANIM_WALK:
		this->animDelay = 5;
			switch (this->animTimer) {
				default:
					this->animTimer = 0;
				// fallthrough
				case 0:
					//Scene.drawImage(IMAGE_PLAYER_WARIO_WALK_1, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight);
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_WALK_1, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 1:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_WALK_2, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 2:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_WALK_3, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 3:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_WALK_4, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
			}
		break;
		case ANIM_BASH:
			this->animDelay = 2;
			switch (this->animTimer) {
				default:
					this->animTimer = 0;
					// fallthrough
				case 0:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_1, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 1:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_2, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 2:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_3, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 3:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_4, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 4:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_1, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 5:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_2, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 6:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_3, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 7:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_4, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 8:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_1, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 9:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_2, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 10:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_3, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 11:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_4, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 12:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_1, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 13:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_2, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 14:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_3, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 15:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_4, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 16:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_5, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 17:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_6, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 18:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_7, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 19:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_8, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
			}
		break;
		case ANIM_CROUCH_WALK:
			this->animDelay = 10;
			switch (this->animTimer) {
				default:
					this->animTimer = 0;
					// fallthrough
				case 0:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_CROUCH_1, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 1:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_CROUCH_2, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 2:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_CROUCH_1, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 3:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_CROUCH_3, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
			}
		break;
		case ANIM_CLIMB_MOVE:
			this->animDelay = 3;
			switch (this->animTimer) {
				default:
					this->animTimer = 0;
					// fallthrough
				case 0:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_CLIMB_5, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, false);
				break;
				case 1:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_CLIMB_6, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, false);
				break;
				case 2:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_CLIMB_5, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, false);
				break;
				case 3:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_CLIMB_4, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, false);
				break;
				case 4:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_CLIMB_3, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, false);
				break;
				case 5:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_CLIMB_2, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, false);
				break;
				case 6:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_CLIMB_1, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, false);
				break;
				case 7:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_CLIMB_2, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, false);
				break;
				case 8:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_CLIMB_3, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, false);
				break;
				case 9:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_CLIMB_4, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, false);
				break;
			}
		break;
		case ANIM_SWIM_DEEP_MOVE:
			this->animDelay = 5;
			switch (this->animTimer) {
				default:
					this->animTimer = 0;
					// fallthrough
				case 0:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_SWIM_1, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 1:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_SWIM_2, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 2:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_SWIM_3, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 3:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_SWIM_4, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 4:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_SWIM_5, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
			}
		break;
		case ANIM_CLIMB_STILL:
			Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_CLIMB_4, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, false);
		break;
		case ANIM_BASH_JUMP:
			Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_BASH_JUMP, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
		break;
		case ANIM_GROUND_POUND:
			Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_GROUND_POUND1, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
		break;
		case ANIM_CROUCH_STAND:
			Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_CROUCH_1, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
		break;
		case ANIM_SWIM_DEEP_STILL:
			Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_SWIM_4, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset + cameraVertOffsetPx, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
		break;
	}
}

void Player::userGroundPound(SDL_Audio &Audio) {
	if (!isTouchingGround) {
		this->vertVect = -10;
		this->playerGroundState = FALLING;
	} else {
		this->vertVect = 0;
		this->moveState = NORMAL;
		this->animState = ANIM_STAND;
	}
}

void Player::calcVertPhysics(SDL_Audio &Audio, int gameFrame) {
	switch (this->moveState) {
		default:
			applyDefaultGravity();
		break;
		case GROUND_POUND:
			userGroundPound(Audio);
		break;
		case CLIMB:
			if (this->moveDir == UP) {
				this->vertVect = 3;
				if (this->animState == ANIM_CLIMB_MOVE) {
					if (this->soundTimer >= 15) {
						Audio.playSFX(SFX_CLIMB, 0);
						this->soundTimer = 0;
					} else
						++this->soundTimer;
				}
				this->playerGroundState = JUMPING;
			} else if (this->moveDir == DOWN) {
				this->vertVect = -3;
				this->playerGroundState = FALLING;
				if (this->animState == ANIM_CLIMB_MOVE) {
					if (this->soundTimer >= 15) {
						Audio.playSFX(SFX_CLIMB, 0);
						this->soundTimer = 0;
					} else
						++this->soundTimer;
					this->playerGroundState = FALLING;
				}
			} else if (this->moveDir == NONE) {
				this->vertVect = 0;
				this->playerGroundState = STANDING;
			}
			this->moveState = NORMAL;
			for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) {
				if (tools::checkVertBounds(this->x, this->y - this->vertVect, this->hitboxWidth, this->hitboxHeight, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
					if ((Room_array.data()[currentRoom]->Room_tiles.data()[i]->colidable && this->moveDir != UP && !Room_array.data()[currentRoom]->Room_tiles.data()[i]->semisolid) || (Room_array.data()[currentRoom]->Room_tiles.data()[i]->semisolid && this->y + (float)this->hitboxHeight <= Room_array.data()[currentRoom]->Room_tiles.data()[i]->y) ) {
						this->moveState = NORMAL;
						this->vertVect = 0;
						this->y = Room_array.data()[currentRoom]->Room_tiles.data()[i]->y - this->hitboxHeight;
						break;
					} else if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->ladder) {
						this->moveState = CLIMB;
						break;
					}
				}
			}
			for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) {
				if (tools::checkVertBounds(this->x, this->y - this->vertVect, this->hitboxWidth, this->hitboxHeight, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
					if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->colidable && !Room_array.data()[currentRoom]->Room_tiles.data()[i]->semisolid) {
						this->vertVect = 0;
						this->animState = ANIM_CLIMB_STILL;
						this->y = Room_array.data()[currentRoom]->Room_tiles.data()[i]->y + Room_array.data()[currentRoom]->Room_tiles.data()[i]->height;
						break;
					}
				}
			}
		break;
		case SWIM:
			swim(Audio, gameFrame); // vert gets run before horiz so i put swim func here as a placeholder instead of in horiz cuz i need to calc it here first!!!! it wouldn't matter tho if it used standard vert gravity, but sometimes i do, sometimes i don't!
		break;
		case CROUCH:
			applyDefaultGravity();
			if (releasingCrouch)
				releaseDown();
		break;
	}
	
	isTouchingGround = false;
	for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) { // is player touching ground
		if (tools::checkVertBounds(this->x, this->y, this->hitboxWidth, this->hitboxHeight, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
			if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->colidable == true) {
				if (playerGroundState == STANDING) {
					if (!Room_array.data()[currentRoom]->Room_tiles.data()[i]->semisolid || (Room_array.data()[currentRoom]->Room_tiles.data()[i]->semisolid && this->y + (float)this->hitboxHeight <= Room_array.data()[currentRoom]->Room_tiles.data()[i]->y)) {
						isTouchingGround = true;
					}
				}
			}
		}
	}
	
	for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) { // is player going to touch ground
		if (tools::checkVertBounds(this->x, this->y - vertVect, this->hitboxWidth, this->hitboxHeight, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
			if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->colidable == true) {
				if (isTouchingGround == false && this->playerGroundState == FALLING) {
					if (!Room_array.data()[currentRoom]->Room_tiles.data()[i]->semisolid || (Room_array.data()[currentRoom]->Room_tiles.data()[i]->semisolid && this->y + (float)this->hitboxHeight <= Room_array.data()[currentRoom]->Room_tiles.data()[i]->y)) {
						if (vertVect <= -playerMaxVertSpeed*0.6 && horizVect == 0 && this->moveState == NORMAL) {
							Audio.playSFX(SFX_LAND, 0);
						}
						this->vertVect = 0;
						this->y = Room_array.data()[currentRoom]->Room_tiles.data()[i]->y - this->hitboxHeight;
						this->playerGroundState = STANDING;
						if (this->moveState == NORMAL) {
							this->animState = ANIM_STAND;
						} else if (this->moveState == SHOULDER_BASH) {
							this->animState = ANIM_BASH;
						}
						
						if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->breakable && this->moveState == GROUND_POUND && gameFrame - this->moveStartTime > 2) {
							if (rand() % 2 == 0)
								Room_array.data()[currentRoom]->Room_tiles.array_push(new PhysicsTile(Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, GOLD_COIN));
							delete Room_array.data()[currentRoom]->Room_tiles.data()[i];
							Room_array.data()[currentRoom]->Room_tiles.array_splice(i, 1);
							Audio.playSFX(SFX_BREAK, 0);
						}
						this->isTouchingGround = true;
					}
				}
			}
		}
	}
	
	for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) { // is player going to touch celing
		if (tools::checkVertBounds(this->x, this->y - vertVect, this->hitboxWidth, this->hitboxHeight, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
			if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->colidable == true) {
				if (isTouchingGround == false && playerGroundState == JUMPING && !Room_array.data()[currentRoom]->Room_tiles.data()[i]->semisolid) {
					this->y = Room_array.data()[currentRoom]->Room_tiles.data()[i]->y + Room_array.data()[currentRoom]->Room_tiles.data()[i]->height;
					Room_array.data()[currentRoom]->Room_tiles.data()[i]->tileVertVect = 0;
					this->vertVect = 0;
					if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->breakable) {
						if (rand() % 2 == 0)
							Room_array.data()[currentRoom]->Room_tiles.array_push(new PhysicsTile(Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, GOLD_COIN));
						delete Room_array.data()[currentRoom]->Room_tiles.data()[i];
						Room_array.data()[currentRoom]->Room_tiles.array_splice(i, 1);
						Audio.playSFX(SFX_BREAK, 0);
					}
				}
			}
		}
	}
	
	this->y -= this->vertVect;
	
	
	updateCameraVertOffset();
}

void Player::calcHorizPhysics(SDL_Audio &Audio, int gameFrame) {
	switch (this->moveState) {
		default:
			userMoveHoriz(Audio);
		break;
		case NORMAL:
			userMoveHoriz(Audio);
		break;
		case SHOULDER_BASH:
			calcShoulderBash(Audio, gameFrame);
		break;
		case GROUND_POUND:
			this->horizVect = 0;
		break;
		case CLIMB:
			this->horizVect = 0;
		break;
		case SWIM:
		break;
	}
	
	bool playerBumped = false;
	
	for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) {
		if (tools::checkHorizBounds(this->x + horizVect, this->y, this->hitboxWidth, this->hitboxHeight, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
			if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->colidable && !Room_array.data()[currentRoom]->Room_tiles.data()[i]->semisolid) {
				if (this->horizVect > 0)
					this->x = Room_array.data()[currentRoom]->Room_tiles.data()[i]->x - this->hitboxWidth;
				else if (this->horizVect < 0)
					this->x = Room_array.data()[currentRoom]->Room_tiles.data()[i]->x + Room_array.data()[currentRoom]->Room_tiles.data()[i]->width;
				if (this->moveState != SHOULDER_BASH) {
					horizVect = 0;
				} else if (this->moveState == SHOULDER_BASH) {
					playerBumped = true;
					if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->breakable) {
						if (rand() % 2 == 0)
							Room_array.data()[currentRoom]->Room_tiles.array_push(new PhysicsTile(Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, GOLD_COIN));
						delete Room_array.data()[currentRoom]->Room_tiles.data()[i];
						Room_array.data()[currentRoom]->Room_tiles.array_splice(i, 1);
						Audio.playSFX(SFX_BREAK, 1);
						i--; // TODO: create a broken flag so i can stop messing with the tile array (which causes tiles to be skipped sometimes)
					}
				}
			}
		}
	}
	
	if (playerBumped) {
		if (isTouchingGround) {
			vertVect = 3;
			this->isTouchingGround = false;
		} else
			vertVect = -1;
		this->horizVect = -horizVect / abs(horizVect) * 5;
		this->moveState = NORMAL;
		this->playerGroundState = JUMPING;
		Audio.playSFX(SFX_BUMP, 0);
	}
	
	x += horizVect;
	
	updateCameraHorizOffset();
}

void Player::userMoveHoriz(SDL_Audio &Audio) {
	if (this->playerPressedMoveDir == NONE) {
		if (this->horizVect > 0) {
			this->horizVect += -this->playerDeceleration[isTouchingGround];
		} else if (horizVect < 0) {
			this->horizVect += this->playerDeceleration[isTouchingGround];
		}
		
		if (this->isTouchingGround && this->moveState == NORMAL) {
			this->animState = ANIM_STAND;
		} else if (this->moveState == CROUCH) {
			this->animState = ANIM_CROUCH_STAND;
		}
		
		//correct for any float rounding errors on the program's part
		if (this->horizVect < this->playerDeceleration[isTouchingGround] && this->horizVect > 0) {
			this->horizVect = 0;
		} else if (this->horizVect > -this->playerDeceleration[isTouchingGround] && this->horizVect < 0) {
			this->horizVect = 0;
		}
	}
	
	if (playerPressedMoveDir == RIGHT) {
		if (horizVect < 0 && this->isTouchingGround)
			horizVect += playerDeceleration[isTouchingGround];
		if (horizVect < playerMaxHorizSpeed) {
			horizVect += playerAcceleration;
		} else {
			horizVect = playerMaxHorizSpeed;
		}
		if (this->soundTimer >= 40 && this->isTouchingGround == true && this->moveState == NORMAL) {
			Audio.playSFX(SFX_WALK, 0);
			this->soundTimer = 0;
		} else if (this->soundTimer >= 35 && this->isTouchingGround == true && this->moveState == CROUCH) {
			Audio.playSFX(SFX_CROUCH_WALK, 0);
			this->soundTimer = 0;
		}
		
		++this->soundTimer;
		if (this->isTouchingGround && this->moveState == NORMAL)
			this->animState = ANIM_WALK;
		else if (this->isTouchingGround && this->moveState == CROUCH)
			this->animState = ANIM_CROUCH_WALK;
	} else if (playerPressedMoveDir == LEFT) {
		if (horizVect > 0)
			horizVect += -playerDeceleration[isTouchingGround];
		if (horizVect > -playerMaxHorizSpeed) {
			horizVect += -playerAcceleration;
		} else {
			horizVect = -playerMaxHorizSpeed;
		}
		
		if (this->soundTimer >= 40 && this->isTouchingGround == true && this->moveState == NORMAL) {
			Audio.playSFX(SFX_WALK, 0);
			this->soundTimer = 0;
		} else if (this->soundTimer >= 35 && this->isTouchingGround == true && this->moveState == CROUCH) {
			Audio.playSFX(SFX_CROUCH_WALK, 0);
			this->soundTimer = 0;
		}
		
		++this->soundTimer;
		
		if (this->isTouchingGround && this->moveState == NORMAL)
			this->animState = ANIM_WALK;
		else if (this->isTouchingGround && this->moveState == CROUCH)
			this->animState = ANIM_CROUCH_WALK;
	}
}

void Player::calcShoulderBash(SDL_Audio &Audio, int gameFrame) {
	if (this->soundTimer >= 8) {
		Audio.playSFX(SFX_DASH, 0);
		this->soundTimer = 0;
	}
	++this->soundTimer;
	
	if (this->animDirection == RIGHT) {
		if (this->horizVect < 7)
			this->horizVect += this->playerAcceleration;
		else
			this->horizVect = 7;
	} else if (this->animDirection == LEFT) {
		if (this->horizVect > -7)
			this->horizVect += -this->playerAcceleration;
		else
			this->horizVect = -7;
	}
	
	if (gameFrame - this->moveStartTime > 50 && this->isTouchingGround) { //shoulderbash duration
		this->moveState = NORMAL;
		this->horizVect = 0;
	}
}

void Player::jump(SDL_Audio &Audio) {
	if (this->isTouchingGround == true && this->moveState != SWIM) {
		vertVect = playerJumpForce;
		isTouchingGround = false;
		playerGroundState = JUMPING;
		Audio.playSFX(SFX_JUMP, 0);
	} else if (this->moveState == CLIMB) {
		vertVect = playerJumpForce;
		isTouchingGround = false;
		playerGroundState = JUMPING;
		Audio.playSFX(SFX_JUMP, 0);
		this->moveState = NORMAL;
	} else if (this->moveState == SWIM) {
		jumpOutOfWater();
	}
}

void Player::moveHoriz(int dir, SDL_Audio &Audio) {
	if (this->playerPressedMoveDir != dir && this->playerPressedMoveDir == NONE) {
		this->soundTimer = 30;
	}
	
	if (dir != this->playerPressedMoveDir && dir != this->animDirection && this->moveState == SHOULDER_BASH) {
		this->moveState = NORMAL;
		this->horizVect = horizVect*0.5;// otherwise you would keep your momentum after canceling a bash and i found that dum. your canceled horizvect at max charge would be 3.5
	} else if (this->moveState == SWIM) {
		changeSwimDirection(dir);
	}
	
	this->playerPressedMoveDir = dir;
	if (dir != NONE)
		this->animDirection = dir;
}

void Player::stopMoving(int dir) {
	if (this->playerPressedMoveDir == dir && this->moveState != SWIM) {
		this->playerPressedMoveDir = NONE;
	} else if (this->moveState == SWIM) {
		stopSwimDir(dir);
		playerPressedMoveDir = NONE;
	}
}

void Player::setXYPos(int input_x, int input_y) {
	this->x = input_x;
	this->y = input_y;
}

void Player::collectCoin(int amount) {
	++this->coins;
}

int Player::getCoinCount(void) {
	return this->coins;
}

void Player::enterDoor(SDL_Audio &Audio) {
	int ret = -1;
	for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) {
		if (tools::checkHorizBounds(x, y, hitboxWidth, hitboxHeight, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
			if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->getType() == TILE_DOOR) {
				ret = i;
				break;
			}
		}
	}
	if (ret != -1 && Room_array.data()[currentRoom]->Room_tiles.data()[ret]->getType() == TILE_DOOR && this->isTouchingGround == true) {
		for (size_t j = 0; j < Room_array.length(); j++) {
			for (size_t i = 0; i < Room_array.data()[j]->Room_tiles.length(); i++) {
				if (Room_array.data()[j]->Room_tiles.data()[i]->param1.warpId == Room_array.data()[currentRoom]->Room_tiles.data()[ret]->param2.destinationWarpId && this->moveState != SHOULDER_BASH && Room_array.data()[j]->Room_tiles.data()[i]->flags.warp && Room_array.data()[j]->Room_tiles.data()[i]->roomId == Room_array.data()[currentRoom]->Room_tiles.data()[ret]->param3.destinationRoomId) {
					double musicPos = Audio.getMusicPos();
					if (Room_array.data()[j]->Room_tiles.data()[i]->roomId != Room_array.data()[currentRoom]->Room_tiles.data()[ret]->roomId) {
						Room_array.data()[Room_array.data()[j]->Room_tiles.data()[i]->roomId]->roomPlayBackgroundMusic(Audio);
					}
					double musicDuration = Audio.getCurrentMusicDuration();
					Audio.setMusicPos(fmod(musicPos, musicDuration)); // hopefully the music pos won't go out of bounds
					this->setCurrentRoomId(Room_array.data()[currentRoom]->Room_tiles.data()[ret]->param3.destinationRoomId);
					this->setXYPos(Room_array.data()[j]->Room_tiles.data()[i]->x + (Room_array.data()[j]->Room_tiles.data()[i]->width - this->hitboxWidth)/2, Room_array.data()[j]->Room_tiles.data()[i]->y + -this->hitboxHeight + 100);
					this->vertVect = 0;
					this->horizVect = 0;
					// fixes for camera flicker
					updateCameraVertOffset();
					updateCameraHorizOffset();
					return;
				}
			}
		}
	}
}

void Player::shoulderBash(SDL_Audio &Audio, int gameFrame) {
	if (this->isTouchingGround && this->moveState == NORMAL) {
		this->moveStartTime = gameFrame;
		this->moveState = SHOULDER_BASH;
		this->animState = ANIM_BASH;
	}
}

void Player::pressDown(SDL_Audio &Audio, int gameFrame) {
	if (!isTouchingGround && this->moveState == NORMAL) {
		this->moveStartTime = gameFrame;
		this->moveState = GROUND_POUND;
		this->animState = ANIM_GROUND_POUND;
	} else if (isTouchingGround && (this->moveState == NORMAL || this->moveState == SHOULDER_BASH)) {
		this->moveStartTime = gameFrame;
		this->moveState = CROUCH;
		this->hitboxHeight = 45;
		this->y = this->y + 50;
		this->playerMaxHorizSpeed = 3;
		this->vertSpriteOffset = 55;
		this->vertCameraOffsetOffset = 300;
		if (horizVect == 0)
			this->animState = ANIM_CROUCH_STAND;
		else
			this->animState = ANIM_CROUCH_WALK;
	} else if (this->moveState == CLIMB) {
		climbLadder(DOWN);
	} else if (this->moveState == SWIM) {
		changeSwimDirection(DOWN);
	}
	
	if (isTouchingGround) {
		this->releasingCrouch = false;
	}
}

void Player::releaseDown(void) {
	if (this->moveState == CROUCH) {
		bool canColide = false;
		releasingCrouch = true;
		for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) { // to protect against releasing down in a tight coridor
			if (tools::checkVertBounds(this->x, this->y - 50, this->hitboxWidth, this->hitboxHeight, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
				if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->colidable && !Room_array.data()[currentRoom]->Room_tiles.data()[i]->semisolid) {
					canColide = true;
					break;
				}
			}
		}
		
		if (!canColide) {
			this->y -= 50;
			this->moveState = NORMAL;
			this->hitboxHeight = 95;
			this->playerMaxHorizSpeed = 6;
			this->vertSpriteOffset = 5;
			this->releasingCrouch = false;
			this->vertCameraOffsetOffset = 250;
		}
	} else if (this->moveState == GROUND_POUND) {
		this->moveState = NORMAL;
		//anim state for normal is constantly updated so i don't need to set the anim state in this case
	} else if (this->moveState == CLIMB) {
		climbLadder(NONE); // i did this so i could set the anim state???????? wtfff there's a way better way to do this! probably..... XD (setting the move dir and handling the anim state seperatly depending on the direction!!)
	} else if (this->moveState == SWIM)
		stopSwimDir(DOWN);
}


void Player::pressUp(SDL_Audio &Audio, int gameFrame) {
	enterDoor(Audio);
	enterLadder();// couldn't think of a better name for this :P
	changeSwimDirection(UP);
}

void Player::releaseUp(void) {
	climbLadder(NONE);
	stopSwimDir(UP);
} 

void Player::userCrouch(SDL_Audio &Audio, int gameFrame) {
	
}

void Player::climbLadder(int dir) {
	if (dir == NONE && this->moveState != CLIMB)
		return;
	if (this->moveState == CLIMB)
		this->moveState = NORMAL;
	for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) {
		if (tools::checkHorizBounds(this->x, this->y, this->hitboxWidth, this->hitboxHeight, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
			if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->ladder) {
				this->moveDir = dir;
				this->moveState = CLIMB;
				if (dir != NONE)
					this->animState = ANIM_CLIMB_MOVE;
				else
					this->animState = ANIM_CLIMB_STILL;
				break;
			}
		}
	}
}

void Player::enterLadder(void) { // is only run once to check if the player should enter state when user presses button
	if (this->moveState != NORMAL && this->moveState != CLIMB)
		return;
	else if (this->moveState == CLIMB)
		climbLadder(UP);
	for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) {
		if (tools::checkVertBounds(this->x, this->y, this->hitboxWidth, this->hitboxHeight, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
			if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->ladder) {
				this->moveState = CLIMB;
				this->x = Room_array.data()[currentRoom]->Room_tiles.data()[i]->x + (Room_array.data()[currentRoom]->Room_tiles.data()[i]->width - this->hitboxWidth)/2;
				this->animState = ANIM_CLIMB_STILL;
				this->soundTimer = 0;
				climbLadder(UP);
				break;
			}
		}
	}
}

void Player::enterWater(SDL_Audio &Audio, int gameFrame) {
	for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) {
		if (tools::checkHorizBounds(this->x, this->y, this->hitboxWidth, this->hitboxHeight, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
			if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->flags.water) {
				if (this->moveState == SWIM) {
					
					return;
				} else {
					this->playerMaxHorizSpeed = 6; // default
					this->jumpingOutOfWater = false;
					if (this->moveState != CROUCH)
						this->y += 50;
					else {
						releaseDown();
					}
					this->moveState = SWIM;
					this->hitboxHeight = 45;
					this->vertSpriteOffset = 55;
					this->moveDir = this->playerPressedMoveDir;
					this->animState = ANIM_SWIM_DEEP_STILL;
					this->soundTimer = 0;
					Audio.playSFX(SFX_LAND_IN_WATER, 0);
					return;
				}
			}
		}
	}
	
	if (this->moveState == SWIM)
		exitWater();
}

void Player::exitWater(void) {
	changeSwimDirection(NONE);
	this->moveState = NORMAL;
	this->hitboxHeight = 95;
	this->vertSpriteOffset = 5;
	this->y -= 50;
}

void Player::swim(SDL_Audio &Audio, int gameFrame) {
	if (this->moveState != SWIM)
		return;
	
	bool intersectedWater = false;
	bool floatingUp = false;
	
	for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) {
		if (tools::checkHorizBounds(this->x, this->y, this->hitboxWidth, this->hitboxHeight - 25, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
			if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->flags.water) {
				if (this->moveState == SWIM) {
					
					intersectedWater = true;
					break;
				}
			}
		}
	}
	
	
	for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) {
		if (tools::checkHorizBounds(this->x, this->y, this->hitboxWidth, this->hitboxHeight, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
			if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->flags.water && !jumpingOutOfWater) {
				switch (this->moveDir) {
					case UP:
						if (intersectedWater) {
							if (this->vertVect < this->maxVertSwimSpeed)
								this->vertVect += this->waterAcceleration;
							else
								this->vertVect = maxVertSwimSpeed;
						} else {
							this->vertVect = 0;
						}
					break;
					case DOWN:
						if (this->vertVect > -this->maxVertSwimSpeed)
							this->vertVect += -this->waterAcceleration;
						else
							this->vertVect = -maxVertSwimSpeed;
					break;
					case LEFT:
						if (this->horizVect > -this->maxHorizSwimSpeed)
							this->horizVect += -this->waterAcceleration;
						else
							this->horizVect = -maxHorizSwimSpeed;
					break;
					case RIGHT:
						if (this->horizVect < this->maxHorizSwimSpeed)
							this->horizVect += this->waterAcceleration;
						else
							this->horizVect = maxHorizSwimSpeed;
					break;
					case NONE:
						if (!intersectedWater && this->vertVect > 0)
							this->vertVect = 0;
						else if (intersectedWater && this->vertVect >= 0 && this->vertVect <= 0.5) { // buoyancy
							if (this->vertVect < 0.5)
								this->vertVect += this->waterAcceleration;
							else
								this->vertVect = 0.5;
							floatingUp = true;
						}
					break;
				}
				break;
			}
		}
	}
	
	if (this->moveDir != NONE && floatingUp) {
		this->vertVect = 0;
		floatingUp = false;
	}
	
	if (this->vertVect != 0) {
		if (this->vertVect > 0 && this->moveDir != UP && !floatingUp) {
			this->vertVect += -waterDeceleration;
		} else if (this->vertVect < 0 && this->moveDir != DOWN) {
			this->vertVect += waterDeceleration;
		}
	}
	
	if (this->horizVect != 0) {
		if (this->horizVect < waterDeceleration && this->horizVect > -waterDeceleration)
			this->horizVect = 0;
		if (this->horizVect > 0 && this->moveDir != RIGHT) {
			this->horizVect += -waterDeceleration;
		} else if (this->horizVect < 0 && this->moveDir != LEFT) {
			this->horizVect += waterDeceleration;
		}
	}
	
	if (this->vertVect != 0) {
		if (this->vertVect < waterDeceleration && this->vertVect > -waterDeceleration) {
			this->vertVect = 0;
		}
	}
	
	if (this->horizVect != 0) {
		this->animState = ANIM_SWIM_DEEP_MOVE;
		if (this->soundTimer >= 30) {
			Audio.playSFX(SFX_SLOW_SWIM, 0);
			this->soundTimer = 0;
		}
		++this->soundTimer;
		
	} else {
		this->animState = ANIM_SWIM_DEEP_STILL;
	}
	
	if (this->vertVect > 0) {
		this->playerGroundState = JUMPING;
	} else if (this->vertVect < 0) {
		this->playerGroundState = FALLING;
	} else {
		this->playerGroundState = STANDING;
	}
}

void Player::changeSwimDirection(int dir) {
	if (this->moveState != SWIM)
		return;
	
	this->moveDir = dir;
}

void Player::stopSwimDir(int dir) {
	if (this->moveState != SWIM)
		return;
	
	if (this->moveDir == dir) {
		this->moveDir = NONE;
	}
}

void Player::jumpOutOfWater(void) {
	if (this->moveState != SWIM || this->vertVect < 0)
		return;
	const int jumpOutOfWaterForce = 8;
	
	for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) {
		if (tools::checkHorizBounds(this->x, this->y - 28, this->hitboxWidth, this->hitboxHeight, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
			if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->flags.water) {
				return;
			}
		}
	}
	
	for (size_t i = 0; i < Room_array.data()[currentRoom]->Room_tiles.length(); i++) {
		if (tools::checkVertBounds(this->x, this->y - 50, this->hitboxWidth, this->hitboxHeight, Room_array.data()[currentRoom]->Room_tiles.data()[i]->x, Room_array.data()[currentRoom]->Room_tiles.data()[i]->y, Room_array.data()[currentRoom]->Room_tiles.data()[i]->width, Room_array.data()[currentRoom]->Room_tiles.data()[i]->height)) {
			if (Room_array.data()[currentRoom]->Room_tiles.data()[i]->colidable && !Room_array.data()[currentRoom]->Room_tiles.data()[i]->semisolid) {
				return;
			}
		}
	}
	
	
	this->jumpingOutOfWater = true;
	this->vertVect = jumpOutOfWaterForce;
}

void Player::setCurrentRoomId(int roomId) {
	this->currentRoom = roomId;
}

int Player::getCurrentRoomId(void) {
	return this->currentRoom;
}

void Player::updateCameraVertOffset(void) {
	if (this->y > this->vertCameraOffsetOffset && this->y < Room_array.data()[this->currentRoom]->roomHeight() - WINDOW_HEIGHT + this->vertCameraOffsetOffset) {
		cameraVertOffsetPx = -this->y + this->vertCameraOffsetOffset;
	} else if (this->y <= this->vertCameraOffsetOffset) {
		cameraVertOffsetPx = 0;
	} else {
		cameraVertOffsetPx = -Room_array.data()[this->currentRoom]->roomHeight() + WINDOW_HEIGHT;
	}
}

void Player::updateCameraHorizOffset(void) {
	if (this->x > 300 && this->x < Room_array.data()[this->currentRoom]->roomWidth() - WINDOW_WIDTH + 300) {
		cameraHorizOffsetPx = -this->x + 300;
	} else if (this->x <= 300) {
		cameraHorizOffsetPx = 0;
	} else {
		cameraHorizOffsetPx = -Room_array.data()[this->currentRoom]->roomWidth() + WINDOW_WIDTH;
	}
}

inline void Player::applyDefaultGravity(void) {
	if (this->isTouchingGround == false && this->vertVect > -this->playerMaxVertSpeed) {
		this->vertVect -= GRAVITY;
		if (this->vertVect < 0) {
			this->playerGroundState = FALLING;
		} else if (this->vertVect > 0) {
			this->playerGroundState = JUMPING;
		}
	} else if (this->isTouchingGround == true) {
		this->vertVect = 0;
	}
	
	if (!this->isTouchingGround) {
		if (this->moveState == NORMAL) {
			this->animState = ANIM_JUMP;
		} else if (this->moveState == SHOULDER_BASH) {
			this->animState = ANIM_BASH_JUMP;
		} else if (this->moveState == CROUCH) {
			this->animState = ANIM_CROUCH_STAND;
		}
	}
}