#include "./vampireBat.h"
#include "./camera.h"
#include "./gamePlay.h"
#include "./simon.h"

#include "./effectDie.h"
#include "./effectHit.h"

#include <math.h>

using namespace std;

vampireBat::vampireBat(int id)
	: gameObjectEx(id) {
	type = "enemy";

	this->health = vampireBat_health;

	this->assignAnimationSet("vampire_bat");

	this->setState(vampireBat_state_idle);
}

float vampireBat::getDistanceToTarget(LPGAMEOBJECT target) {
	float distance = 0.0f;

	if (target->getX() > this->pos_x + this->bbox_w) {
		distance = std::abs(target->getX() - (this->pos_x + this->bbox_w));
	}
	else if (target->getX() + target->getWidth() < this->pos_x) {
		distance = std::abs(this->pos_x - (target->getX() + target->getWidth()));
	}
	else
		distance = 0.0f;

	return distance;
}

void vampireBat::calculateSpeed(LPGAMEOBJECT target) {
	D3DXVECTOR2 targetPos = D3DXVECTOR2(target->getX(),
		target->getY() + target->getHeight());
	D3DXVECTOR2 thisPos = D3DXVECTOR2(this->pos_x, this->pos_y);

	D3DXVECTOR2 res = targetPos - thisPos;

	D3DXVECTOR2 normalizedVec;

	print("[BAT] following\n");

	D3DXVec2Normalize(&normalizedVec, &res);

	speed_x = normalizedVec.x * vampireBat_speed;
	speed_y = normalizedVec.y * vampireBat_speed;
}

void vampireBat::hurt(int damage) {
	if (isDestroyed || isHurted)
		return;

	this->health -= damage;
	isHurted = true;

	this->spawnHitEffect();

	startHurtedTime = GetTickCount64();

	if (this->health <= 0) {
		this->spawnDieEffect();

		isDestroyed = true;
	}
}

void vampireBat::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {
}

void vampireBat::setState(int value) {
	state = value;

	switch (state) {
	case vampireBat_state_idle:
		speed_x = speed_y = 0;

		aniState = vampireBat_ani_idle;
		break;
	case vampireBat_state_attack:
		aniState = vampireBat_ani_attack;

		animation[aniState]->reset();
		break;
	}
}

void vampireBat::update(float dt) {
	simon* target = cast(gamePlay::getInstance()->player, simon*);
	camera* cam = camera::getInstance();

	// first time detect target

	if (!firstTimeDetect) {
		if (abs(target->getY() - this->pos_y) <= vampireBat_distance_y && 
			this->getDistanceToTarget(target) <= vampireBat_distance_detect) {

			setState(vampireBat_state_attack);

			if (target->getX() - this->pos_x > 0)
				directionX = Right;
			else
				directionX = Left;

			firstTimeDetect = true;
		}
	}

	// verified target

	if (state == vampireBat_state_attack) {
		if (!turnOffTracking) {
			calculateSpeed(target);

			turnOffTracking = true;
		}
		pos_y += vampireBat_distance_increase * dt;
	}

	vector<string> AABBTag = { "torch", "candle", "flame" };
	vector<string> AABBType = { "enemy", "item", "portal", "subweapon" };

	this->updateSweptFilterAABB(game::makeFilterCollision(AABBTag),
		game::makeFilterCollision(AABBType), dt);

	float screen_x = pos_x - cam->getX();
	float screen_y = pos_y - cam->getY();

	if (screen_x < 0 || screen_x > wndWidth || screen_y < 0 || screen_y > wndHeight) {
		this->isDestroyed = true;
	}
}

void vampireBat::collisionHandle(LPCOLLISION colEvent,
	float dt) {
	LPGAMEOBJECT obj = colEvent->obj;

	if (state == vampireBat_state_attack) {
		if (obj->tag == "simon") {
			if (cast(obj, simon*)->hurt(colEvent->nx, colEvent->ny, vampireBat_damage)) {
				this->hurt(vampireBat_damage);
			}
		}
	}
	this->removeCollisionBarrier(colEvent, dt);
}

void vampireBat::render() {
	this->animation[aniState]->render(pos_x, pos_y, directionX, 255);
}
