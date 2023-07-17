#include "draw.hpp"
#include "player.hpp"
#include "tiles.hpp"
#include "squishy_array.hpp"
#include "tools.hpp"

extern float cameraHorizOffsetPx;
extern int cameraVertOffsetPx;
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

void Player::update(SDL_Audio &Audio, int gameFrame) {
	for (size_t i = 0; i < Tile_array.length(); i++) {
		if (tools::checkHorizBounds(x, y, hitboxWidth, hitboxHeight, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
			if (Tile_array.data()[i]->getType() == GOLD_COIN) {
				switch (Tile_array.data()[i]->getType()) {
					default:
					break;
					case GOLD_COIN:
						delete Tile_array.data()[i];
						Tile_array.array_splice(i, 1);
						this->collectCoin(1);
						Audio.playSFX(SFX_COLLECT_COIN, 1);
					break;
				}
			}
		}
	}
	
	calcVertPhysics(Audio, gameFrame);
	calcHorizPhysics(Audio, gameFrame);
	
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
	}
}

inline void Player::userGroundPound(SDL_Audio &Audio) {
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
		break;
		case GROUND_POUND:
			userGroundPound(Audio);
		break;
		case CLIMB:
			if (this->climbDir == UP) {
				this->vertVect = 3;
				if (this->animState == ANIM_CLIMB_MOVE) {
					if (this->soundTimer >= 15) {
						Audio.playSFX(SFX_CLIMB, 0);
						this->soundTimer = 0;
					} else
						++this->soundTimer;
				}
			} else if (this->climbDir == DOWN) {
				this->vertVect = -3;
				if (this->animState == ANIM_CLIMB_MOVE) {
					if (this->soundTimer >= 15) {
						Audio.playSFX(SFX_CLIMB, 0);
						this->soundTimer = 0;
					} else
						++this->soundTimer;
				}
			} else if (this->climbDir == NONE) {
				this->vertVect = 0;
			}
			this->playerGroundState = STANDING;
			this->moveState = NORMAL;
			for (size_t i = 0; i < Tile_array.length(); i++) {
				if (tools::checkVertBounds(this->x, this->y - this->vertVect, this->hitboxWidth, this->hitboxHeight, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
					if ((Tile_array.data()[i]->colidable && this->climbDir != UP && !Tile_array.data()[i]->semisolid) || (Tile_array.data()[i]->semisolid && this->y + (float)this->hitboxHeight <= Tile_array.data()[i]->y) ) {
						this->moveState = NORMAL;
						this->vertVect = 0;
						this->y = Tile_array.data()[i]->y - this->hitboxHeight;
						break;
					} else if (Tile_array.data()[i]->ladder) {
						this->moveState = CLIMB;
						break;
					}
				}
			}
			for (size_t i = 0; i < Tile_array.length(); i++) {
				if (tools::checkVertBounds(this->x, this->y - vertVect - 1, this->hitboxWidth, this->hitboxHeight, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
					if (Tile_array.data()[i]->colidable && !Tile_array.data()[i]->semisolid) {
						this->vertVect = 0;
						this->animState = ANIM_CLIMB_STILL;
						this->y = Tile_array.data()[i]->y + Tile_array.data()[i]->height;
						break;
					}
				}
			}
		break;
	}
	
	if (releasingCrouch) {
		releaseDown();
	}
	
	isTouchingGround = false;
	for (size_t i = 0; i < Tile_array.length(); i++) {
		if (tools::checkVertBounds(this->x, this->y, this->hitboxWidth, this->hitboxHeight, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
			if (Tile_array.data()[i]->colidable == true) {
				if (playerGroundState == STANDING) {
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
				if (isTouchingGround == false && this->playerGroundState == FALLING) {
					if (!Tile_array.data()[i]->semisolid || (Tile_array.data()[i]->semisolid && this->y + (float)this->hitboxHeight <= Tile_array.data()[i]->y)) {
						//vertVect = -(Tile_array.data()[intersetingRet]->y - this->y - hitboxHeight);//changed this from setting your vect to setting your pos so that colision bugs aren't so catastrophic
						if (vertVect <= -playerMaxVertSpeed*0.6 && horizVect == 0) {
							Audio.playSFX(SFX_LAND, 0);
						}
						this->vertVect = 0;
						this->y = Tile_array.data()[i]->y - this->hitboxHeight;
						this->playerGroundState = STANDING;
						if (this->moveState == NORMAL) {
							this->animState = ANIM_STAND;
						} else if (this->moveState == SHOULDER_BASH) {
							this->animState = ANIM_BASH;
						}
						
						if (Tile_array.data()[i]->breakable && this->moveState == GROUND_POUND && gameFrame - this->moveStartTime > 2) {
							if (rand() % 2 == 0)
								Tile_array.array_push(new PhysicsTile(Tile_array.data()[i]->x, Tile_array.data()[i]->y, GOLD_COIN));
							delete Tile_array.data()[i];
							Tile_array.array_splice(i, 1);
							Audio.playSFX(SFX_BREAK, 0);
						}
						this->isTouchingGround = true;
					}
				}
			}
		}
	}
	
	for (size_t i = 0; i < Tile_array.length(); i++) {
		if (tools::checkVertBounds(this->x, this->y - vertVect - 1, this->hitboxWidth, this->hitboxHeight, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
			if (Tile_array.data()[i]->colidable == true) {
				if (isTouchingGround == false && playerGroundState == JUMPING && !Tile_array.data()[i]->semisolid) {
					//vertVect = -(Tile_array.data()[intersectingBottomForCeling]->y - this->y + Tile_array.data()[intersectingBottomForCeling]->height) - 1;
					this->y = Tile_array.data()[i]->y + Tile_array.data()[i]->height + 1;
					Tile_array.data()[i]->tileVertVect = 0;
					this->vertVect = 0;
					if (Tile_array.data()[i]->breakable) {
						if (rand() % 2 == 0)
							Tile_array.array_push(new PhysicsTile(Tile_array.data()[i]->x, Tile_array.data()[i]->y, GOLD_COIN));
						delete Tile_array.data()[i];
						Tile_array.array_splice(i, 1);
						Audio.playSFX(SFX_BREAK, 0);
					}
				}
			}
		}
	}
	
	this->y -= this->vertVect;
	
	
	if (this->y > this->vertCameraOffsetOffset) {
		//cameraHorizOffsetPx -= horizVect;
		cameraVertOffsetPx = -this->y + this->vertCameraOffsetOffset;
	} else {
		cameraVertOffsetPx = 0;
	}
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
	}
	
	
	if (playerCheckHorizBoundries(this->x + horizVect, this->y, this->hitboxWidth, this->hitboxHeight, true) != -1) {
		int intersectingTileId = playerCheckHorizBoundries(this->x + horizVect, this->y, this->hitboxWidth, this->hitboxHeight, true);
		
		if (horizVect > 0 && !Tile_array.data()[intersectingTileId]->semisolid) {
			this->x = Tile_array.data()[intersectingTileId]->x - this->hitboxWidth;
			if (this->moveState != SHOULDER_BASH) {
				horizVect = 0;
			} else if (this->moveState == SHOULDER_BASH) {
				if (isTouchingGround) {
					vertVect = 3;
					this->isTouchingGround = false;
				} else
					vertVect = -1;
				horizVect = -5;
				this->moveState = NORMAL;
				this->playerGroundState = JUMPING;
				Audio.playSFX(SFX_BUMP, 0);
				if (Tile_array.data()[intersectingTileId]->breakable) {
					if (rand() % 2 == 0)
						Tile_array.array_push(new PhysicsTile(Tile_array.data()[intersectingTileId]->x, Tile_array.data()[intersectingTileId]->y, GOLD_COIN));
					delete Tile_array.data()[intersectingTileId];
					Tile_array.array_splice(intersectingTileId, 1);
					Audio.playSFX(SFX_BREAK, 1);
				}
			}
		} else if (horizVect < 0 && !Tile_array.data()[intersectingTileId]->semisolid) {
			this->x = Tile_array.data()[intersectingTileId]->x + Tile_array.data()[intersectingTileId]->width;
			if (this->moveState != SHOULDER_BASH) {
				horizVect = 0;
			} else if (this->moveState == SHOULDER_BASH) {
				if (isTouchingGround) {
					vertVect = 3;
					this->isTouchingGround = false;
				} else
					vertVect = -1;
				horizVect = 5;
				this->moveState = NORMAL;
				this->playerGroundState = JUMPING;
				Audio.playSFX(SFX_BUMP, 0);
				if (Tile_array.data()[intersectingTileId]->breakable) {
					if (rand() % 2 == 0)
						Tile_array.array_push(new PhysicsTile(Tile_array.data()[intersectingTileId]->x, Tile_array.data()[intersectingTileId]->y, GOLD_COIN));
					delete Tile_array.data()[intersectingTileId];
					Tile_array.array_splice(intersectingTileId, 1);
					Audio.playSFX(SFX_BREAK, 1);
				}
			}
		}
	}
	
	x += horizVect;
	
	if (this->x > 300) {
		//cameraHorizOffsetPx -= horizVect;
		cameraHorizOffsetPx = -this->x + 300;
	} else {
		cameraHorizOffsetPx = 0;
	}
}

inline void Player::userMoveHoriz(SDL_Audio &Audio) {
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
		if (this->soundTimer >= 40 && this->isTouchingGround == true) {
			Audio.playSFX(SFX_WALK, 0);
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
		
		if (this->soundTimer >= 40 && this->isTouchingGround == true) {
			Audio.playSFX(SFX_WALK, 0);
			this->soundTimer = 0;
		}
		++this->soundTimer;
		
		if (this->isTouchingGround && this->moveState == NORMAL)
			this->animState = ANIM_WALK;
		else if (this->isTouchingGround && this->moveState == CROUCH)
			this->animState = ANIM_CROUCH_WALK;
	}
}

inline void Player::calcShoulderBash(SDL_Audio &Audio, int gameFrame) {
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
	if (this->isTouchingGround == true) {
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
	}
}

void Player::moveHoriz(int dir, SDL_Audio &Audio) {
	if (this->playerPressedMoveDir != dir && this->playerPressedMoveDir == NONE) {
		this->soundTimer = 30;
	}
	
	if (dir != this->playerPressedMoveDir && dir != this->animDirection && this->moveState == SHOULDER_BASH) {
		this->moveState = NORMAL;
		this->horizVect = horizVect*0.5;// otherwise you would keep your momentum after canceling a bash and i found that dum. your canceled horizvect at max charge would be 3.5
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

inline void Player::enterDoor(void) {
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
			if (Tile_array.data()[i]->param1.doorId == Tile_array.data()[ret]->param2.destinationDoorId && this->moveState != SHOULDER_BASH) {
				this->setXYPos(Tile_array.data()[i]->x + (Tile_array.data()[i]->width - this->hitboxWidth)/2, Tile_array.data()[i]->y + -this->hitboxHeight + 100);
				this->vertVect = 0;
				this->horizVect = 0;
				//this->playerPressedMoveDir = NONE;
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
	} else if (isTouchingGround && this->moveState == NORMAL) {
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
	}
	
	if (isTouchingGround) {
		this->releasingCrouch = false;
	}
}

void Player::releaseDown(void) {
	if (this->moveState == CROUCH) {
		bool canColide = false;
		releasingCrouch = true;
		for (size_t i = 0; i < Tile_array.length(); i++) {
			if (tools::checkVertBounds(this->x, this->y - 50, this->hitboxWidth, this->hitboxHeight, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
				if (Tile_array.data()[i]->colidable && !Tile_array.data()[i]->semisolid) {
					canColide = true;
					break;
				}
			}
			if (tools::checkVertBounds(this->x, this->y + this->hitboxHeight, this->hitboxWidth, this->hitboxHeight, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height) && !this->isTouchingGround) { // crappy solution to the floor glitch but idrk
				if ((Tile_array.data()[i]->colidable && !Tile_array.data()[i]->semisolid) || (Tile_array.data()[i]->colidable && Tile_array.data()[i]->semisolid && this->y + (float)this->hitboxHeight <= Tile_array.data()[i]->y)) {
					this->y = Tile_array.data()[i]->y - this->hitboxHeight;
					this->isTouchingGround = true;
					break;
				}
			}
		}
		
		if (!canColide && this->moveState) {
			if (isTouchingGround)
				this->y = this->y - 50;
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
		climbLadder(NONE);
	}
}

void Player::pressUp(SDL_Audio &Audio, int gameFrame) {
	enterDoor();
	enterLadder();// couldn't think of a better name for this :P
}

void Player::releaseUp(void) {
	climbLadder(NONE);
} 

inline void Player::userCrouch(SDL_Audio &Audio, int gameFrame) {
	
}

void Player::climbLadder(int dir) {
	if (dir == NONE && this->moveState != CLIMB)
		return;
	if (this->moveState == CLIMB)
		this->moveState = NORMAL;
	for (size_t i = 0; i < Tile_array.length(); i++) {
		if (tools::checkHorizBounds(this->x, this->y, this->hitboxWidth, this->hitboxHeight, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
			if (Tile_array.data()[i]->ladder) {
				this->climbDir = dir;
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

void Player::enterLadder(void) {
	for (size_t i = 0; i < Tile_array.length(); i++) {
		if (tools::checkHorizBounds(this->x, this->y, this->hitboxWidth, this->hitboxHeight, Tile_array.data()[i]->x, Tile_array.data()[i]->y, Tile_array.data()[i]->width, Tile_array.data()[i]->height)) {
			if (Tile_array.data()[i]->ladder) {
				this->moveState = CLIMB;
				this->x = Tile_array.data()[i]->x + (Tile_array.data()[i]->width - this->hitboxWidth)/2;
				this->animState = ANIM_CLIMB_STILL;
				this->soundTimer = 0;
				climbLadder(UP);
				break;
			}
		}
	}
}