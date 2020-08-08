#include "./bone.h"
#include "./simon.h"
#include "./camera.h"
#include "./gamePlay.h"

bone::bone(int id, float speedx, float speedy, int directionX)
	: gameObjectEx(id) {
	this->type = "enemy";
	this->assignAnimationSet("bone");

	this->health = bone_health;
	this->aniState = 0;

	this->directionX = directionX;

	this->speed_x = speedx * this->directionX;
	this->speed_y = speedy;

	this->setSizeItem("bone"); 
}

void bone::hurt(int damage) {
	if (isDestroyed) return;

	this->health -= damage;

	this->spawnHitEffect();

	if (this->health <= 0) {
		this->spawnDieEffect();

		isDestroyed = true;
	}
}

void bone::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {
}

void bone::setState(int) {
}

void bone::update(float dt) {
	speed_y += bone_gravity * dt;

	vector<string> AABBTag = { "torch", "candle", "flame" };
	vector<string> AABBType = { "enemy", "item", "portal" };

	this->updateSweptFilterAABB(game::makeFilterCollision(AABBTag),
		game::makeFilterCollision(AABBType), dt);

	camera* cam = camera::getInstance();

	float screen_x = this->pos_x - cam->getX();
	float screen_y = this->pos_y - cam->getY();

	if (screen_x + bbox_w < 0 || screen_x > wndWidth || screen_y + bbox_h < 0 || screen_y > wndHeight) {
		isDestroyed = true;
	}
}

void bone::collisionHandle(LPCOLLISION colEvent,
	float dt) {
	LPGAMEOBJECT obj = colEvent->obj;

	if (obj->tag != "simon")
		this->removeCollisionBarrier(colEvent, dt);
	else {
		cast(obj, simon*)->hurt(colEvent->nx, colEvent->ny, bone_damage);
		isDestroyed = true;
	}
}

void bone::render() {
	animation[aniState]->render(pos_x, pos_y, directionX, 255);
}