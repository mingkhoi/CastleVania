#pragma once
#include "./gameObjectEx.h"

#define zombie_health 1
#define zombie_speed_x 45
#define zombie_damage 1
#define zombie_state_walk 0
#define zombie_hit_time 200
#define zombie_gravity 650.0f 
#define zombie_out_screen 100

class zombie : public gameObjectEx
{
public:
	DWORD startHurtedTime;

	int health;
	bool isHurted;

	zombie(int id);

	virtual void hurt(int damage);

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};

