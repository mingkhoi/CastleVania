#pragma once
#include "./gameObjectEx.h"

#define money_lv1_gravity 650.0f

class moneyLv1 : public gameObjectEx
{
public:
	moneyLv1(int id)
		: gameObjectEx(id) {
		this->assignAnimationSet("money_lv1");
		this->setSizeItem("moneyLv1");
	}

	virtual void die();

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {
	}

	virtual void setState(int) {}

	virtual void update(float dt) {
		this->speed_y += money_lv1_gravity * dt;

		this->updateSweptAABB(dt);
	}

	virtual void collisionHandle(LPCOLLISION colEvent, float dt) {
		LPGAMEOBJECT obj = colEvent->obj;

		if (obj->tag != "ground") {
			this->removeCollisionBarrier(colEvent, dt);
		}
	}

	virtual void render() {
		this->animation[aniState]->render(pos_x, pos_y, Left, 255);
	}
};
