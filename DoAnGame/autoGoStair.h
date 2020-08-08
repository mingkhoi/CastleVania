#pragma once
#include "./gameObjectEx.h"
#include "./userData.h"

#define autoGoStair_nullFloat -1.07374e+08

class autoGoStair : public gameObjectEx, public userData
{
public:
	autoGoStair(int id);

	void loadData(xml_n* objNode);

	bool hasDetectedUpStair = false;
	bool hasDetectedDownStair = false;

	float toXUpStair = autoGoStair_nullFloat;
	float toXDownStair = autoGoStair_nullFloat;

	vector<LPGAMEOBJECT> stairList;

	LPGAMEOBJECT getUpStair(); 
	LPGAMEOBJECT getDownStair(); 

	bool checkUpStair();
	bool checkDownStair();

	float getPosUpStair();
	float getPosDownStair();

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);

	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};