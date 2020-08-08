#pragma once
#include "./gameObjectEx.h"
#include "./database.h"
#include "./camera.h"

#define brokenBrick_gravity 650.0f


class brokenBrick : public gameObjectEx
{
public:
	brokenBrick(int id) : gameObjectEx(id) {
		this->assignAnimationSet("broken_brick");
		this->setSizeItem("brokenBrick");

		this->aniState = 0;

		this->canTouch = false;
	}

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {

	}

	virtual void setState(int) {

	}

	virtual void update(float dt) {
		this->speed_y += brokenBrick_gravity * dt;
		this->updateSweptAABB(dt);

		camera* cam = camera::getInstance();

		float screen_x = pos_x - cam->getX();
		float screen_y = pos_y - cam->getY();

		if (screen_x + this->bbox_w < 0 || screen_x > wndWidth ||
			screen_y + this->bbox_h < 0 || screen_y > wndHeight) {
			this->isDestroyed = true;
		}
	}

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt) {
		this->removeCollisionBarrier(collisionEvent, dt);
	}


	virtual void render() {
		this->animation[aniState]->render(pos_x, pos_y, Left, 255);
	}
};

