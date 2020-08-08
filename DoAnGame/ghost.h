#pragma once
#include "./gameObjectEx.h"

#define ghost_distanceAttack 10
#define ghost_attackTime 1200
#define ghost_flying_speedX 40
#define ghost_flying_speedY 40

#define ghost_hurt_time 200
#define ghost_damage 2
#define ghost_health 3

class ghost : public gameObjectEx
{
public:
	ghost(int id);

	DWORD startHurt;
	DWORD startAttack;

	bool isAttack = false;
	bool callAttack = false;
	bool isHurt = false;

	float speedAttackX;
	float speedAttackY;

	int health;

	void flying();
	void keepAttackSpeed();
	virtual void hurt(int damage);

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};
