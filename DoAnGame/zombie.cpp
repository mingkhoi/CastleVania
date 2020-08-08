#include "./zombie.h"
#include "./camera.h"
#include "./simon.h"
#include "./gamePlay.h"

using namespace std;

zombie::zombie(int id)
	: gameObjectEx(id) {
	type = "enemy";

	this->health = zombie_health;

	this->assignAnimationSet("zombie");

	aniState = 0;
}


void zombie::hurt(int damage) {
	if (isDestroyed || isHurted)
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

void zombie::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {
}

void zombie::setState(int value) {
	state = value;

	switch (state) {
	case zombie_state_walk:
		speed_x = zombie_speed_x * this->directionX;

		break;
	}
}

void zombie::update(float dt) {
	speed_y += zombie_gravity * dt;

	if (!isHurted) this->setState(zombie_state_walk);
	else speed_x = 0;

	if (isHurted) {
		if (GetTickCount64() - startHurtedTime >= zombie_hit_time)
			isHurted = false;
	}
	
	vector<string> AABBTag = { "torch", "candle", "flame" };
	vector<string> AABBType = { "enemy", "item", "portal", "subweapon" };

	this->updateSweptFilterAABB(game::makeFilterCollision(AABBTag),
		game::makeFilterCollision(AABBType), dt); 

	camera* cam = camera::getInstance();

	float screen_x = pos_x - cam->getX();
	float screen_y = pos_y - cam->getY();

	if (screen_x + this->bbox_w < -zombie_out_screen || screen_x > wndWidth + zombie_out_screen || 
		screen_y + this->bbox_h < -zombie_out_screen || screen_y > wndHeight + zombie_out_screen ||
		gamePlay::Instance->startBossMode) {
		this->isDestroyed = true;
	}
}

void zombie::collisionHandle(LPCOLLISION colEvent,
	float dt) {
	LPGAMEOBJECT obj = colEvent->obj;

	if (obj->tag == "simon") {
		cast(obj, simon*)->hurt(colEvent->nx, colEvent->ny, zombie_damage);
		this->removeCollisionBarrier(colEvent, dt);
	}

	if (obj->tag == "ground" && colEvent->nx != 0) {
		this->removeCollisionBarrier(colEvent, dt);
	}
}

void zombie::render() {
	this->animation[aniState]->render(pos_x, pos_y, directionX, 255);
}
