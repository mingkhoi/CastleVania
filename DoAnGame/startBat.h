#pragma once
#include "./database.h"
#include "./gameObject.h"

#define fly_1 0
#define fly_2 1

class startBat : public gameObject
{
public:
	startBat(int id);

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {
	}

	virtual void setState(int) {
	}

	virtual void update(float dt) {
	}

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt) {
	}

	virtual void render();
};
