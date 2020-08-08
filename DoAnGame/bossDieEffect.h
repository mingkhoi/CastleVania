#pragma once
#include "./gameObject.h"
#include "./bossBall.h"
#include "./camera.h"
#include "./gamePlay.h"

#define endEffect 500

class bossDieEffect : public gameObject	
{
public:
	DWORD startCount;

	bossDieEffect(int id) : gameObject(id) {
		this->assignAnimationSet("boss_die");
		this->animation.back()->reset();

		this->bbox_w = this->bbox_h = 0;

		startCount = GetTickCount64();
	}

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {
	}

	virtual void setState(int) {}

	virtual void update(float dt) {
		if (animation.back()->isDone()) {
			float x = camera::getInstance()->getX() + (wndWidth / 2);
			float y = camera::getInstance()->getY() + (wndHeight / 2);

			bossBall* endGameBall = new bossBall(Counter::index++);
			endGameBall->setPos(x, y);
			gamePlay::getInstance()->objects.push_back(endGameBall);

			isDestroyed = true;
		}
	}

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt) {
	}

	virtual void render() {
		this->animation.back()->render(pos_x, pos_y, Left, 255);
	}
};

