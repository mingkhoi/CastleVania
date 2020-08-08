#pragma once
#include "./gameObjectEx.h"

#define axe_speedX 60.0f
#define axe_speedY -320.0f
#define axe_gravity 650.0f

#define axe_state_item 0
#define axe_state_attack 1

#define axe_ani_item 0
#define axe_ani_attack 1

class axe : public gameObjectEx
{
public:
	DWORD startDie;

	axe(int id);

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