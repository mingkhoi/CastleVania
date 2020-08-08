#include "./hunchBack.h"
#include "./camera.h"
#include "./gamePlay.h"
#include "./simon.h"
#include "./whip.h"

hunchBack::hunchBack(int id)
	: gameObjectEx(id) {
	this->type = "enemy";
	this->assignAnimationSet("hunch_back");
	this->health = hunchBack_health;

	setState(hunchBack_state_prepare);
	prepareTimeStart = GetTickCount64();
}

void hunchBack::hurt(int damage) {
	this->health -= damage;

	if (this->health <= 0) {
		this->spawnDieEffect();

		isDestroyed = true;
	}
	this->spawnHitEffect();
}

void hunchBack::observeSimon() {
	if (!onTheGround)
		return;

	LPGAMEOBJECT target = gamePlay::getInstance()->player;

	if (runAway) {
		bool atkCondition1 = (target->getDirectionX() == this->directionX && std::abs(target->getSpeedX()) > 0);
		bool atkCondition2 = (target->getDirectionX() == this->directionX && cast(target, simon*)->isAttack);

		if (distanceXTo(target) >= hunchBack_runaway_distance && (atkCondition1 || atkCondition2)) {
			runAway = false;
		}

		if (runAway)
			return;
	}

	D3DXVECTOR2 thisVector, targetVector;

	if (target->getX() + target->getWidth() >= this->pos_x)
		directionX = Right;
	else if (target->getX() <= this->pos_x + this->bbox_w)
		directionX = Left;
}

bool hunchBack::checkOtherSide(LPGAMEOBJECT obj) {
	bool condition1 = (obj->getX() > this->pos_x + this->bbox_w);
	bool condition2 = (obj->getX() + obj->getWidth() < this->pos_x);

	return (condition1 || condition2);
}

float hunchBack::distanceXTo(LPGAMEOBJECT obj) {
	float distance = 0.0f;

	if (obj->getX() > this->pos_x + this->bbox_w) {
		distance = std::abs(obj->getX() - (this->pos_x + this->bbox_w));
	}
	else if (obj->getX() + obj->getWidth() < this->pos_x) {
		distance = std::abs(this->pos_x - (obj->getX() + obj->getWidth()));
	}
	else
		distance = 0.0f;

	return distance;
}

void hunchBack::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {

	if (startCLimbing == true) {
		if (pos_y + bbox_h < wallEnd)
			speed_y = 0;
		else speed_y = -hunchBack_hop_force_y * 2;

		if (onTheGround)
			startCLimbing = false;

		speed_x = 0;

		state = hunchBack_state_hop;
		return;
	}

	if (state == hunchBack_state_prepare) {
		if (GetTickCount64() - prepareTimeStart >= hunchBack_prepare_time)
			setState(hunchBack_state_hop);
		else
			return;
	}

	observeSimon();

	if (onTheGround == true) {
		if (state == hunchBack_state_hop || state == hunchBack_state_dodge) {
			stopTimeStart = GetTickCount64();
			isStop = true;
		}
		if (isStop == true) {
			if (GetTickCount64() - stopTimeStart >= hunchBack_stop_time) {
				isStop = false;

				int sign = checkAttack(this->deltaTime);

				switch (sign) {
				case hunchBack_hop_dodge:
					setState(hunchBack_state_dodge);
					break;
				case hunchBack_normal_hop:
					setState(hunchBack_state_hop);
					break;
				}
			}
			else
				this->setState(hunchBack_state_idle);
		}
	}
}

void hunchBack::setState(int value) {
	state = value;

	switch (value) {
	case hunchBack_state_dodge:
		speed_x = hunchBack_hop_dodge_force_x * directionX;
		speed_y = -hunchBack_hop_dodge_force_y;

		aniState = hunchBack_ani_hop;
		break;
	case hunchBack_state_hop:
		speed_x = hunchBack_hop_force_x * directionX;
		speed_y = -hunchBack_hop_force_y;

		aniState = hunchBack_ani_hop;
		break;
	case hunchBack_state_attack:
		speed_x = hunchBack_attack_force_x;
		speed_y = hunchBack_attack_force_y;

		aniState = hunchBack_ani_hop;
		break;
	case hunchBack_state_idle:
		speed_y = speed_x = 0;

		aniState = hunchBack_ani_idle;
		break;

	case hunchBack_state_prepare:
		speed_y = speed_x = 0;

		aniState = hunchBack_ani_prepare;
		break;
	}
}

int hunchBack::checkAttack(float dt) {
	LPGAMEOBJECT target = gamePlay::getInstance()->player;

	float whipLength = 0.0f;

	if (cast(target, simon*)->whipLv > 2)
		whipLength = whip_lv3_length;
	else
		whipLength = whip_lv1_length;

	if (cast(target, simon*)->isAttack && checkOtherSide(target)) {
		if (this->directionX == Right && target->getDirectionX() == Left) {
			if (this->pos_x + this->bbox_w >= target->getX() - whipLength)
				return hunchBack_hop_dodge;
		}
		else if (this->directionX == Left && target->getDirectionX() == Right) {
			if (this->pos_x <= target->getX() + target->getWidth() + whipLength + whip_pad_width)
				return hunchBack_hop_dodge;
		}
	}
	else if (!cast(target, simon*)->isAttack) {
		if (std::abs(target->getSpeedX()) > 0 && checkOtherSide(target)
			&& target->getDirectionX() != this->directionX) {
			if (distanceXTo(target) <= hunchBack_distance_attackX)
				return hunchBack_hop_dodge;
		}
	}
	return hunchBack_normal_hop;
}

void hunchBack::update(float dt) {
	deltaTime = dt;

	speed_y += hunchBack_gravity * dt;
	
	vector<string> AABBTag = { "torch", "candle", "flame" };
	vector<string> AABBType = { "enemy", "item", "portal", "subweapon" };

	this->updateSweptFilterAABB(game::makeFilterCollision(AABBTag),
		game::makeFilterCollision(AABBType), dt);

	camera* cam = camera::getInstance();

	float screen_x = this->pos_x - cam->getX();
	float screen_y = this->pos_y - cam->getY();

	if (screen_x + bbox_w < 0 || screen_x > wndWidth || screen_y + bbox_h < 0 || screen_y > wndHeight) {
		isDestroyed = true;
	}
}

void hunchBack::collisionHandle(LPCOLLISION colEvent,
	float dt) {
	LPGAMEOBJECT obj = colEvent->obj;

	if (obj->tag == "simon") {
		cast(obj, simon*)->hurt(colEvent->nx, colEvent->ny, hunchBack_damage);

		this->removeCollisionBarrier(colEvent, dt);
		runAway = true;
	}
	if (obj->type == "enemy")
		this->removeCollisionBarrier(colEvent, dt);

	if (obj->tag == "ground") {
		if (colEvent->ny == 1 && colEvent->nx == 0) {
			this->removeCollisionBarrier(colEvent, dt);
		}
		if (colEvent->nx != 0 && colEvent->ny == 0) {
			if ((obj->getX() + obj->getWidth() <= this->pos_x && directionX == Left)
				|| obj->getX() >= this->pos_x + this->bbox_w && directionX == Right) {

				this->removeCollisionBarrier(colEvent, dt);

				pos_x += collisionOldSpeedX * dt;
				wallEnd = obj->getY();

				startCLimbing = true;
			}
		}
	}
}

void hunchBack::render() {
	animation[aniState]->render(pos_x, pos_y, directionX, 255);
}