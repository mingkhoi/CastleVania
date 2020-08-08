#pragma once
#include "./gameObjectEx.h"

#define invisible_gravity 650.0f


class invisible : public gameObjectEx
{
public:
	invisible(int id)
		: gameObjectEx(id) {
		this->assignAnimationSet("invisible");
		this->setSize(13, 16);
	}

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {
	}

	virtual void setState(int) {}

	virtual void update(float dt) {
		this->speed_y += invisible_gravity * dt;

		if (standingGround != NULL && standingGround->type == "moving ground"
			&& onTheGround && state == item_state) {
			speed_x = standingGround->getSpeedX();
		}

		this->updateSweptAABB(dt);
	}

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt) {
	}

	virtual void render() {
		this->animation[aniState]->render(pos_x, pos_y, Left, 255);
	}
};