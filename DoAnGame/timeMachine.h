#pragma once
#include "./gameObjectEx.h"

#define timeMachine_gravity 650.0f
#define timeMachine_state_item 0
#define timeMachine_state_attack 1

class timeMachine : public gameObjectEx
{
public:
	bool isGravity = false;
	DWORD startDie;

	timeMachine(int id);

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};

