#pragma once
#include "./gameObjectEx.h"

#define boomerang_speedX 90.0f
#define boomerang_gravity 650.0f
#define boomerang_dame 2

#define boomerang_distance 64

#define boomerang_state_item 0
#define boomerang_state_attack 1

#define boomerang_ani_item 0
#define boomerang_ani_attack 1

#define dieTime 8000

class boomerang : public gameObjectEx
{
public:
	bool turnAround = false;
	float count;

	DWORD startDie;

	boomerang(int id);

	virtual void die();

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {
	}

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};

