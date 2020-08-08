#include "./subWeaponSpawner.h"
#include "./animationSet.h"
#include "./axe.h"
#include "./gamePlay.h"
#include "./listGameobject.h"
#include "./simon.h"
#include "./SoundManager.h"
#include "knife.h"

#define item_attack_state 1

subWeaponSpawner::subWeaponSpawner() {
	isSpawn = false;
}

void subWeaponSpawner::setHero(LPGAMEOBJECT val) {
	this->hero = val;
}

void subWeaponSpawner::spawn(string object, int directionX) {
	this->hero = gamePlay::getInstance()->player;

	this->directionX = directionX;

	this->objectName = object;

	if (cast(this->hero, simon*)->item != NULL) {
		if (cast(this->hero, simon*)->item->tag == "doubleShot") {
			this->weaponSpawnEach = subWeapon_doubleshot;
		}
		else if (cast(this->hero, simon*)->item->tag == "tripleShot") {
			this->weaponSpawnEach = subWeapon_tripleshot;
		}
		else this->weaponSpawnEach = subWeapon_singleshot;
	}
	else this->weaponSpawnEach = subWeapon_singleshot;

	if (cast(this->hero, simon*)->subWeaponName == "timeMachine") {
		this->weaponSpawnEach = subWeapon_singleshot;
	}
}

void subWeaponSpawner::shoot() {
	SoundManager::GetInstance()->playSoundEffect("throw");

	spawnObject = listGameobject::getInstance()->reflect(this->objectName);
	spawnObject->tag = this->objectName;
	spawnObject->type = "subweapon";

	float temp_y = hero->getY() + whip_pad_height_right;
	if (directionX == Left) {
		spawnObject->setPos(hero->getX() - spawnObject->getWidth(), temp_y);
	}
	else if (directionX == Right) {
		spawnObject->setPos(hero->getX() + hero->getWidth() + whip_pad_width, temp_y);
	}
	spawnObject->setDirecttionX(this->directionX);
	spawnObject->setState(item_attack_state);

	cast(hero, simon*)->hearts -= 1;

	gamePlay::getInstance()->objects.push_back(spawnObject);
}

void subWeaponSpawner::pullBack() {
	this->weaponSpawnEach = 0;
}

void subWeaponSpawner::update() {
	if (this->weaponSpawnEach <= 0) 
		return; 
	
	if (hero->getCurrentAnimation()->remainFrame() == 1) {
		this->shoot();

		this->weaponSpawnEach--;

		if (this->weaponSpawnEach > 0) {
			hero->getCurrentAnimation()->reset();
		}
	}
}