#pragma once
#include "gameObjectEx.h"

#define whip_pad_height_right 7.0f
#define whip_pad_height_left 8.0f
#define whip_pad_width 5.5f
#define whip_hero_start_state_attack 10

#define whip_lv1_length 24
#define whip_lv2_length 24
#define whip_lv3_length 40

class whip : public gameObjectEx
{
private:
	// hero to hold the weapon s
	LPGAMEOBJECT hero;

	LPSPRITE currentSprite;
	LPANIMATION currentAni;

	int currentLevel;

	bool stopCollision;
	bool stopRender;
	bool isSpawn;
	bool stopWhip;

	int damage;

public:
	whip(int id);

	void setHero(LPGAMEOBJECT);

	void spawn(int level, int directionX);

	void pullBack();

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};
