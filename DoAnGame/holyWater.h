#pragma once
#include "./gameObjectEx.h"

#define holy_water_state_item 0
#define holy_water_state_attack 1
#define holy_water_state_burn 2

#define holy_water_gravity 650.0f
#define holy_water_speedX 60.0f
#define holy_water_speedY -150.0f

#define holy_water_ani_item 0
#define holy_water_ani_attack 1
#define holy_water_ani_burn 2

#define holy_water_burnWidthSide 8
#define holy_water_burnWidth 16

#define normalBounding 0
#define burnBounding 1

#define holy_water_

#define burnTime 1500

class holyWater : public gameObjectEx
{
public:
	int boundingMode;

	DWORD startDie;
	DWORD startBurn;

	holyWater(int id);

	virtual FRect getBoundingBox();

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {
	}

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};
