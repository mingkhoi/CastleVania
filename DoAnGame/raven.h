#pragma once
#include "./gameObjectEx.h"

#define raven_speed 60.0f
#define raven_distance 64
#define raven_distance_to_simon 32
#define raven_wait 500
#define raven_detect_simon 125
#define raven_health 1
#define raven_damage 3

class raven : public gameObjectEx
{
public:
	bool hasDetected = false;
	bool isWait = false;
	bool hasChoosePoint = false;
	bool hasMapPoint = false;
	bool hasDecideAttack = false;

	float directionMoveX;

	int health;

	D3DXVECTOR2 destinationPoint;
	D3DXVECTOR2 endAttackPoint;
	DWORD startWaitTime;

	raven(int id);

	void hurt(int damage);


	bool isAttackHorizon();
	bool isAttackTracking();

	void observeSimon();

	void detectSimon();	

	void flyToPoint();
	void attack();

	void decideAttack();

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};

