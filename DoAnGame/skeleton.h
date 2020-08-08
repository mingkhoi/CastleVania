#pragma once
#include "./gameObjectEx.h"

#define skeleton_speedX 55.0f
#define skeleton_speedY 250.0f
#define skeleton_throw_speed_x 120.0f
#define skeleton_throw_speed_y 250.0f
#define skeleton_jump_height_pad 15

#define skeleton_outside_screen 50

#define skeleton_health 2
#define skeleton_damage 1

#define skeleton_collision_point_pad 2.0f

#define skeleton_gravity 650.f

#define skeleton_numpoint_rand 4
#define skeleton_numpoint_distance 5

#define skeleton_special_jump_divide 2
#define skeleton_special_jump_limit 75
#define skeleton_special_jump_too_small 10
#define skeleton_special_jump_too_small_speedX 15
#define skeleton_special_jump_too_small_speedY -180

#define skeleton_in_areaY 33
#define skeleton_in_areaX 75
#define skeleton_boudary_null -123
#define skeleton_ground_height 20
#define skeleton_direction_null 0
#define skeleton_rand_time 350

#define sheleton_shoot_time_each 300
#define skeleton_shoot_time 1500
#define skeleton_shoot_count_each 2

#define skeleton_normal_jump 0
#define skeleton_special_jump 1
#define skeleton_touch_border 2
#define skeleton_do_nothing 3

class skeleton : public gameObjectEx
{
public:
	skeleton(int id);

	DWORD shootTime;
	DWORD shootTimeEach;
	DWORD randPointTime;

	bool isRand = true;

	float chaseX;
	float speedJumpX;
	float limitX;
	float dt;

	int shootCount;
	int directionMoveX;
	int health;
	int limitXDirection;

	void observeSimon();

	LPGAMEOBJECT isGround(float x, float y);

	int observerAround();
	float distanceXTo(LPGAMEOBJECT obj);

	void doSpecialJump();

	void shoot();
	void spawnBullet();

	void moveToPointX(float x);
	void chaseEnemy();

	void hurt(int damage);

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};