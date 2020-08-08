#pragma once
#include "./gameObjectEx.h"

#define bow_gravity 650.0f

class bow : public gameObjectEx
{
public:
	bow(int id)
		: gameObjectEx(id) {
		this->assignAnimationSet("bow");
		this->setSizeItem("bow");
	}

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {
	}

	virtual void setState(int) {}

	virtual void update(float dt) {
		speed_y += bow_gravity * dt;

		if (standingGround != NULL && standingGround->type == "moving ground"
			&& onTheGround && state == item_state) {
			speed_x = standingGround->getSpeedX();
		}

		this->updateSweptAABB(dt);
	}

	virtual void collisionHandle(LPCOLLISION colEvent,
		float dt) {
		LPGAMEOBJECT obj = colEvent->obj;

		if (obj->tag != "ground") {
			this->removeCollisionBarrier(colEvent, dt);
		}
	}

	virtual void render() {
		animation.back()->render(pos_x, pos_y, Left, 255);
	}
};
