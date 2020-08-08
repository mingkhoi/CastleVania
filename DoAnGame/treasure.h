#pragma once
#include "./gameObjectEx.h"

#define treasure_gravity 650.0f 

class treasure : public gameObjectEx
{
public:
	treasure(int id)
		: gameObjectEx(id) {
		this->assignAnimationSet("treasure");
		this->setSizeItem("treasure");

		this->tag = "treasure";
		this->type = "item";
	}

	virtual void die();

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {
	}

	virtual void setState(int) {}

	virtual void update(float dt) {
		this->speed_y += treasure_gravity * dt;

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

