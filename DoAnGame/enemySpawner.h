#pragma once
#include "./gameObjectEx.h"
#include "./userData.h"

#define spawnEnemy_none -10
#define spawnEnemy_out_screen 0
#define spawnEnemy_in_screen 1
#define spawnEnemy_pad_spawn 50

class enemySpawner : public gameObjectEx, public userData
{
public:
	enemySpawner(int id);

	void loadData(xml_n* objNode);

	bool spawn_2_side;

	float spawn_x;
	float spawn_y;
	float spawn_w;
	float spawn_h;
	float spawn_speed_x;
	float spawn_speed_y;

	int spawnDirectionX;
	string spawnName;
	string spawnTag;

	bool firstTimeSpawn = true;
	bool simonTouch = false;

	int checkSide;

	void spawnObject();
	bool checkSpawn();

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};
