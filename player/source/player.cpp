#include "draw.hpp"
#include "player.hpp"
#include "tiles.hpp"
#include "squishy_array.hpp"
#include "tools.hpp"

extern float cameraHorizOffsetPx;
extern SquishyArray <Tile*>Tile_array;

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
	this->playerState = STANDING;
	this->prevAnimState = STANDING;
	this->animState = ANIM_STAND;
	this->animTimer = 0;
	this->animDelay = 5;//frames
	this->animDirection = RIGHT;
	playerDeceleration[0] = 0.3;
	playerDeceleration[1] = 0.6;
}

Player::~Player() {
}

void Player::update(SDL_Audio &Audio, int gameFrame) {
	int ret = -1;
	for (size_t i = 0; i < Tile_array.length(); i++) {
		if (tools::checkHorizBounds(x, y, hitboxWidth, hitboxHeight, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
			if (Tile_array.data()[i]->getType() == GOLD_COIN) {
				ret = i;
				break;
			}
		}
	}
	if (ret != -1) {
		switch (Tile_array.data()[ret]->getType()) {
			default:
			break;
			case GOLD_COIN:
				delete Tile_array.data()[ret];
				Tile_array.array_splice(ret, 1);
				this->collectCoin(1);
				Audio.playSFX(SFX_COLLECT_COIN, 1);
			break;
		}
	}	
	
	
	calcVertPhysics(Audio, gameFrame);
	calcHorizPhysics(Audio);
	
	this->prevAnimState = this->animState;
}

int Player::playerCheckVertBoundries(float your_x, float your_y, float your_width, float your_height) {
	for (size_t i = 0; i < Tile_array.length(); i++) {
		if (tools::checkVertBounds(your_x, your_y, your_width, your_height, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height) && Tile_array.data()[i]->colidable == true) {
			return i;//the wall you intersected with
		}
	}
	return -1;//random number which can never naturally appear, so it's chosen as "no intersection"
}

int Player::playerCheckHorizBoundries(float your_x, float your_y, float your_width, float your_height, bool mustColide) {
	for (size_t i = 0; i < Tile_array.length(); i++) {
		if (tools::checkHorizBounds(your_x, your_y, your_width, your_height, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
			if (mustColide == true && Tile_array.data()[i]->colidable == true) {
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
		Scene.drawImage(IMAGE_HITBOX_RECTANGLE, this->x + cameraHorizOffsetPx, this->y, this->hitboxWidth, this->hitboxHeight);
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "%f", this->horizVect);
		Scene.drawText((char*)buffer, 0, 0, 30);
		snprintf(buffer, sizeof(buffer), "%f", this->vertVect);
		Scene.drawText((char*)buffer, 0, 30, 30);
		snprintf(buffer, sizeof(buffer), "%f", this->x);
		Scene.drawText((char*)buffer, 0, 60, 30);
		snprintf(buffer, sizeof(buffer), "%f", this->y);
		Scene.drawText((char*)buffer, 0, 90, 30);
		snprintf(buffer, sizeof(buffer), "%d", this->playerState);
		Scene.drawText((char*)buffer, 0, 120, 30);
	}
	
	if (this->prevAnimState != this->animState)
		this->animTimer = 0;
	
	if (gameFrame % this->animDelay == 0) 
		++this->animTimer;
	
	switch (this->animState) {
		case ANIM_STAND:
			Scene.drawImageWithDir(IMAGE_PLAYER_WARIO, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
		break;
		case ANIM_JUMP:
			Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_FALL, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
		break;
		case ANIM_WALK:
			switch (this->animTimer) {
				default:
					this->animTimer = 0;
				// fallthrough
				case 0:
					//Scene.drawImage(IMAGE_PLAYER_WARIO_WALK_1, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset, this->spriteWidth, this->spriteHeight);
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_WALK_1, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 1:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_WALK_2, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 2:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_WALK_3, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
				case 3:
					Scene.drawImageWithDir(IMAGE_PLAYER_WARIO_WALK_4, this->x + cameraHorizOffsetPx - this->horizSpriteOffset, this->y - this->vertSpriteOffset, this->spriteWidth, this->spriteHeight, this->animDirection == RIGHT ? false : true);
				break;
			}
		break;
	}
}

void Player::calcVertPhysics(SDL_Audio &Audio, int gameFrame) {
	isTouchingGround = false;
	for (size_t i = 0; i < Tile_array.length(); i++) {
		if (tools::checkVertBounds(this->x, this->y, this->hitboxWidth, this->hitboxHeight, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
			if (Tile_array.data()[i]->colidable == true) {
				if (playerState == STANDING) {
					if (!Tile_array.data()[i]->semisolid) {
						isTouchingGround = true;
					} else if (Tile_array.data()[i]->semisolid && this->y + (float)this->hitboxHeight <= Tile_array.data()[i]->y) {
						isTouchingGround = true;
					}
				}
			}
		}
	}
	
	for (size_t i = 0; i < Tile_array.length(); i++) {
		if (tools::checkVertBounds(this->x, this->y - vertVect, this->hitboxWidth, this->hitboxHeight, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
			if (Tile_array.data()[i]->colidable == true) {
				if (isTouchingGround == false && this->playerState == FALLING) {
					if (!Tile_array.data()[i]->semisolid || (Tile_array.data()[i]->semisolid && this->y + (float)this->hitboxHeight <= Tile_array.data()[i]->y)) {
						//vertVect = -(Tile_array.data()[intersetingRet]->y - this->y - hitboxHeight);//changed this from setting your vect to setting your pos so that colision bugs aren't so catastrophic
						if (vertVect <= -playerMaxVertSpeed*0.6 && horizVect == 0) {
							Audio.playSFX(SFX_LAND, 0);
						}
						this->vertVect = 0;
						this->y =  Tile_array.data()[i]->y - this->hitboxHeight;
						this->playerState = STANDING;
						this->animState = ANIM_STAND;
						this->isTouchingGround = true;
					}
				}
			}
		}
	}
	
	for (size_t i = 0; i < Tile_array.length(); i++) {
		if (tools::checkVertBounds(this->x, this->y - vertVect - 1, this->hitboxWidth, this->hitboxHeight, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
			if (Tile_array.data()[i]->colidable == true) {
				if (isTouchingGround == false && playerState == JUMPING && !Tile_array.data()[i]->semisolid) {
					//vertVect = -(Tile_array.data()[intersectingBottomForCeling]->y - this->y + Tile_array.data()[intersectingBottomForCeling]->height) - 1;
					this->vertVect = 0;
					this->y = Tile_array.data()[i]->y + Tile_array.data()[i]->height + 1;
					Tile_array.data()[i]->tileVertVect = 0;
					if (Tile_array.data()[i]->breakable) {
						if (gameFrame % 2 == 0)
							Tile_array.array_push(new PhysicsTile(Tile_array.data()[i]->x, Tile_array.data()[i]->y, GOLD_COIN));
						
						delete Tile_array.data()[i];
						Tile_array.array_splice(i, 1);
					}
				}
			}
		}
	}
	
	if (this->isTouchingGround == false && this->vertVect > -this->playerMaxVertSpeed) {
		this->vertVect -= GRAVITY;
		if (this->vertVect < 0) {
			this->playerState = FALLING;
		}
	} else if (this->isTouchingGround == true) {
		this->vertVect = 0;
	}
	
	if (!this->isTouchingGround) {
		this->animState = ANIM_JUMP;
	}
	
	this->y -= this->vertVect;
}

void Player::calcHorizPhysics(SDL_Audio &Audio) {
	if (this->playerPressedMoveDir == NONE) {
		if (this->horizVect > 0) {
			this->horizVect += -this->playerDeceleration[isTouchingGround];
		} else if (horizVect < 0) {
			this->horizVect += this->playerDeceleration[isTouchingGround];
		}
		
		if (this->isTouchingGround) {
			this->animState = ANIM_STAND;
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
		if (this->walkSoundTimer >= 40 && this->isTouchingGround == true) {
			Audio.playSFX(SFX_WALK, 0);
			this->walkSoundTimer = 0;
		}
		
		++this->walkSoundTimer;
		if (this->isTouchingGround && this->isTouchingGround)
			this->animState = ANIM_WALK;
	} else if (playerPressedMoveDir == LEFT) {
		if (horizVect > 0)
			horizVect += -playerDeceleration[isTouchingGround];
		if (horizVect > -playerMaxHorizSpeed) {
			horizVect += -playerAcceleration;
		} else {
			horizVect = -playerMaxHorizSpeed;
		}
		
		if (this->walkSoundTimer >= 40 && this->isTouchingGround == true) {
			Audio.playSFX(SFX_WALK, 0);
			this->walkSoundTimer = 0;
		}
		++this->walkSoundTimer;
		
		if (this->isTouchingGround)
			this->animState = ANIM_WALK;
	}
	
	if (playerCheckHorizBoundries(this->x + horizVect, this->y, this->hitboxWidth, this->hitboxHeight, true) != -1) {
		int intersectingTileId = playerCheckHorizBoundries(this->x + horizVect, this->y, this->hitboxWidth, this->hitboxHeight, true);
		
		if (horizVect > 0 && !Tile_array.data()[intersectingTileId]->semisolid) {
			//horizVect = Tile_array.data()[intersectingTileId]->x - this->x - this->hitboxWidth;
			horizVect = 0;
			this->x = Tile_array.data()[intersectingTileId]->x - this->hitboxWidth;
		} else if (horizVect < 0 && !Tile_array.data()[intersectingTileId]->semisolid) {
			//horizVect = Tile_array.data()[intersectingTileId]->x + Tile_array.data()[intersectingTileId]->width - this->x;
			horizVect = 0;
			this->x = Tile_array.data()[intersectingTileId]->x + Tile_array.data()[intersectingTileId]->width;
		}
	}
	
	if (this->x > 300) {
		//cameraHorizOffsetPx -= horizVect;
		cameraHorizOffsetPx = -this->x + 300;
	} else {
		cameraHorizOffsetPx = 0;
	}
	
	x += horizVect;
}

void Player::jump(SDL_Audio &Audio) {
	if (this->isTouchingGround == true) {
		vertVect = playerJumpForce;
		isTouchingGround = false;
		playerState = JUMPING;
		Audio.playSFX(SFX_JUMP, 0);
	}
}

void Player::moveHoriz(int dir, SDL_Audio &Audio) {
	if (this->playerPressedMoveDir != dir && this->playerPressedMoveDir == NONE) {
		this->walkSoundTimer = 30;
	}
	this->playerPressedMoveDir = dir;
	if (dir != NONE)
		this->animDirection = dir;
}

void Player::stopMoving(int dir) {
	if (this->playerPressedMoveDir == dir) {
		this->playerPressedMoveDir = NONE;
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

void Player::enterDoor(void) {
	int ret = -1;
	for (size_t i = 0; i < Tile_array.length(); i++) {
		if (tools::checkHorizBounds(x, y, hitboxWidth, hitboxHeight, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
			if (Tile_array.data()[i]->getType() == TILE_DOOR) {
				ret = i;
				break;
			}
		}
	}
	if (ret != -1 && Tile_array.data()[ret]->getType() == TILE_DOOR && this->isTouchingGround == true) {
		for (size_t i = 0; i < Tile_array.length(); i++) {
			if (Tile_array.data()[i]->param1.doorId == Tile_array.data()[ret]->param2.destinationDoorId) {
				this->setXYPos(Tile_array.data()[i]->x, Tile_array.data()[i]->y + -this->hitboxHeight + 100);
				this->vertVect = 0;
				this->horizVect = 0;
				//this->playerPressedMoveDir = NONE;
			}
		}
	}
}