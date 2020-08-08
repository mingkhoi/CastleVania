#pragma once
#include "./gameObjectEx.h"
#include "./gamePlay.h"
#include "./listGameobject.h"
#include "./SoundManager.h"
#include "./userData.h"

class hidden : public gameObjectEx, public userData
{
public:
	string itemName; 
	float item_x, item_y;

	hidden(int id) : gameObjectEx(id) {
		this->type = "portal"; 
		this->tag = "hidden";
		
		this->setSizeItem("hidden"); 
	}

	virtual void loadData(xml_n* objectNode) {
		xml_n* spawnNode = objectNode->first_node("spawnObject");

		itemName = (string)get_atr(spawnNode, "name");
		item_x = Numeric::toFloat(get_atr(spawnNode, "x"));
		item_y = Numeric::toFloat(get_atr(spawnNode, "y"));
	}

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {
	}

	void spawnObject() {
		SoundManager::GetInstance()->playSoundEffect("treasure");

		LPGAMEOBJECT item = listGameobject::getInstance()->reflect(itemName);
		item->setPos(gamePlay::getInstance()->rootX + item_x, 
			gamePlay::getInstance()->rootY + item_y);

		gamePlay::getInstance()->objects.push_back(item);

		this->isDestroyed = true;
	}

	virtual void setState(int) {
	}

	virtual void update(float dt) {

	}

	virtual void collisionHandle(LPCOLLISION collisionEvent,
		float dt) {

	}

	virtual void render() {

	}
};

