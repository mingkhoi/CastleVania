#include "./autoGoStair.h"

autoGoStair::autoGoStair(int id) : gameObjectEx(id) {
	this->setSpeed(0, 0);
	this->type = "portal";
}

void autoGoStair::loadData(xml_n* objectNode) {
	this->isStaticObject = false;
}

void autoGoStair::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {
	
}

void autoGoStair::setState(int) {

}

void autoGoStair::update(float dt) {
	this->updateAABB(dt);
}

LPGAMEOBJECT autoGoStair::getDownStair() {
	for (auto& x : this->stairList)
		if (x->tag == "down stair") {
			return x;
		}
	return NULL;
}

LPGAMEOBJECT autoGoStair::getUpStair() {
	for (auto& x : this->stairList)
		if (x->tag == "up stair") {
			return x;
		}
	return NULL;
}

bool autoGoStair::checkUpStair() {
	for (auto &x : this->stairList)
		if (x->tag == "up stair") {
			return true;
		}
	return false;
}

bool autoGoStair::checkDownStair() {
	for (auto& x : this->stairList)
		if (x->tag == "down stair") {
			return true;
		}
	return false;
}

float autoGoStair::getPosUpStair() {
	return this->toXUpStair;
}

float autoGoStair::getPosDownStair() {
	return this->toXDownStair;
}

void autoGoStair::collisionHandle(LPCOLLISION collisionEvent, float dt) {
	LPGAMEOBJECT obj = collisionEvent->obj;

	if (obj->tag == "up stair" && !hasDetectedUpStair) {
		this->stairList.push_back(obj);
		this->toXUpStair = obj->getX();

		hasDetectedUpStair = true;
	}
	else if (obj->tag == "down stair" && !hasDetectedDownStair) {
		this->stairList.push_back(obj);
		this->toXDownStair = obj->getX();

		hasDetectedDownStair = true;
	}
}


void autoGoStair::render() {

}