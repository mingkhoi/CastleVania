#include "./holyWater.h"
#include "./camera.h"
#include "./torch.h"
#include "./candle.h"
#include "./SoundManager.h"

holyWater::holyWater(int id) : gameObjectEx(id) {
	this->assignAnimationSet("holy_water");

	boundingMode = normalBounding;

	this->setSizeItem("holyWater"); 

	startDie = GetTickCount64();
}

FRect holyWater::getBoundingBox() {
	FRect res;

	if (boundingMode == burnBounding) {
		res.left = this->pos_x - holy_water_burnWidthSide;
		res.right = this->pos_x + this->bbox_w + holy_water_burnWidthSide;
		res.top = this->pos_y;
		res.bottom = this->pos_y + this->bbox_h;
	}
	else if (boundingMode == normalBounding) {
		res.left = this->pos_x;
		res.top = this->pos_y;
		res.right = this->pos_x + this->bbox_w;
		res.bottom = this->pos_y + this->bbox_h;
	}

	return res;
}

void holyWater::setState(int value) {
	this->state = value;

	switch (state) {
	case holy_water_state_item:
		speed_x = 0;
		aniState = holy_water_ani_item;

		bbox_w = animation[aniState]->getFrame(0)->getWidth();
		bbox_h = animation[aniState]->getFrame(0)->getHeight();

		break;

	case holy_water_state_attack:
		speed_x = holy_water_speedX * directionX;
		speed_y = holy_water_speedY;
		aniState = holy_water_ani_attack;

		bbox_w = animation[aniState]->getFrame(0)->getWidth();
		bbox_h = animation[aniState]->getFrame(0)->getHeight();

		break;

	case holy_water_state_burn:
		this->tag = "flame";
		boundingMode = burnBounding;

		bbox_w = holy_water_burnWidth;

		aniState = holy_water_ani_burn;
		break;
	}
}

void holyWater::update(float dt) {
	if (state == holy_water_state_attack)
		speed_x = holy_water_speedX * directionX;
	else if (state == holy_water_state_burn)
		speed_x = 0;

	speed_y += holy_water_gravity * dt;

	if (state == holy_water_state_item && (GetTickCount64() - startDie) >= item_die_time)
		isDestroyed = true;

	if (state == holy_water_state_burn && (GetTickCount64() - startBurn) > burnTime) {
		isDestroyed = true;
	}

	if (standingGround != NULL && standingGround->type == "moving ground"
		&& onTheGround && state == holy_water_state_item) {
		speed_x = standingGround->getSpeedX();
	}

	vector<string> AABBTag = { "simon" };
	vector<string> AABBType = { "enemy", "portal", "subweapon" };

	this->updateSweptFilterAABB(game::makeFilterCollision(AABBTag),
		game::makeFilterCollision(AABBType), dt);

	float screenX = pos_x - camera::getInstance()->getX();
	if (screenX < 0 || screenX > wndWidth)
		isDestroyed = true;
}

void holyWater::collisionHandle(LPCOLLISION colEvent,
	float dt) {
	LPGAMEOBJECT obj = colEvent->obj;

	if (obj->tag == "bossBall") {
		isDestroyed = true;
		return;
	}

	if (colEvent->tag != AABBCollision) {
		if (state == holy_water_state_attack) {
			if (obj->tag == "torch") {
				cast(obj, torch*)->spawnObject();
			}
			else if (obj->tag == "candle") {
				cast(obj, candle*)->spawnObject();
				obj->isDestroyed = true;
			}
			else if (obj->type == "item") {
				obj->isDestroyed = true;
			}
			SoundManager::GetInstance()->playSoundEffect("flame");
			startBurn = GetTickCount64();
			setState(holy_water_state_burn);
		}
	}
	else {
		if (state == holy_water_state_burn) {
			if (obj->type == "enemy") {
				obj->setSpeed(0, 0);
				obj->hurt(item_damage);
			}
		}
	}

	if (state == holy_water_state_burn) {
		if (obj->tag != "ground") {
			this->removeCollisionBarrier(colEvent, dt);
		}
	}
}

void holyWater::render() {
	this->animation[aniState]->render(pos_x, pos_y, Left, 255);
}