#include "./enemySpawner.h"
#include "./camera.h"
#include "./gamePlay.h"
#include "./listGameobject.h"
#include "./simon.h"

enemySpawner::enemySpawner(int id)
	: gameObjectEx(id) {
	this->type = "portal"; 
	speed_x = speed_y = 0;

	checkSide = spawnEnemy_none;
}

void enemySpawner::loadData(xml_n* objNode) {
	xml_n* spawnProperty = objNode->first_node("spawnProperty");

	int rootX = gamePlay::getInstance()->rootX;
	int rootY = gamePlay::getInstance()->rootY;

	this->spawn_2_side = Numeric::toInt(get_atr(spawnProperty, "spawn_2_side"));
	this->spawn_x = Numeric::toFloat(get_atr(spawnProperty, "x")) + rootX;
	this->spawn_y = Numeric::toFloat(get_atr(spawnProperty, "y")) + rootY;
	this->spawn_w = Numeric::toFloat(get_atr(spawnProperty, "width"));
	this->spawn_h = Numeric::toFloat(get_atr(spawnProperty, "height"));
	this->spawn_speed_x = Numeric::toFloat(get_atr(spawnProperty, "vx"));
	this->spawn_speed_y = Numeric::toFloat(get_atr(spawnProperty, "vy"));
	this->spawnDirectionX = Numeric::toInt(get_atr(spawnProperty, "directionX"));

	this->spawnName = (string)get_atr(spawnProperty, "name");
	this->spawnTag = (string)get_atr(spawnProperty, "tag");

	this->isStaticObject = false;
}

void enemySpawner::spawnObject() {
	LPGAMEOBJECT spawnObj = listGameobject::getInstance()->reflect(spawnName);

	spawnObj->setPos(spawn_x, spawn_y);
	spawnObj->setSize(spawn_w, spawn_h);
	spawnObj->setDirecttionX(spawnDirectionX);

	spawnObj->tag = spawnTag;

	gamePlay::getInstance()->objects.push_back(spawnObj);
}

bool enemySpawner::checkSpawn() {
	camera* cam = camera::getInstance();

	float screen_x = this->pos_x - cam->getX();
	float screen_y = this->pos_y - cam->getY();

	if (this->spawn_2_side) {
		if (firstTimeSpawn) {
			if (screen_x >= 0 && screen_x < wndWidth) {
				checkSide = spawnEnemy_none;
				firstTimeSpawn = false;
				return true;
			}
		}
		else {
			if (screen_x + bbox_w <= -spawnEnemy_pad_spawn
				|| screen_x >= wndWidth + spawnEnemy_pad_spawn) {
				checkSide = spawnEnemy_out_screen;
			}
			else if ((screen_x >= 0 || screen_x >= wndWidth - spawn_w)
				&& screen_x < wndWidth && checkSide == spawnEnemy_out_screen) {
				checkSide = spawnEnemy_none;
				return true;
			}
		}
	}
	else {
		if (firstTimeSpawn) {
			if (screen_x >= 0 && screen_x < wndWidth) {
				checkSide = spawnEnemy_none;
				firstTimeSpawn = false;
				return true;
			}
		}
		else {
			if (screen_x + bbox_w <= -spawnEnemy_pad_spawn)
				checkSide = spawnEnemy_out_screen;
			else if (screen_x >= 0 && screen_x < wndWidth
				&& checkSide == spawnEnemy_out_screen) {
				checkSide = spawnEnemy_none;

				return true;
			}
		}
	}
	return false;
}

void enemySpawner::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {
}

void enemySpawner::setState(int value) {
}

void enemySpawner::update(float dt) {
	if (camera::getInstance()->autoPan) return;

	if (this->checkSpawn())
		this->spawnObject();
}

void enemySpawner::collisionHandle(LPCOLLISION collisionEvent,
	float dt) {
}

void enemySpawner::render() {
}