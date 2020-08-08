#include "./torch.h"
#include "./effectDie.h"
#include "./effectHit.h"
#include "./gamePlay.h"
#include "./listGameobject.h"
#include "./simon.h"

using namespace std;

torch::torch(int id)
	: gameObjectEx(id) {
	this->assignAnimationSet("torch");

	bbox_w = 16;
	bbox_h = 30;

	aniState = 0;
}

void torch::loadData(xml_n* objNode) {
	xml_n* spawnProperty = objNode->first_node("spawnProperty");

	this->spawnObjectName = (string)get_atr(spawnProperty, "name");
	this->isStaticObject = true;
}

void torch::spawnObject() {
	if (isDestroyed) return;

	LPGAMEOBJECT hero = gamePlay::getInstance()->player;

	if (hero != NULL && cast(hero, simon*)->whipLv < simon_max_whip_level) {
		discrete_distribution<> rand = { 3.0f, 7.0f };

		int index = rand(game::generator);

		if (index == 1) {
			spawnObjectName = "bow";
		}
	}

	LPGAMEOBJECT spawnObj = listGameobject::getInstance()->reflect(spawnObjectName);

	spawnObj->setPos(this->pos_x, this->pos_y);
	spawnObj->setState(item_state);

	spawnObj->type = "item";
	spawnObj->tag = spawnObjectName;

	gamePlay::getInstance()->objects.push_back(spawnObj);

	this->spawnDieEffect();
	this->spawnHitEffect();

	isDestroyed = true;
}

void torch::update(float dt) {
	speed_y += 650.0f * dt;

	this->updateSweptAABB(dt);
}

void torch::collisionHandle(LPCOLLISION colEvent,
	float dt) {
	LPGAMEOBJECT obj = colEvent->obj;

	if (obj->tag != "ground") {
		removeCollisionBarrier(colEvent, dt);
	}
}

void torch::render() {
	this->animation[aniState]->renderFromBottom(pos_x, pos_y + bbox_h, Left, 255);
}