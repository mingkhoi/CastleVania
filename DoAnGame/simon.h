#pragma once
#pragma once
#include "./game.h"
#include "./gameObjectEx.h"
#include "./portal.h"
#include "./subWeaponSpawner.h"
#include "./whip.h"

using namespace std;

#define simon_state_null -999
#define simon_state_intro -2
#define simon_state_idle_stair -1
#define simon_state_walk 0
#define simon_state_idle 1
#define simon_state_jump 2
#define simon_state_duck 3
#define simon_state_hurt 4
#define simon_state_dead 5
#define simon_state_ascend 6
#define simon_state_descend 7
#define simon_state_auto_walk 8
#define simon_state_duck_attack 10
#define simon_state_attack 11
#define simon_state_ascend_attack 12
#define simon_state_descend_attack 13
#define simon_state_start_ascend 14
#define simon_state_idle_ascend 15
#define simon_state_idle_descend 16
#define simon_state_detect_stair 17


#define simon_out_screen 500

#define simon_ani_intro 0
#define simon_ani_idle 1
#define simon_ani_walk 2
#define simon_ani_duck 3
#define simon_ani_ascend 4
#define simon_ani_descend 5
#define simon_ani_hurt 6
#define simon_ani_dead 7
#define simon_ani_jump 8
#define simon_ani_attack 9
#define simon_ani_duck_attack 10
#define simon_ani_ascend_attack 11
#define simon_ani_descend_attack 12

#define simon_gravity 650.0f
#define simon_move_speed 60.0f
#define simon_jump_speed 220.0f

#define simon_cannot_touch_time 3000
#define simon_stair_speedX 35.0f
#define simon_stair_speedY 35.0f
#define simon_hurt_speedX 25.0f
#define simon_hurt_speedY 200.0f
#define simon_landing_speedY 300.0f
#define simon_transparent 150
#define simon_normal_color 255
#define simon_hurt_time 2500
#define simon_max_hurt_time 5000
#define simon_landing_time 500

#define simon_normal_width 16
#define simon_normal_height 29.5f
#define simon_duck_height 23
#define simon_death_height_1 21
#define simon_death_height_2 15
#define simon_start_whip_level 1
#define simon_up_stair_force surfaceNormalForce * 8

#define simon_stair_size 8
#define simon_stair_up 0
#define simon_stair_down 1
#define simon_stair_null -1

#define simon_max_whip_level 3

#define simon_small_heart 1
#define simon_big_heart 10
#define simon_chicken 5

class simon : public gameObjectEx
{
public:
	int hearts;
	int health;
	int scores;
	int lives;

public:
	// short term flag

	bool isMove = false;
	bool isMoveStair = false;
	bool isJump = false;
	bool isArrowUp = false;
	bool isArrowDown = false;

	// long term flag

	bool cannotTouch = false;
	bool lockLanding = true;
	bool isHurted = false;
	bool isLanding = false;
	bool isAttack = false;
	bool isClimbing = false;
	bool fallFromHighGround = false;
	bool nextStairMove = false;
	bool hasMapPoint = false;

	bool autoStairWalk = false;
	bool hasEnterAutoStair = false;

	bool sceneTransition = false;

	// -------------------------

	float jumpSpeedX;

	float nextStairX, nextStairY;
	float touchStairX; 

	int whipLv;
	int stairMode;

	string subWeaponName;

	subWeaponSpawner* weaponSpawner;
	LPGAMEOBJECT portalStair;
	LPGAMEOBJECT whips;
	LPGAMEOBJECT subWeapon;
	LPGAMEOBJECT item;

	DWORD hurtStartTime;
	DWORD landingStartTime;
	DWORD dieStartTime;
	DWORD cannotTouchStartTime;

	simon(int id);

	void useWhip();
	void useSubWeapon();

	// simon hurt
	// call from other gameobject only

	void startCantTouch();
	bool hurt(float nx, float ny, int damage);

	void resetState();
	void changeScene();

	void detectStair();
	void standUp();
	void duck();
	void walk(int direction);
	void climbingStair(int direction, int upDown);

	void autoGoStairX(float endX);

	void stairProcess();

	bool checkStair();

	void simonMovementHandle(const BYTE* keyState,
		const BYTE* keyEvent);
	void landingProcess();

	// virual function overload from base class

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};
