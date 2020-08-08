#pragma once
#include "./gameObjectEx.h"
#include "./userData.h"

class candle : public gameObjectEx, public userData
{
public:
	candle(int);	

	string spawnObjectName;

	void loadData(xml_n* objNode);

	void spawnObject();

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent);

	virtual void setState(int);
	
	virtual void update(float dt);

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt);

	virtual void render();
};

