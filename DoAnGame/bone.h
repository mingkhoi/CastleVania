#pragma once
#include "./gameObjectEx.h"

#define bone_gravity 650.0f
#define bone_damage 3
#define bone_health 1

class bone : public gameObjectEx
{
public:
	int health;
	
	bone(int id, float speedx, float speedy, int directionX);

	void hurt(int damage);

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};
