#pragma once
#include "./gameObjectEx.h"

#include <unordered_map>

#define blackKnight_padX 2
#define blackKnight_padY 2
#define blackKnight_turn_around 0
#define blackKnight_
#define blackKnight_boundary_null -123.0f

#define blackKnight_probability1 1.0f
#define blackKnight_probability2 4.5f
#define blackKnight_probability3 4.5f
#define blackKnight_rand_distance 80

#define blackKnight_damage 1
#define blackKnight_state_walking 0
#define blackKnight_speedX 30.0f
#define blackKnight_gravity 650.0f

#define blackKnight_brick_width 16
#define blackKnight_ground_pad_detect 2

#define blackKnight_detect_simon_y 35
#define blackKnight_detect_simon_distance 112

// probability when normal

#define blackKnight_always_full_boudary 1, 0
//---------------------

#define blackKnight_health 5
#define blackKnight_hit_time 200
#define blackKnight_random_time 200

class blackKnight : public gameObjectEx
{
public:
	int health;

	float leftBorder, rightBorder;
	float randDistance = 0;
	float distanceGo = 0;

	bool isDetectSimon;

	bool isHurted;
	bool isRandom;

	DWORD startHurtedTime;
	DWORD startRandomTime;

	blackKnight(int id);

	bool isGround(float x, float y);
	void detectBoundary();

	void detectSimon(float dt);

	void attackSimon();

	float distanceTo(LPGAMEOBJECT obj);
	void hurt(int damage);

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};