#pragma once
#include "./gameObjectEx.h"

using namespace std;

#define bossBullet_damage 2


class BossBullet : public gameObjectEx
{
public:
	int health = 1;

	BossBullet(int id);

	virtual void hurt(int damage);

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt); 

	virtual void render();
};

