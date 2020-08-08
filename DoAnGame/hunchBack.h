#pragma once
#include "./gameObjectEx.h"

#define hunchBack_state_null -99
#define hunchBack_state_hop 0
#define hunchBack_state_idle 2
#define hunchBack_state_prepare 3
#define hunchBack_state_dodge 4
#define hunchBack_state_attack 5
#define hunchBack_hop_padY 0.5f

#define hunchBack_distance_attackX 50
#define huncBack_distance_detect_simon 48

#define hunchBack_ani_idle 0
#define hunchBack_ani_hop 1
#define hunchBack_ani_prepare 2

#define hunchBack_gravity 650.0f
#define hunchBack_hop_force_y 80.0f
#define hunchBack_hop_force_x 180.0f
#define hunchBack_attack_force_y 80.0f
#define hunchBack_attack_force_x 400.0f
#define hunchBack_hop_dodge_force_y 250.0f
#define hunchBack_hop_dodge_force_x 80.0f
#define hunchBack_runaway_distance 32

#define hunchBack_below_simon 50
#define hunchBack_damage 2
#define hunchBack_health 2

#define hunchBack_normal_hop 0
#define hunchBack_hop_dodge 1
#define hunchBack_attack 2

#define hunchBack_stop_time 200
#define hunchBack_prepare_time 1200

class hunchBack : public gameObjectEx
{
public:
	hunchBack(int id);

	DWORD stopTimeStart;
	DWORD prepareTimeStart;

	int health;

	float wallEnd;

	bool isStop = false;
	bool runAway = false;

	bool startCLimbing = false;
	bool isGravity = true;

	float deltaTime;

	int checkAttack(float dt);

	void hurt(int damage);
	void observeSimon();

	float distanceXTo(LPGAMEOBJECT obj);
	bool checkOtherSide(LPGAMEOBJECT obj);

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};