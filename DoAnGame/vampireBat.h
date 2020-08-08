#pragma once
#include "./gameObjectEx.h"

#define vampireBat_speed 100
#define vampireBat_distance_detect 110
#define vampireBat_distance_y 40
#define vampireBat_distance_increase -8.5f
#define vampireBat_distance_stop 20

#define vampireBat_state_idle 0
#define vampireBat_state_attack 1

#define vampireBat_ani_idle 0
#define vampireBat_ani_attack 1
#define vampireBat_damage 1
#define vampireBat_health 1

class vampireBat : public gameObjectEx
{
public:
	bool firstTimeDetect = false;
	bool turnOffTracking = false;
	bool isHurted = false;

	int health;
	DWORD startHurtedTime;

	vampireBat(int id);

	float getDistanceToTarget(LPGAMEOBJECT target);

	void calculateSpeed(LPGAMEOBJECT target);

	void hurt(int damage);

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};