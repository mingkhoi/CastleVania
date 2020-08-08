#pragma once
#include "./gameObjectEx.h"

#define boss_detectSimon 25
#define boss_waitBeforeStart 800
#define boss_damage 2
#define boss_defaultHealth 16

#define boss_shoot 2
#define boss_attack 1

#define boss_sizePad 5

#define boss_hurtTime 200
#define boss_idle 800

#define boss_defaultSpeedX 60
#define boss_defaultSpeedY 60
#define boss_attackSpeedX 110
#define boss_attackSpeedY 70
#define boss_returnSpeedX 100
#define boss_returnSpeedY 520
#define boss_attackForceY 2.7f
#define boss_stopAttackSpeedY 150

#define boss_divisionBeforeAttack 2

#define boss_targetAttackPadX 100
#define boss_targetAttackPadY 2

#define boss_reverseGravity 180
#define boss_gravity 50

#define boss_distanceBackwardX 280
#define boss_homeY 55

#define boss_bulletSpeed 80 
#define boss_targetShootPadY 5
#define boss_targetShootPadX 5

#define boss_bulletPad 10
#define boss_bulletDistance 80		

#define boss_ani_idle 0
#define boss_ani_fly 1

using namespace std;

// no gravity

class boss : public gameObjectEx
{
public:
	int health;

	bool isHurted = false;
	bool waitForSimon = true;
	bool detectSimon = false;

	bool hasShoot = false;

	bool canFlyTowardTarget = false;
	bool canFlyHome = false;
	bool canShoot = false;

	bool isAttacking = false;

	bool isCalculatedPathToTarget = false;
	bool isCalculatedPathToHome = false;

	bool goToAttackIdlePoint = false;
	bool goToTargetPoint = false;
	bool goToHomePoint = false;
	bool isTouchAttackIdle = false;
	bool isTouchTargerEnd = false;
	bool isTouchHomePoint = false;

	bool isHome = false;

	int attackMode = -1;

	D3DXVECTOR2 attackIdlePoint;
	D3DXVECTOR2 attackTargetPoint; 
	D3DXVECTOR2 homePoint;

	DWORD startHurtTime;	
	DWORD shootTime;
	DWORD waitBeforeStartTime;
	DWORD touchAttackIdlePointTime;
	DWORD touchHomePoint;
	DWORD homeIdleTime;

	boss(int id);

	D3DXVECTOR2 bulletRootPos(); 
	void shoot();

	void flyTowardTarget(float dt);
	void flyTowardHome(float dt);

	void waitSimonHandle();
	void mainHandle(float dt); 
	void attackHandle(float dt);
	void returnHomeHandle(float dt);
	void shootHandle(); 

	virtual FRect getBoundingBox();

	virtual void hurt(int damage);
	void hurtHandle(); 

	void prepareAttack();

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};

