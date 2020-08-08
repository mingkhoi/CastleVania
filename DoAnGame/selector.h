#pragma once
#include "./gameObjectEx.h"

#define selector_yesX 102
#define selector_yesY 143
#define selector_noX 102
#define selector_noY 158

class selector : public gameObjectEx
{
public:
	int chooseIndex = 0;

public:
	selector(int id);

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};

