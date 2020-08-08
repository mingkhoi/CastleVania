#include "./axe.h"
#include "./animationSet.h"
#include "./camera.h"
#include "./effectHit.h"
#include "./gamePlay.h"
#include "./torch.h"
#include "./candle.h"

#include <algorithm>

axe::axe(int id)
	: gameObjectEx(id) {
	this->animation = animationSet::getInstance()->getAnimationSet("axe");

	startDie = GetTickCount64();

	this->setSizeItem("axe"); 
}

void axe::die() {
	if (isDestroyed == false)
		isDestroyed = true;
	else
		return;
}

void axe::setState(int value) {
	this->state = value;

	switch (this->state) {
	case axe_state_item:
		aniState = axe_ani_item;
		speed_x = 0;

		break;
	case axe_state_attack:
		aniState = axe_ani_attack;
		speed_x = axe_speedX * directionX;
		speed_y = axe_speedY;

		break;
	}
}

void axe::update(float dt) {
	if (state == axe_state_attack)
		speed_x = axe_speedX * directionX;
	speed_y += axe_gravity * dt;

	if (state == axe_state_item && (GetTickCount64() - startDie) >= item_die_time)
		isDestroyed = true;

	if (standingGround != NULL && standingGround->type == "moving ground"
		&& onTheGround && state == axe_state_item) {
		speed_x = standingGround->getSpeedX();
	}

	vector<string> AABBTag = { "simon"};
	vector<string> AABBType = { "portal", "subweapon" };

	this->updateSweptFilterAABB(game::makeFilterCollision(AABBTag),
		game::makeFilterCollision(AABBType), dt);

	float screenX = pos_x - camera::getInstance()->getX();
	if (screenX < 0 || screenX > wndWidth)
		isDestroyed = true;
}

void axe::collisionHandle(LPCOLLISION colEvent,
	float dt) {
	LPGAMEOBJECT obj = colEvent->obj;

	bool noDestroy = false;

	if (obj->tag == "bossBall") {
		isDestroyed = true;
		return;
	}

	if (colEvent->tag != AABBCollision) {
		if (state != axe_state_item) {

			if (obj->tag == "torch") {
				cast(obj, torch*)->spawnObject();
			}
			else if (obj->tag == "candle") {
				cast(obj, candle*)->spawnObject();
				obj->isDestroyed = true;
			}
			else if (obj->type == "item")
				obj->isDestroyed = true;
			else if (obj->type == "enemy") {
				obj->hurt(item_damage);
			}
			else if (obj->type == "portal") {
				this->removeCollisionBarrier(colEvent, dt);
				noDestroy = true;
			}
			if (!noDestroy)
				isDestroyed = true;
		}
	}
}

void axe::render() {
	this->animation[aniState]->render(pos_x, pos_y, directionX, 255);
}
