#include "./candle.h"
#include "./listGameobject.h"
#include "./gamePlay.h"
#include "./simon.h"

using namespace std;


candle::candle(int id) : gameObjectEx(id) {
	this->assignAnimationSet("candle");

	aniState = 0;
}

void candle::loadData(xml_n* objNode) {
	xml_n* spawnProperty = objNode->first_node("spawnProperty");

	this->spawnObjectName = (string)get_atr(spawnProperty, "name");
	this->isStaticObject = true;
}

void candle::spawnObject() {
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

void candle::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {

}

void candle::setState(int value) {

}

void candle::update(float dt) {
	
}

void candle::collisionHandle(LPCOLLISION collisionEvent,
	float dt) {
}

void candle::render() {
	animation[aniState]->renderFromBottom(pos_x, pos_y + bbox_h, directionX, 255);
}
