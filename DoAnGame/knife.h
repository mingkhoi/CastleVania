#pragma once
#include "./gameObjectEx.h"

#define knife_gravity 650.0f
#define knife_speedX 250.0f

#define knife_state_item 0
#define knife_state_attack 1

class knife : public gameObjectEx
{
public:
	DWORD startDie;
	bool isGravity;

	knife(int id);

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};