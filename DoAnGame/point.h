#pragma once
#include "./gameObject.h"

#define _10 0
#define _20 1
#define _40 2
#define _70 3

#define pointWidth 8

#define _0 4
#define _00 5

#define timeAppear 300

class point : public gameObject
{
public:
	DWORD startTime;

	int num;

	int first, second;

	point(int id)
		: gameObject(id) {
		this->bbox_h = this->bbox_h = 0;
		startTime = GetTickCount64();

		this->assignAnimationSet("point");

		print("[POINT] size animation=%s\n",
			to_string(this->animation.size()).c_str());
	}

	void setNum(int value) {
		num = value;

		if (num % 1000 == 0) {
			first = (num / 100) / 20;
			second = _00;
		}
		else if (num % 100 == 0) {
			first = (num / 10) / 20;
			second = _0;
		}
		else {
			first = num / 20;
			second = -1;
		}

		if (second != -1) {
			this->animation[second]->reset();
		}
		this->animation[first]->reset();
	}

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {
	}

	virtual void setState(int) {}

	virtual void update(float dt) {
		if (GetTickCount64() - startTime > timeAppear)
			isDestroyed = true;
	}

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt) {
	}

	virtual void render() {
		this->animation[first]->render(pos_x, pos_y, Left, 255);
		if (second != -1)
			this->animation[second]->render(pos_x + 8, pos_y, Left, 255);
	}
};
