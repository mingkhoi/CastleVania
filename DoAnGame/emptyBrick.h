#pragma once
#include "./gameObject.h"
#include "./gameObjectEx.h"
#include "./gamePlay.h"
#include "./listGameobject.h"
#include "./userData.h"

class emptyBrick : public gameObjectEx, public userData
{
public:
	string itemName;

	emptyBrick(int id) : gameObjectEx(id) {
		this->assignAnimationSet("empty_brick");
		this->tag = "ground";
		this->isRender = false;

		aniState = 0;
	}

	virtual void loadData(xml_n* objectNode) {
		this->type = "empty";

		xml_n* spawnNode = objectNode->first_node("spawnObject"); 

		this->itemName = (string)get_atr(spawnNode, "name");
	}

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {
	}

	virtual void setState(int) {
	}

	virtual void update(float dt) {

	}

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt) {

	}

	void spawnObject() {
		if (itemName == "")
			return;

		LPGAMEOBJECT spawn = listGameobject::getInstance()->reflect(itemName); 
		spawn->setPos(this->pos_x, this->pos_y);

		gamePlay::getInstance()->objects.push_back(spawn); 
	}

	void setEmpty() {
		this->isRender = true;
		this->canTouch = false;

		this->spawnBrokenBrickEffect(); 
	}

	virtual void render() {
		if (isRender) {
			this->animation[aniState]->render(pos_x, pos_y, Left, 255);
		}
	}
};

