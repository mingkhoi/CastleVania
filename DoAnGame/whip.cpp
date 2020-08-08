#include "./whip.h"
#include "./animationSet.h"
#include "./gamePlay.h"
#include "./simon.h"
#include "./torch.h"
#include "./candle.h"
#include "./SoundManager.h"
#include "./emptyBrick.h"

whip::whip(int id)
	: gameObjectEx(id) {
	this->addAnimationSet("whip_lv1");
	this->addAnimationSet("whip_lv2");
	this->addAnimationSet("whip_lv3");

	this->type = "whip";

	this->animation[0]->loop = false;
	this->animation[1]->loop = false;
	this->animation[2]->loop = false;

	stopRender = true;
	stopCollision = true;
	isSpawn = false;
}

void whip::setHero(LPGAMEOBJECT value) {
	this->hero = value;
}

void whip::spawn(int level, int directionX) {
	SoundManager::GetInstance()->playSoundEffect("whip");
	
	currentAni = hero->getCurrentAnimation();
	currentLevel = level;

	if (level == 1)
		this->damage = 1;
	else if (level >= 2)
		this->damage = 2;

	this->directionX = directionX;

	stopWhip = false;
	isSpawn = true;
	stopCollision = false;
	stopRender = false;
}

void whip::pullBack() {
	this->stopWhip = true;
	this->isSpawn = false;
	this->stopCollision = true;
	this->stopRender = true;

	bbox_w = bbox_h = 0;
}

void whip::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {
}

void whip::setState(int state) {
}

void whip::update(float dt) {
	if (currentLevel <= 0 || !this->isSpawn)
		return;

	FRect bboxHero = hero->getBoundingBox();

	if (currentAni->remainFrame() == 3) {
		currentSprite = this->animation[currentLevel - 1]->getFrame(0);

		if (directionX == Left) {
			pos_y = bboxHero.top + whip_pad_height_left;
			pos_x = bboxHero.right + whip_pad_width;
		}
		else if (directionX == Right) {
			pos_y = bboxHero.top + whip_pad_height_right;
			pos_x = bboxHero.left - currentSprite->getWidth();
		}
	}
	if (currentAni->remainFrame() == 2) {
		this->pos_y = bboxHero.top + whip_pad_height_right;
		currentSprite = this->animation[currentLevel - 1]->getFrame(1);

		if (directionX == Left) {
			pos_y = bboxHero.top + whip_pad_height_left;
			pos_x = bboxHero.right + whip_pad_width;
		}
		else if (directionX == Right) {
			pos_y = bboxHero.top + whip_pad_height_right;
			pos_x = bboxHero.left - currentSprite->getWidth();
		}
	}
	if (currentAni->remainFrame() == 1) {
		this->pos_y = bboxHero.top + whip_pad_height_right;

		currentSprite = this->animation[currentLevel - 1]->getFrame(2);

		if (directionX == Left) {
			pos_x = bboxHero.left - currentSprite->getWidth();
		}
		else if (directionX == Right)
			pos_x = bboxHero.right + whip_pad_width;

		bbox_w = currentSprite->getWidth();
		bbox_h = currentSprite->getHeight();

		// AABB collision

		if (this->stopCollision == false) {
			this->updateAABB(dt);
		}
	}
	if (stopWhip == true) {
		this->isSpawn = false;
		this->stopCollision = true;
		this->stopRender = true;

		bbox_w = bbox_h = 0;
	}
}

void whip::collisionHandle(LPCOLLISION colEvent,
	float dt) {
	if (stopCollision == true)
		return;

	LPGAMEOBJECT obj = colEvent->obj;

	bool isTouched = false;

	if (obj->tag == "bossBall") {
		stopCollision = true;
		bbox_w = bbox_h = 0;

		return;
	}

	if (obj->tag == "torch") {
		cast(obj, torch*)->spawnObject();

		isTouched = true;
	}
	else if (obj->tag == "candle") {
		cast(obj, candle*)->spawnObject();

		isTouched = true;
	}
	else if (obj->type == "item") {
		obj->isDestroyed = true;

		isTouched = true;
	}
	else if (obj->type == "enemy") {
		obj->hurt(this->damage);
		isTouched = true;
	}
	else if (obj->type == "empty") {
		cast(obj, emptyBrick*)->setEmpty();
		cast(obj, emptyBrick*)->spawnObject();

		isTouched = true;
	}

	if (isTouched) {
		stopCollision = true;
		bbox_w = bbox_h = 0;
	}
}

void whip::render() {
	if (!stopRender) {
		if (this->currentLevel <= 2) {
			currentSprite->draw(pos_x, pos_y, directionX,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		else {
			int r = 255;
			currentSprite->draw(pos_x, pos_y, directionX,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}