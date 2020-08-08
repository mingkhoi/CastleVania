#pragma once
#include "./gameObjectEx.h"

#define small_heart_gravity 450.0f
#define speedX 60.0f
#define distance 30.0f

class smallHeart : public gameObjectEx
{
public:
	bool firstTimeGetPos = true;

	smallHeart(int id)
		: gameObjectEx(id) {
		this->assignAnimationSet("small_heart");
		this->setSizeItem("smallHeart");
		aniState = 0;
	}

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {
	}

	virtual void setState(int) {}

	virtual void update(float dt) {
		speed_y += small_heart_gravity * dt;

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
