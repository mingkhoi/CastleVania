#include "./raven.h"
#include "./gamePlay.h"
#include "./simon.h"
#include <vector>

using namespace std;

raven::raven(int id) : gameObjectEx(id) {
	this->assignAnimationSet("raven");
	this->type = "enemy";

	this->health = raven_health;
	aniState = 0;
}

void raven::hurt(int damage) {
	this->health -= damage;

	if (this->health <= 0) {
		this->spawnDieEffect();

		isDestroyed = true;
	}
	this->spawnHitEffect();
}

void raven::observeSimon() {
	LPGAMEOBJECT target = gamePlay::getInstance()->player;

	if (target == NULL)
		return;

	if (target->getX() + target->getWidth() >= this->pos_x)
		directionX = Right;
	else if (target->getX() <= this->pos_x + this->bbox_w)
		directionX = Left;
}

void raven::detectSimon() {
	if (hasDetected) return;

	LPGAMEOBJECT target = gamePlay::getInstance()->player;

	if (target == NULL) return;

	D3DXVECTOR2 targetVec = D3DXVECTOR2(target->getX(), target->getY());
	D3DXVECTOR2 thisVec = D3DXVECTOR2(this->pos_x, this->pos_y);
	D3DXVECTOR2 dis = targetVec - thisVec;

	float length = D3DXVec2Length(&dis);

	if (length <= raven_detect_simon) {
		hasDetected = true;

		aniState = 1;
	}
}

bool raven::isAttackHorizon() {
	LPGAMEOBJECT target = gamePlay::getInstance()->player;

	if (target == NULL)
		return false;

	FRect area = this->getBoundingBox();

	if (area.bottom <= target->getY() ||
		area.top >= target->getY() + target->getHeight())
		return false;
	else return true;
}

bool raven::isAttackTracking() {
	return !isAttackHorizon();
}

void raven::flyToPoint() {
	LPGAMEOBJECT target = gamePlay::getInstance()->player;

	if (target == NULL || !hasDetected) 
		return;

	if (hasChoosePoint && !hasMapPoint) {
		D3DXVECTOR2 thisVec = D3DXVECTOR2(this->pos_x, this->pos_y);
		D3DXVECTOR2 dis = destinationPoint - thisVec;

		if (game::isSamePoint(this->pos_x, destinationPoint.x, this->speed_x) &&
			game::isSamePoint(this->pos_y, destinationPoint.y, this->speed_y)) {

			this->setPos(destinationPoint.x, destinationPoint.y);
			hasMapPoint = true;

			speed_x = speed_y = 0;

			isWait = true;
			startWaitTime = GetTickCount64();
		}
		else {
			D3DXVec2Normalize(&dis, &dis);
			speed_x = dis.x * raven_speed;
			speed_y = dis.y * raven_speed;
		}
		return;
	}

	if (hasChoosePoint) return;

	vector<D3DXVECTOR2> points(4);

	points[0] = D3DXVECTOR2(target->getX(), target->getY() - this->bbox_h);
	points[1] = D3DXVECTOR2(target->getX(), target->getY() + this->bbox_h);
	points[2] = D3DXVECTOR2(target->getX() - raven_distance_to_simon, target->getY());
	points[3] = D3DXVECTOR2(target->getX() + raven_distance_to_simon, target->getY());

	if (isAttackHorizon()) {
		destinationPoint = points[rand() % 2];
		hasChoosePoint = true;
		hasMapPoint = false;
	}
	else if (isAttackTracking()) {
		destinationPoint = points[(rand() % 2) + 2];
		hasChoosePoint = true;
		hasMapPoint = false;
	}
}

void raven::decideAttack() {
	if (hasDecideAttack || !hasMapPoint)
		return;

	if (GetTickCount64() - startWaitTime < raven_wait)
		return;
	else isWait = false;

	if (isAttackHorizon()) {
		speed_x = directionX * raven_speed;
		speed_y = 0;

		endAttackPoint.x = this->pos_x + (directionX * raven_distance);
		endAttackPoint.y = this->pos_y;
		hasDecideAttack = true;
	}
	else if (isAttackTracking()) {
		LPGAMEOBJECT target = gamePlay::getInstance()->player;

		D3DXVECTOR2 targetVec = D3DXVECTOR2(target->getX(), target->getY());
		D3DXVECTOR2 thisVec = D3DXVECTOR2(this->pos_x, this->pos_y);
		D3DXVECTOR2 dis = targetVec - thisVec;

		D3DXVec2Normalize(&dis, &dis); 

		speed_x = dis.x * raven_speed;
		speed_y = dis.y * raven_speed;

		endAttackPoint.x = this->pos_x + (dis.x * raven_distance);
		endAttackPoint.y = this->pos_y + (dis.y * raven_distance);
		hasDecideAttack = true;
	}
}

void raven::attack() {
	if (!hasDecideAttack)
		return;

	D3DXVECTOR2 thisVec = D3DXVECTOR2(this->pos_x, this->pos_y);
	D3DXVECTOR2 dis = endAttackPoint - thisVec;

	if (game::isSamePoint(this->pos_x, endAttackPoint.x, this->speed_x) &&
		game::isSamePoint(this->pos_y, endAttackPoint.y, this->speed_y)) {

		this->setSpeed(0, 0);
		this->setPos(endAttackPoint.x, endAttackPoint.y);

		hasDecideAttack = false;
		hasChoosePoint = false;
		hasMapPoint = false;
	}
}

void raven::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {

}

void raven::setState(int) {

}

void raven::update(float dt) {

	this->observeSimon();
	this->detectSimon();
	this->flyToPoint();
	this->decideAttack();
	this->attack();

	vector<string> AABBTag = { "torch", "candle", "flame" };
	vector<string> AABBType = { "enemy", "item", "portal", "subweapon" };

	this->updateSweptFilterAABB(game::makeFilterCollision(AABBTag),
		game::makeFilterCollision(AABBType), dt);
}

void raven::collisionHandle(LPCOLLISION colEvent,
	float dt) {
	LPGAMEOBJECT obj = colEvent->obj;

	if (obj->tag == "simon") {
		this->hurt(raven_health);
		cast(obj, simon*)->hurt(colEvent->nx, colEvent->ny, raven_damage);
	}
	this->removeCollisionBarrier(colEvent, dt);
}

void raven::render() {
	animation[aniState]->render(pos_x, pos_y, directionX, 255);
}