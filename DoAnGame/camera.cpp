#include "./camera.h"
#include "./game.h"
#include "./gamePlay.h"

camera* camera::Instance = NULL;

camera::camera() {
	this->x = this->y = 0;
	this->limitPositiveX = this->limitPositiveY = camera_nullLimit;
	this->limitNegativeX = this->limitNegativeY = camera_nullLimit;
}

camera* camera::getInstance() {
	if (Instance == NULL) {
		Instance = new camera();
	}
	return Instance;
}

void camera::setMaster(LPGAMEOBJECT master) {
	this->master = master;
}

void camera::setPos(int x, int y) {
	this->x = x;
	this->y = y;
}

void camera::setAutoPan(float toX, float toY, float panSpeed) {
	this->endAutoPanX = toX;
	this->endAutoPanY = toY;
	this->panSpeed = panSpeed;

	D3DXVECTOR2 endPoint = D3DXVECTOR2(this->endAutoPanX, this->endAutoPanY);
	D3DXVECTOR2 thisPoint = D3DXVECTOR2(this->x, this->y);

	D3DXVECTOR2 disVec = endPoint - thisPoint;
	D3DXVec2Normalize(&disVec, &disVec);

	this->autoPanSpeedX = disVec.x * panSpeed;
	this->autoPanSpeedY = disVec.y * panSpeed;

	turnOnLimit = false;
	autoPan = true;
}

void camera::update(float dt) {
	if (gamePlay::Instance->startBossMode) {
		this->x = this->limitPositiveX;
		return;
	}

	if (autoPan) {
		if (game::isSamePoint(this->x, this->endAutoPanX, this->speedX)) {
			this->setPos(this->endAutoPanX, this->endAutoPanY);
			this->setSpeed(0, 0);

			turnOnLimit = true;
			autoPan = false;
		}
		else {
			this->x += this->autoPanSpeedX * dt;
		}
		return;
	}

	if (master == NULL) {
		this->setSpeed(0, 0);
		return;
	}
	else {
		float masterSpeed = std::abs(master->getSpeedX());
		float targetX = floor(master->getX() - (float)(wndWidth / 2));
		float targetY = floor(gamePlay::getInstance()->rootY);

		D3DXVECTOR2 endPoint = D3DXVECTOR2(targetX, targetY);
		D3DXVECTOR2 thisPoint = D3DXVECTOR2(this->x, this->y);

		D3DXVECTOR2 disVec = endPoint - thisPoint;
		D3DXVec2Normalize(&disVec, &disVec);

		this->speedX = disVec.x * camera_default_speed;
		this->speedY = disVec.y * 0;

		this->x += this->speedX * dt;
		this->y += this->speedY * dt;

		if (game::isSamePoint(this->x, targetX, speedX)) {
			this->x = targetX; 
		}
		this->y = targetY;
	}

	if (turnOnLimit && limitPositiveX != camera_nullLimit && limitPositiveY != camera_nullLimit
		&& limitNegativeX != camera_nullLimit && limitNegativeY != camera_nullLimit) {

		if (master->getX() <= this->limitNegativeX + (wndWidth / 2)) {
			if (game::isSamePoint(this->x, this->limitNegativeX, this->speedX)) {
				this->x = this->limitNegativeX;
				this->isTouchLimit = true;
			}
			else this->isTouchLimit = false;
		}
		else if (master->getX() >= this->limitPositiveX + (wndWidth / 2)) {
			if (game::isSamePoint(this->x, this->limitPositiveX, this->speedX)) {
				this->x = this->limitPositiveX;
				this->isTouchLimit = true;
			}
			else this->isTouchLimit = false;
		}
		else this->isTouchLimit = false;
	}
}