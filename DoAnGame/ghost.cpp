#include "./ghost.h"
#include "./camera.h"
#include "./effectDie.h"
#include "./effectHit.h"
#include "./gamePlay.h"
#include "./simon.h"

ghost::ghost(int id)
	: gameObjectEx(id) {
	this->type = "enemy";
	this->health = ghost_health;

	this->assignAnimationSet("ghost");
}

void ghost::flying() {
	LPGAMEOBJECT target = gamePlay::getInstance()->player;

	D3DXVECTOR2 targetVec = D3DXVECTOR2(target->getX(),
		target->getY() + target->getHeight() / 2);

	D3DXVECTOR2 thisVector = D3DXVECTOR2(this->pos_x, this->pos_y);

	D3DXVECTOR2 distanceVec = targetVec - thisVector;
	D3DXVec2Normalize(&distanceVec, &distanceVec);

	if (target->getX() > this->pos_x)
		directionX = Right;
	else
		directionX = Left;

	this->speed_x = distanceVec.x * ghost_flying_speedX;
	this->speed_y = distanceVec.y * ghost_flying_speedY;
}

void ghost::hurt(int damage) {
	this->health -= damage;

	this->spawnHitEffect();

	if (this->health <= 0) {
		isDestroyed = true;

		this->spawnDieEffect();
		return;
	}
	startHurt = GetTickCount64();
	isHurt = true;
}

void ghost::keepAttackSpeed() {
	if (isAttack)
		return;

	LPGAMEOBJECT target = gamePlay::getInstance()->player;

	speedAttackX = speed_x;
	speedAttackY = speed_y;

	startAttack = GetTickCount64();
	isAttack = true;
}

void ghost::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {
}

void ghost::setState(int value) {
}

void ghost::update(float dt) {
	if (!isAttack)
		this->flying();

	if (isAttack) {
		if (GetTickCount64() - startAttack >= ghost_attackTime) {
			isAttack = false;
		}
		else {
			if (std::abs(speedAttackX) > 0) {
				speed_x = speedAttackX;
				speed_y = speedAttackY;
			}
		}
	}

	if (isHurt) {
		if (GetTickCount64() - startHurt >= ghost_hurt_time)
			isHurt = false;
		else
			speed_x = speed_y = 0;
	}

	this->updateSweptAABB(dt);
}

void ghost::collisionHandle(LPCOLLISION colEvent,
	float dt) {
	LPGAMEOBJECT obj = colEvent->obj;

	if (obj->tag == "simon") {
		cast(obj, simon*)->hurt(colEvent->nx, colEvent->ny, ghost_damage);
		this->keepAttackSpeed();
	}
	this->removeCollisionBarrier(colEvent, dt);
}

void ghost::render() {
	this->animation[0]->render(pos_x, pos_y, directionX, 255);
}