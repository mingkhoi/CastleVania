#pragma once
#include "./gameObjectEx.h"

#define doubleShot_gravity 450.0f

class doubleShot : public gameObjectEx
{
public:
	doubleShot(int id) : gameObjectEx(id) {
		this->assignAnimationSet("double_shot");
		this->tag = "doubleShot";
		this->type = "item";

		this->setSizeItem("doubleShot");

		aniState = 0;
	}

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {
	}

	virtual void setState(int) {}

	virtual void update(float dt) {
		this->speed_y += doubleShot_gravity * dt;

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
		this->animation[aniState]->render(pos_x, pos_y, Left, 255);
	}
};

