#pragma once
#include "./gameObject.h"

#define surfaceNormalForce 0.4f
#define groundPad 0.5f
#define Px_too_small 5


class gameObjectEx : public gameObject
{
public:
	LPGAMEOBJECT standingGround = NULL;

protected:
	gameObjectEx(int id);

	bool onTheGround;
	bool checkCollisionBorderX;
	bool checkCollisionBorderY;

	void spawnHitEffect();
	void spawnDieEffect();
	void spawnBrokenBrickEffect();
	void spawnPoint(int point); 

	void setSizeItem(string itemName);

	void groundHandle();

	D3DXVECTOR2 calcaulateLaunchForce(D3DXVECTOR2 target, float gravity, float jumpHeight, 
		float dt,bool reverseGravity);

	// supported collision detection method
	// collision information will be pass into "collisionHandle" function

	void updateSweptAABB(float dt);
	void updateAABB(float dt);

	void updateSweptFilterAABB(std::unordered_map<string, bool> filterTag,
		std::unordered_map<string, bool> filterType, float dt);

	void performCollisionBarrier(float dt);
	void removeCollisionX(LPCOLLISION col, float dt);
	void removeCollisionY(LPCOLLISION col, float dt);
	void removeCollisionBarrier(LPCOLLISION col, float dt);
};