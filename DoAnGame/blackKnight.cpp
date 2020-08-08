#include "./blackKnight.h"
#include "./gamePlay.h"
#include "./simon.h"

#include "./effectDie.h"
#include "./effectHit.h"

#include <random>
#include <vector>

using namespace std;

blackKnight::blackKnight(int id)
	: gameObjectEx(id) {
	this->assignAnimationSet("black_knight");
	this->type = "enemy";

	this->health = blackKnight_health;

	leftBorder = rightBorder = blackKnight_boundary_null;
}

float blackKnight::distanceTo(LPGAMEOBJECT obj) {
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

void blackKnight::attackSimon() {
	LPGAMEOBJECT target = gamePlay::getInstance()->player;

	if (this->pos_x < target->getX() + target->getWidth())
		directionX = Right;
	else if (this->pos_x + this->bbox_w > target->getX())
		directionX = Left;
}

void blackKnight::hurt(int damage) {
	if ((isDestroyed || isHurted) && !game::gameFreeze)
		return;

	this->health -= damage;
	isHurted = true;

	this->spawnHitEffect();

	startHurtedTime = GetTickCount64();

	if (this->health <= 0) {
		this->spawnDieEffect();

		isDestroyed = true;
	}
}

void blackKnight::detectSimon(float dt) {
	LPGAMEOBJECT target = gamePlay::getInstance()->player;

	bool posYCondition = std::abs((target->getY() + target->getHeight())
		- (this->pos_y + this->getHeight()))
		<= groundPad;

	LPGAMEOBJECT targetStandingGround = cast(target, gameObjectEx*)->standingGround;

	bool groundCondition = (targetStandingGround != NULL && this->standingGround != NULL
		&& targetStandingGround->tag_id == this->standingGround->tag_id);

	if (posYCondition && groundCondition && distanceTo(target) <= blackKnight_detect_simon_distance
		&& isDetectSimon == false) {
		isDetectSimon = true;

		this->attackSimon();
	}
	if (!groundCondition && !posYCondition)
		isDetectSimon = false;
}

bool blackKnight::isGround(float x, float y) {
	gamePlay* gameInstance = gamePlay::getInstance();

	int cellW = gameInstance->cellWidth;
	int cellH = gameInstance->cellHeight;
	int gridX = gameInstance->gridCountX;

	int id = game::getCell(x, y, cellW, cellH, gridX);

	vector<LPGAMEOBJECT> arr = gameInstance->surfaceGrid[id];

	for (int i = 0; i < arr.size(); i++) {
		FRect area = arr[i]->getBoundingBox();

		if (arr[i]->tag != "ground")
			continue;

		if (x >= area.left && x <= area.right && y >= area.top && y <= area.bottom) {
			return true;
		}
	}
	return false;
}

void blackKnight::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {
}

void blackKnight::detectBoundary() {
	if (this->directionX == Right) {
		FRect bbox = this->getBoundingBox();

		float wall_x = bbox.right + blackKnight_padX;
		float wall_y = bbox.bottom - blackKnight_padY;

		float grave_x = bbox.right + blackKnight_padX;
		float grave_y = bbox.bottom + blackKnight_padY;

		bool con1 = isGround(wall_x, wall_y);
		bool con2 = !isGround(grave_x, grave_y);

		if (con1 || con2) {
			if (rightBorder == blackKnight_boundary_null) {
				if (con1)
					rightBorder = wall_x;
				else if (con2)
					rightBorder = grave_x;
			}
			directionX = Left;
		}
	}
	else if (this->directionX == Left) {
		FRect bbox = this->getBoundingBox();

		float wall_x = bbox.left - blackKnight_padX;
		float wall_y = bbox.bottom - blackKnight_padY;

		float grave_x = bbox.left - blackKnight_padX;
		float grave_y = bbox.bottom + blackKnight_padY;

		bool con1 = isGround(wall_x, wall_y);
		bool con2 = !isGround(grave_x, grave_y);

		if (con1 || con2) {
			if (leftBorder == blackKnight_boundary_null) {
				if (con1)
					leftBorder = wall_x;
				else if (con2)
					leftBorder = grave_x;
			}
			directionX = Right;
		}
	}
}

void blackKnight::setState(int value) {
	state = value;

	switch (state) {
	case blackKnight_state_walking:
		speed_x = blackKnight_speedX * directionX;
		break;
	}
}

void blackKnight::update(float dt) {
	speed_y += blackKnight_gravity * dt;

	if (!isHurted) {
		setState(blackKnight_state_walking);
	}
	else
		speed_x = 0;

	if (isHurted) {
		if (GetTickCount64() - startHurtedTime >= blackKnight_hit_time)
			isHurted = false;
	}

	this->detectSimon(dt);
	this->detectBoundary();
		
	vector<string> AABBTag = { "torch", "candle", "flame" };
	vector<string> AABBType = { "enemy", "item", "portal", "subweapon" };

	this->updateSweptFilterAABB(game::makeFilterCollision(AABBTag),
		game::makeFilterCollision(AABBType), dt);
}

void blackKnight::collisionHandle(LPCOLLISION colEvent, float dt) {
	LPGAMEOBJECT obj = colEvent->obj;

	if (obj->tag == "simon") {
		cast(obj, simon*)->hurt(colEvent->nx, colEvent->ny, blackKnight_damage);

		this->removeCollisionBarrier(colEvent, dt);
	}
}

void blackKnight::render() {
	this->animation[aniState]->render(pos_x, pos_y, directionX, 255);
}