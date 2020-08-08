#pragma once
#include "./gameObjectEx.h"

#define movePaddle_speedX 30.0f
#define movePaddle_speedY 0.0f

class movePaddle : public gameObjectEx
{
public:
	movePaddle(int id);

	void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	void setState(int);

	void update(float dt);

	void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	void render();
};