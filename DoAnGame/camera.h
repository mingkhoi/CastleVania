#pragma once
#include "./define.h"
#include "./gameObject.h"

#define camera_nullLimit -9999999999
#define camera_smooth_speed_frame 10
#define camera_default_speed 700

class camera
{
protected:
	int endAutoPanX, endAutoPanY;

	int limitPositiveX, limitPositiveY;
	int limitNegativeX, limitNegativeY;

	float speedX, speedY;
	float autoPanSpeedX, autoPanSpeedY;
	float panSpeed;

	bool turnOnLimit = true;
	bool isTrackPoint = false;

	LPGAMEOBJECT master;

	static camera* Instance;

public:
	int x, y;

	bool isTouchLimit = false;
	bool autoPan = false;

	static camera* getInstance();

	camera();

	int getX() { return x; }
	int getY() { return y; }

	void setLimitPositiveX(int value) { limitPositiveX = value; }
	void setLimitPositiveY(int value) { limitPositiveY = value; }
	void setLimitNegativeX(int value) { limitNegativeX = value; }
	void setLimitNegativeY(int value) { limitNegativeY = value; }

	int getLimitPositiveX() { return limitPositiveX; }
	int getLimitPositiveY() { return limitPositiveY; }
	int getLimitNegativeX() { return limitNegativeX; }
	int getLimitNegativeY() { return limitNegativeY; }

	void setMaster(LPGAMEOBJECT master);
	void setPos(int x, int y);
	void setSpeedX(float val) { speedX = val; };
	void setSpeedY(float val) { speedY = val; };
	void setSpeed(float valX, float valY) {
		speedX = valX;
		speedY = valY;
	}


	void setAutoPan(float toX, float toY, float panSpeed);

	void update(float dt);
};