#include "./zombieSpawner.h"
#include "./gamePlay.h"
#include "./zombie.h"
#include "./camera.h"

zombieSpawner::zombieSpawner(int id) : gameObjectEx(id) {
	this->type = "portal";
}

void zombieSpawner::loadData(xml_n* objNode) {
	xml_n* spawnProperty = objNode->first_node("spawnProperty");

	this->number = Numeric::toInt(get_atr(spawnProperty, "number"));
	this->spawnWidth = Numeric::toFloat(get_atr(spawnProperty, "width"));
	this->spawnHeight = Numeric::toFloat(get_atr(spawnProperty, "height"));

	this->isStaticObject = false;
}

void zombieSpawner::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {

}

void zombieSpawner::spawnZombie() {

	if (GetTickCount64() - this->timeSpawn >= zombieSpawner_time_spawn) {
		if (GetTickCount64() - this->timePerSpawn >= zombieSpawner_time_per_spawn) {

			this->count++;
			if (this->count == this->number) {
				this->timeSpawn = GetTickCount64();
				this->count = 0;
			}
			this->timePerSpawn = GetTickCount64();

			zombie* newObj = new zombie(Counter::index++);
			newObj->setSize(this->spawnWidth, this->spawnHeight);
			newObj->setDirecttionX(this->directionX);

			if (this->directionX == Right) {
				newObj->setX(camera::getInstance()->getX());
			}
			else if (this->directionX == Left) {
				newObj->setX(camera::getInstance()->getX() + wndWidth -
					newObj->getWidth());
			}
			newObj->setY(this->pos_y);

			gamePlay::getInstance()->objects.push_back(newObj);
		}
	}
}

void zombieSpawner::setState(int value) {

}

void zombieSpawner::update(float dt) {
	LPGAMEOBJECT target = gamePlay::getInstance()->player;

	if (target == NULL || camera::getInstance()->autoPan) return;

	if (target->getX() + target->getWidth() >= this->pos_x) {
		directionX = Right;
	}
	else if (target->getX() <= this->pos_x + this->bbox_w) {
		directionX = Left;
	}

	this->spawnZombie();
}

void zombieSpawner::collisionHandle(LPCOLLISION collisionEvent,
	float dt) {

}

void zombieSpawner::render() {

}