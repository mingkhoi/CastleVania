#pragma once
#include "./gameObject.h"

class introShip : public gameObject
{
public:
	introShip(int id);

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {
	}

	virtual void setState(int) {
	}

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt) {
	}

	virtual void render();
};
