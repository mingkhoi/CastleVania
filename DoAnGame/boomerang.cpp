#include "./boomerang.h"
#include "./animationSet.h"
#include "./camera.h"
#include "./effectHit.h"
#include "./gamePlay.h"
#include "./torch.h"
#include "./candle.h"

#include <algorithm>

boomerang::boomerang(int id) : gameObjectEx(id) {
	this->animation = animationSet::getInstance()->getAnimationSet("boomerang");

	startDie = GetTickCount64();

	this->setSizeItem("boomerang");
}

void boomerang::die() {
	if (isDestroyed == false)
		isDestroyed = true;
	else
		return;
}

void boomerang::setState(int value) {
	this->state = value;

	switch (this->state) {
	case boomerang_state_item:
		aniState = boomerang_ani_item;
		speed_x = 0;

		break;
	case boomerang_state_attack:
		aniState = boomerang_ani_attack;
		speed_x = boomerang_speedX * directionX;
		speed_y = 0;

		break;
	}
}

void boomerang::update(float dt) {
	if (state == boomerang_state_attack) {
		speed_x = boomerang_speedX * directionX;
		count += std::abs(speed_x * dt);

		if (count >= boomerang_distance && !turnAround) {
			directionX *= -1;
			turnAround = true;
		}
	}
	else {
		speed_y += boomerang_gravity * dt;
	}

	if (state == boomerang_state_item && (GetTickCount64() - startDie) >= dieTime)
		isDestroyed = true;

	if (standingGround != NULL && standingGround->type == "moving ground"
		&& onTheGround && state == boomerang_state_item) {
		speed_x = standingGround->getSpeedX();
	}

	vector<string> AABBTag = { };
	vector<string> AABBType = { "portal", "subweapon" };

	this->updateSweptFilterAABB(game::makeFilterCollision(AABBTag),
		game::makeFilterCollision(AABBType), dt);

	float screenX = pos_x - camera::getInstance()->getX();
	if (screenX + this->bbox_w < 0 || screenX > wndWidth)
		isDestroyed = true;
}

void boomerang::collisionHandle(LPCOLLISION colEvent,
	float dt) {
	LPGAMEOBJECT obj = colEvent->obj;

	if (obj->tag == "bossBall") {
		isDestroyed = true;
		return;
	}

	if (colEvent->tag != AABBCollision) {
		if (state != boomerang_state_item) {
			if (obj->tag == "torch") {
				cast(obj, torch*)->spawnObject();
			}
			else if (obj->tag == "candle") {
				cast(obj, candle*)->spawnObject();
			}
			else if (obj->type == "item")
				obj->isDestroyed = true;
			else if (obj->type == "enemy") {
				obj->hurt(item_damage);
			}
			else if (obj->tag == "simon") {
				this->isDestroyed = true;
			}
			this->removeCollisionBarrier(colEvent, dt);
		}
		else if (obj->tag != "ground") {
			this->removeCollisionBarrier(colEvent, dt);
		}
	}
}

void boomerang::render() {
	this->animation[aniState]->render(pos_x, pos_y, directionX, 255);
}
