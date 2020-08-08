#pragma once
#include "./gameObject.h"

#define endEffect 200

class effectHit : public gameObject
{
public:
	DWORD startCount;

	effectHit(int id)
		: gameObject(id) {
		this->assignAnimationSet("hit");

		this->canTouch = false;
		startCount = GetTickCount64();
	}

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {
	}

	virtual void setState(int) {}

	virtual void update(float dt) {
		if (GetTickCount64() - startCount > endEffect)
			isDestroyed = true;
	}

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt) {
	}

	virtual void render() {
		this->animation.back()->render(pos_x, pos_y, Left, 255);
	}
};
