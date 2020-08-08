#include "./boss.h"
#include "./simon.h"
#include "./camera.h"
#include "./BossBullet.h"
#include "./bossDieEffect.h"
#include "./score.h"
#include "./gamePlay.h"

boss::boss(int id) : gameObjectEx(id) {
	this->health = boss_defaultHealth;

	this->assignAnimationSet("boss");
	this->type = "enemy";
}

D3DXVECTOR2 boss::bulletRootPos() {
	LPGAMEOBJECT target = gamePlay::Instance->player;

	if (target->getX() >= this->pos_x) {
		return D3DXVECTOR2(this->pos_x + this->bbox_w - boss_bulletPad,
			this->pos_y + this->bbox_h);
	}
	else {
		return D3DXVECTOR2(this->pos_x + boss_bulletPad,
			this->pos_y + this->bbox_h);
	}
}

void boss::shoot() {
	LPGAMEOBJECT target = gamePlay::Instance->player;

	D3DXVECTOR2 targetPoint;
	targetPoint.x = target->getX();
	targetPoint.y = target->getY() + boss_targetShootPadY;

	D3DXVECTOR2 disVec = targetPoint - bulletRootPos();
	D3DXVec2Normalize(&disVec, &disVec);

	BossBullet* bullet = new BossBullet(Counter::index++);
	bullet->setSpeed(disVec.x * boss_bulletSpeed, disVec.y * boss_bulletSpeed);
	bullet->setPos(this->bulletRootPos().x, this->bulletRootPos().y);
	gamePlay::Instance->objects.push_back(bullet);

	shootTime = GetTickCount64();
	hasShoot = true;
}

void boss::flyTowardTarget(float dt) {
	if (!isCalculatedPathToTarget) {
		LPGAMEOBJECT target = gamePlay::Instance->player;

		this->attackIdlePoint.x = (this->pos_x + target->getX()) / (float)boss_divisionBeforeAttack;
		this->attackIdlePoint.y = (this->pos_y + target->getY()) / (float)boss_divisionBeforeAttack;

		this->isCalculatedPathToTarget = true;
	}
	else {
		if (goToAttackIdlePoint) {
			if (isTouchAttackIdle) {
				if (GetTickCount64() - touchAttackIdlePointTime >= boss_idle) {

					goToAttackIdlePoint = false;
					goToTargetPoint = true;

					D3DXVECTOR2 disVec = this->attackTargetPoint - D3DXVECTOR2(this->pos_x, this->pos_y);

					this->speed_x = (disVec.x / abs(disVec.x)) * boss_attackSpeedX;
					this->speed_y = disVec.y * boss_attackForceY;

					return;
				}
				else {
					this->speed_x = this->speed_y = 0;

					LPGAMEOBJECT target = gamePlay::Instance->player;

					float x2, y2;

					if (target->getX() + target->getWidth() >= this->pos_x) {
						x2 = target->getX() + target->getWidth() + boss_targetAttackPadX;
						y2 = target->getY() - boss_targetAttackPadY;
					}
					else if (target->getX() <= this->pos_x + this->bbox_w) {
						x2 = target->getX() - boss_targetAttackPadX;
						y2 = target->getY() - boss_targetAttackPadY;
					}
					this->attackTargetPoint = D3DXVECTOR2(x2, y2);
				}
				return;
			}
			if (!game::isSamePoint(this->pos_x, this->attackIdlePoint.x, this->speed_x) ||
				!game::isSamePoint(this->pos_y, this->attackIdlePoint.y, this->speed_y)) {
				D3DXVECTOR2 disVec = this->attackIdlePoint - D3DXVECTOR2(this->pos_x, this->pos_y);

				D3DXVec2Normalize(&disVec, &disVec);
				this->speed_x = disVec.x * boss_defaultSpeedX;
				this->speed_y = disVec.y * boss_defaultSpeedY;

				return;
			}
			else {
				if (!isTouchAttackIdle) {
					touchAttackIdlePointTime = GetTickCount64();
					isTouchAttackIdle = true;
				}
			}
		}
		else if (goToTargetPoint) {
			this->speed_y -= boss_reverseGravity * dt;
			if (this->speed_y <= -boss_stopAttackSpeedY)
			{
				isTouchTargerEnd = true;
				goToTargetPoint = false;

				canFlyTowardTarget = false;
				canFlyHome = true;
				return;
			}
		}
	}
}

void boss::flyTowardHome(float dt) {
	D3DXVECTOR2 vec1 = D3DXVECTOR2(5, 0);
	D3DXVECTOR2 vec2 = D3DXVECTOR2(5, 5);

	float anglePi = D3DXVec2Dot(&vec1, &vec2);

	print("[dot product test %f]\n", anglePi); 

	if (!isCalculatedPathToHome) {

		float x2, y2;

		if (speed_x > 0) {
			x2 = this->pos_x - boss_distanceBackwardX;
			y2 = gamePlay::Instance->rootY + boss_homeY;
		}
		else {
			x2 = this->pos_x + boss_distanceBackwardX;
			y2 = gamePlay::Instance->rootY + boss_homeY;
		}

		this->homePoint = D3DXVECTOR2(x2, y2);

		D3DXVECTOR2 disVec = this->homePoint - D3DXVECTOR2(this->pos_x, this->pos_y);

		D3DXVec2Normalize(&disVec, &disVec);
		this->speed_x = disVec.x * boss_returnSpeedX;
		this->speed_y = disVec.y * boss_returnSpeedY;

		this->isCalculatedPathToHome = true;
	}
	else {
		if (!goToHomePoint) {
			if (isTouchHomePoint) {
				if (GetTickCount64() - touchHomePoint >= boss_idle) {
					goToHomePoint = true;
					isHome = true;
				}
				else {
					this->speed_x = this->speed_y = 0;
				}
				return;
			}
			this->speed_y += boss_reverseGravity * dt;

			if (this->speed_y >= boss_gravity ||
				game::isSamePoint(this->pos_x, this->homePoint.x, this->speed_x)) {

				if (!isTouchHomePoint) {
					touchHomePoint = GetTickCount64();
					isTouchHomePoint = true;
				}
			}
		}
	}
}

void boss::waitSimonHandle() {
	if (waitForSimon == false) {
		return;
	}

	aniState = boss_ani_idle;
	speed_x = speed_y = 0;

	if (gamePlay::Instance->player != NULL && this->detectSimon == false) {
		LPGAMEOBJECT target = gamePlay::Instance->player;

		if (target->getX() > gamePlay::Instance->rootX + gamePlay::Instance->currentTileSet->mapWidth
			- wndWidth + boss_detectSimon && camera::getInstance()->isTouchLimit) {
			gamePlay::Instance->startBossMode = true;
			this->detectSimon = true;

			this->waitBeforeStartTime = GetTickCount64();
		}
	}
	if (this->detectSimon == true) {
		if (GetTickCount64() - this->waitBeforeStartTime >= boss_waitBeforeStart) {
			waitForSimon = false;
			isHome = true;
			aniState = boss_ani_fly;
		}
	}
}

void boss::prepareAttack() {
	canShoot = false;
	hasShoot = false;

	canFlyTowardTarget = false;
	canFlyHome = false;

	isAttacking = true;

	isCalculatedPathToTarget = false;
	isCalculatedPathToHome = false;

	goToAttackIdlePoint = true;
	goToTargetPoint = false;
	goToHomePoint = false;

	isTouchAttackIdle = false;
	isTouchTargerEnd = false;
	isTouchHomePoint = false;
}

void boss::mainHandle(float dt) {
	if (isHome) {
		this->attackMode = (rand() % 2) + 1;
		isHome = false;

		prepareAttack();

		if (this->attackMode == boss_attack) {
			this->canFlyTowardTarget = true;
		}
		else if (this->attackMode == boss_shoot) {
			canShoot = true;
		}
	}
	if (this->attackMode == boss_attack) {
		this->attackHandle(dt);
		this->returnHomeHandle(dt);
	}
	else if (this->attackMode == boss_shoot) {
		this->shootHandle();
	}
}

void boss::shootHandle() {
	if (!canShoot) {
		return;
	}
	if (hasShoot == true) {
		if (GetTickCount64() - this->shootTime >= boss_idle) {
			canShoot = false;
			isHome = true;
		}
		return;
	}
	shoot();
}

void boss::attackHandle(float dt) {
	if (!canFlyTowardTarget)
		return;

	float limitNegX = gamePlay::Instance->rootX + gamePlay::Instance->currentTileSet->mapWidth - wndWidth;
	float limitPosX = gamePlay::Instance->rootX + gamePlay::Instance->currentTileSet->mapWidth;

	if (isTouchAttackIdle && (this->pos_x < limitNegX || this->pos_x > limitPosX - this->bbox_w)) {
		canFlyTowardTarget = false;
		canFlyHome = true;
		return;
	}
	flyTowardTarget(dt);
}

void boss::returnHomeHandle(float dt) {
	if (!canFlyHome) {
		return;
	}

	float limitNegX = gamePlay::Instance->rootX + gamePlay::Instance->currentTileSet->mapWidth - wndWidth;
	float limitPosX = gamePlay::Instance->rootX + gamePlay::Instance->currentTileSet->mapWidth;

	if (this->pos_x < limitNegX || this->pos_x > limitPosX - this->bbox_w ||
		pos_y <= gamePlay::Instance->rootY + boss_homeY) {

		speed_x = speed_y = 0;

		canFlyHome = false;
		isHome = true;

		return;
	}

	flyTowardHome(dt);
}

FRect boss::getBoundingBox() {
	FRect result;
	result.left = this->pos_x + boss_sizePad;
	result.right = this->pos_x + this->bbox_w - boss_sizePad;
	result.top = this->pos_y;
	result.bottom = this->pos_y + this->bbox_h;

	return result;
}

void boss::hurt(int damage) {
	if (damage > 2) {
		damage /= 3;
	}
	else if (damage > 1) {
		damage /= 2;
	}

	this->health -= damage;

	if (this->health <= 0) {
		this->health = 0;
		this->isDestroyed = true;

		bossDieEffect *dieEffect = new bossDieEffect(Counter::index++);
		dieEffect->setPos(this->pos_x, this->pos_y); 
		gamePlay::getInstance()->objects.push_back(dieEffect);

	}
	score::getInstance()->updateEnemy(this->health);

	isHurted = true;

	startHurtTime = GetTickCount64();

	this->spawnHitEffect();
}

void boss::hurtHandle() {
	if (isHurted == false) {
		return;
	}
	if (GetTickCount64() - startHurtTime >= boss_hurtTime) {
		isHurted = false;
	}
	else {
		speed_x = speed_y = 0;
	}
}

void boss::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {
}

void boss::setState(int) {
}

void boss::update(float dt) {
	waitSimonHandle();

	if (waitForSimon == true) {
		return;
	}
	if (!isHurted)
		mainHandle(dt);

	hurtHandle();

	this->updateSweptAABB(dt);
}

void boss::collisionHandle(LPCOLLISION collisionEvent,
	float dt) {
	LPGAMEOBJECT obj = collisionEvent->obj;

	this->removeCollisionBarrier(collisionEvent, dt);

	if (obj->tag == "simon") {
		cast(gamePlay::Instance->player, simon*)->hurt(collisionEvent->nx,
			collisionEvent->ny, boss_damage);
	}
}

void boss::render() {
	float centerX = pos_x + (bbox_w / 2);
	float centerY = pos_y + (bbox_h / 2);

	this->animation[aniState]->renderCenter(centerX, centerY, Left,
		D3DCOLOR_XRGB(255, 255, 255));
}