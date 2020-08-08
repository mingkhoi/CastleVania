#include "./BossBullet.h"
#include "./simon.h"
#include "./camera.h"

BossBullet::BossBullet(int id) : gameObjectEx(id) {
	this->type = "enemy"; 
	this->addAnimationSet("boss_bullet");

	this->setSizeItem("bossBullet"); 

	this->aniState = 0;
}

void BossBullet::hurt(int damage) {
	if (isDestroyed) return;

	this->health -= damage;

	this->spawnHitEffect();

	if (this->health <= 0) {
		this->spawnDieEffect();

		isDestroyed = true;
	}
}

void BossBullet::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {

}

void BossBullet::setState(int) {

}

void BossBullet::update(float dt) {

	vector<string> AABBTag = { "torch", "candle", "flame" };
	vector<string> AABBType = { "enemy", "item", "portal", "subweapon" };

	this->updateSweptFilterAABB(game::makeFilterCollision(AABBTag),
		game::makeFilterCollision(AABBType), dt);

	camera* cam = camera::getInstance();

	float screen_x = this->pos_x - cam->getX();
	float screen_y = this->pos_y - cam->getY();

	if (screen_x + bbox_w < 0 || screen_x > wndWidth ||
		screen_y + bbox_h < 0 || screen_y > wndHeight) {
		isDestroyed = true;
	}
}

void BossBullet::collisionHandle(LPCOLLISION collisionEvent,
	float dt) {
	if (collisionEvent->obj->tag == "simon") {
		cast(collisionEvent->obj, simon*)->hurt(collisionEvent->nx, collisionEvent->ny,
			bossBullet_damage);
		this->isDestroyed = true;
	}
	else {
		this->removeCollisionBarrier(collisionEvent, dt);
	}
}

void BossBullet::render() {
	this->animation[aniState]->render(pos_x, pos_y, Left, 
		D3DCOLOR_XRGB(255, 255, 255));
}