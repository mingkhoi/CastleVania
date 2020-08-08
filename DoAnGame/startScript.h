#pragma once
#include "./gameObject.h"

#define idle 0
#define flash 1

class startScript : public gameObject
{
public:
	startScript(int id);

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int) {
	}

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt) {
	}

	virtual void render();
};
