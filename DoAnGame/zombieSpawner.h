#pragma once
#include "./gameObjectEx.h"
#include "./userData.h"

#define zombieSpawner_time_per_spawn 500
#define zombieSpawner_time_spawn 5000


class zombieSpawner : public gameObjectEx, public userData
{
public:
	zombieSpawner(int id);

	int number;
	int count;

	float spawnWidth;
	float spawnHeight;

	DWORD timePerSpawn;
	DWORD timeSpawn;

	void loadData(xml_n* objNode);

	void spawnZombie();

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};

