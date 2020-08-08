#include "./knife.h"
#include "./animationSet.h"
#include "./camera.h"
#include "./torch.h"
#include "./candle.h"

knife::knife(int id)
	: gameObjectEx(id) {
	this->animation = animationSet::getInstance()->getAnimationSet("knife");

	this->setSizeItem("knife");

	startDie = GetTickCount64();
}

void knife::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {
}

void knife::setState(int value) {
	this->state = value;

	switch (state) {
	case knife_state_item:
		isGravity = true;
		speed_x = 0;

		aniState = 0;
		break;
	case knife_state_attack:
		isGravity = false;
		speed_x = knife_speedX * directionX;

		aniState = 0;
		break;
	}
}

void knife::update(float dt) {
	if (state == knife_state_attack)
		speed_x = knife_speedX * directionX;

	if (state == knife_state_item && (GetTickCount64() - startDie) >= item_die_time)
		isDestroyed = true;

	if (isGravity)
		speed_y += knife_gravity * dt;

	float screenX = pos_x - camera::getInstance()->getX();
	if (screenX < 0 || screenX > wndWidth)
		isDestroyed = true;

	if (standingGround != NULL && standingGround->type == "moving ground"
		&& onTheGround && state == knife_state_item) {
		speed_x = standingGround->getSpeedX();
	}

	// start collision

	vector<string> AABBTag = { "simon" };
	vector<string> AABBType = { "portal", "subweapon" };

	this->updateSweptFilterAABB(game::makeFilterCollision(AABBTag),
		game::makeFilterCollision(AABBType), dt);
}

void knife::collisionHandle(LPCOLLISION colEvent,
	float dt) {

	LPGAMEOBJECT obj = colEvent->obj;

	bool noDestroy = false;

	if (obj->tag == "bossBall") {
		isDestroyed = true;
		return;
	}

	if (colEvent->tag != AABBCollision) {
		if (state != knife_state_item) {

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

void knife::render() {
	this->animation[aniState]->render(pos_x, pos_y, directionX, 255);
}
