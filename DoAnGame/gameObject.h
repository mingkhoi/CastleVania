#pragma once
#include "./animation.h"
#include "./game.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

class gameObject;
typedef gameObject* LPGAMEOBJECT;

#define AABBCollision 0
#define SweptAABBCollision 1
#define collisionPad 12

class collision
{
public:
	LPGAMEOBJECT obj;
	float t, nx, ny;

	int tag; 

	collision(float t, float nx, float ny,
		LPGAMEOBJECT obj);

	bool operator<(const collision* other);
};
typedef collision* LPCOLLISION;

class gameObject
{
protected:
	bool gravity = true;

	float pos_x;
	float pos_y;

	float speed_x = 0;
	float speed_y = 0;

	float bbox_w, bbox_h;

	int directionX, directionY;

	float collisionMinTx, collisionMinTy,
		collisionNx, collisionNy;
	float collisionOldSpeedX, collisionOldSpeedY;

	D3DXMATRIX matrix;


public:
	std::vector<LPANIMATION> animation;

	int aniState;
	int state;

	bool isStaticObject = true;

	bool canTouch;
	bool isDestroyed;
	bool isRender;

	std::wstring tag_id;
	std::string tag;
	std::string type;

	gameObject(std::wstring tag_id);

	gameObject(int tag_id);

	gameObject(const LPGAMEOBJECT& other); 

	~gameObject();

	float getX();
	float getY();
	float getWidth();
	float getHeight();
	float getSpeedX();
	float getSpeedY();
	int getDirectionX();
	int getCurrentState();

	LPANIMATION getCurrentAnimation();
	LPANIMATION getAnimation(int id);
	int getCurrentAnimationState();
	vector<LPANIMATION> getAnimationSet();


	void setX(float value);
	void setY(float value);
	void setPos(float x, float y);
	void setSpeedX(float value);
	void setSpeedY(float value);
	void setSpeed(float x, float y);
	void setWidth(float value);
	void setHeight(float value);
	void setSize(float w, float h);
	void setDirecttionX(int value);

	void addAnimationSet(string name);

	void assignAnimationSet(string name);

	virtual void loadData(xml_n* objNode);

	virtual FRect getBoundingBox();

	LPCOLLISION sweptAABBEx(LPGAMEOBJECT other, float dt);

	void getCollisionSweptAABB(
		std::vector<LPCOLLISION>& collisionEvent, float dt);

	void getCollisionAABB(
		std::vector<LPCOLLISION>& collisionEvent, float dt);

	void filterCollision(std::vector<LPCOLLISION>& coEvent,
		std::vector<LPCOLLISION>& coResult,
		float& min_tx, float& min_ty,
		float& nx, float& ny);

	virtual void hurt(int damage);

	// user implementation

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent)
		= 0;

	virtual void setState(int) = 0;

	virtual void update(float dt) = 0;

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt)
		= 0;

	virtual void render() = 0;
};
typedef gameObject* LPGAMEOBJECT;