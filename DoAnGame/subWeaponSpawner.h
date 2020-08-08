#pragma once
#include "./gameObject.h"

#define whip_pad_height_right 7.0f
#define whip_pad_width 5.5f
#define subWeapon_pad 5
#define subWeapon_singleshot 1
#define subWeapon_doubleshot 2
#define subWeapon_tripleshot 3

using namespace std;

class subWeaponSpawner
{
protected:
	LPGAMEOBJECT hero;
	int directionX;
	bool isSpawn;

	string objectName;
	int weaponSpawnEach; 
public:
	LPGAMEOBJECT spawnObject;

	subWeaponSpawner();
	void setHero(LPGAMEOBJECT val);
	void spawn(string object, int directionX);

	void shoot();
	void update();

	void pullBack();
};
