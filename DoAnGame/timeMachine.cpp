#include "./timeMachine.h"

#include "./gamePlay.h"
#include "./animationSet.h"
#include "./camera.h"

timeMachine::timeMachine(int id)
	: gameObjectEx(id) {
	this->animation = animationSet::getInstance()->getAnimationSet("time_machine");

	this->setSizeItem("timeMachine");

	startDie = GetTickCount64();
}

void timeMachine::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {
}

void timeMachine::setState(int value) {
	this->state = value;

	switch (state) {
	case timeMachine_state_item:
		isGravity = true;
		speed_x = 0;

		aniState = 0;
		break;
	case timeMachine_state_attack:
		isGravity = false;
		speed_x = speed_y = 0;

		aniState = 0;
		break;
	}
}

void timeMachine::update(float dt) {
	if (state == timeMachine_state_attack) {
		gamePlay::getInstance()->pauseScreen();
		this->isDestroyed = true;

		speed_x = speed_y = 0;
		return;
	}

	if (state == timeMachine_state_item && (GetTickCount64() - startDie) >= item_die_time)
		isDestroyed = true;

	if (isGravity)
		speed_y += timeMachine_gravity * dt;

	float screenX = pos_x - camera::getInstance()->getX();
	if (screenX < 0 || screenX > wndWidth)
		isDestroyed = true;

	if (standingGround != NULL && standingGround->type == "moving ground"
		&& onTheGround && state == timeMachine_state_item) {
		speed_x = standingGround->getSpeedX();
	}

	// start collision

	vector<string> AABBTag = { "simon" };
	vector<string> AABBType = { "portal", "subweapon" };

	this->updateSweptFilterAABB(game::makeFilterCollision(AABBTag),
		game::makeFilterCollision(AABBType), dt);
}

void timeMachine::collisionHandle(LPCOLLISION colEvent,
	float dt) {
	if (colEvent->obj->tag != "ground") {
		this->removeCollisionBarrier(colEvent, dt);
	}
}

void timeMachine::render() {
	this->animation[aniState]->render(pos_x, pos_y, directionX, 255);
}