#include "./gameObject.h"
#include "./animation.h"
#include "./animationSet.h"
#include "./database.h"
#include "./effectDie.h"
#include "./effectHit.h"
#include "./gamePlay.h"
#include "./texture.h"

#include <algorithm>
#include <unordered_map>

collision::collision(float t, float nx, float ny,
	LPGAMEOBJECT obj) {
	this->t = t;
	this->nx = nx;
	this->ny = ny;
	this->obj = obj;
}

bool compareCollision(collision* a, collision* b) {
	return a->t < b->t;
}

// ============================================

gameObject::gameObject(std::wstring tag_id) {
	isDestroyed = false;
	canTouch = true;

	this->tag_id = tag_id;

	speed_x = 0;
	speed_y = 0;
	bbox_w = bbox_h = 0;

	directionX = Left; // to left
}

gameObject::gameObject(int tag_id) {
	isDestroyed = false;
	canTouch = true;

	this->tag_id = std::to_wstring(tag_id);

	speed_x = 0;
	speed_y = 0;
	bbox_w = bbox_h = 0;

	directionX = Left; // to left
}

gameObject::gameObject(const LPGAMEOBJECT& other) {
	this->tag_id = std::to_wstring(Counter::index++);

	this->setPos(other->getX(), other->getY());
	this->setSpeed(other->getSpeedX(), other->getSpeedY());
	this->bbox_w = other->getWidth();
	this->bbox_h = other->getHeight();
	
	this->animation = other->getAnimationSet();
	this->aniState = other->getCurrentAnimationState();

	this->tag = other->tag;
	this->type = other->type;
}

gameObject::~gameObject() {
	animation.clear();
}

float gameObject::getX() { return pos_x; }
float gameObject::getY() { return pos_y; }
float gameObject::getWidth() { return bbox_w; }
float gameObject::getHeight() { return bbox_h; }
float gameObject::getSpeedX() { return speed_x; }
float gameObject::getSpeedY() { return speed_y; }
int gameObject::getDirectionX() { return directionX; }
int gameObject::getCurrentState() { return state; }

LPANIMATION gameObject::getCurrentAnimation() {
	return animation[aniState];
}
LPANIMATION gameObject::getAnimation(int id) {
	if (id >= animation.size())
		return NULL;
	return animation[id];
}

vector<LPANIMATION> gameObject::getAnimationSet() {
	return animation;
}

int gameObject::getCurrentAnimationState() {
	return aniState;
}

void gameObject::setX(float value) { pos_x = value; }
void gameObject::setY(float value) { pos_y = value; }
void gameObject::setPos(float x, float y) {
	pos_x = x;
	pos_y = y;
}
void gameObject::setSpeedX(float value) { speed_x = value; }
void gameObject::setSpeedY(float value) { speed_y = value; }
void gameObject::setSpeed(float x, float y) {
	speed_x = x;
	speed_y = y;
}
void gameObject::setWidth(float value) { bbox_w = value; }
void gameObject::setHeight(float value) { bbox_h = value; }
void gameObject::setSize(float w, float h) {
	this->bbox_w = w;
	this->bbox_h = h;
}

void gameObject::setDirecttionX(int value) { directionX = value; }

void gameObject::addAnimationSet(string name) {
	vector<LPANIMATION> temp = animationSet::getInstance()->getAnimationSet(name);

	this->animation.insert(animation.end(), temp.begin(), temp.end());
}

void gameObject::assignAnimationSet(string name) {
	this->animation = animationSet::getInstance()->getAnimationSet(name);
}

void gameObject::hurt(int damage) {

}

void gameObject::loadData(xml_n* objNode) {

}

FRect gameObject::getBoundingBox() {
	FRect value;
	value.left = this->pos_x;
	value.top = this->pos_y;
	value.right = value.left + bbox_w;
	value.bottom = value.top + bbox_h;

	return value;
}

LPCOLLISION gameObject::sweptAABBEx(LPGAMEOBJECT other, float dt) {
	float other_vx = other->getSpeedX() * dt;
	float other_vy = other->getSpeedY() * dt;

	float nx, ny;

	float vx = (this->speed_x * dt) - other_vx;
	float vy = (this->speed_y * dt) - other_vy;

	if (other->bbox_w == 0 || other->bbox_h == 0)
		return new collision(5.0f, 1.0f, 1.0f, other);

	float t = game::getInstance()->sweptAABB(
		this->getBoundingBox(), other->getBoundingBox(),
		vx, vy, nx, ny);

	return new collision(t, nx, ny, other);
}

void gameObject::getCollisionSweptAABB(
	std::vector<LPCOLLISION>& collisionEvent, float dt) {
	vector<LPGAMEOBJECT> arr;

	gamePlay* gameInstance = gamePlay::getInstance();

	int cellW = gameInstance->cellWidth;
	int cellH = gameInstance->cellHeight;
	int gridX = gameInstance->gridCountX;

	int pad = collisionPad;

	int id1 = game::getCell(pos_x - pad, pos_y - pad, cellW, cellH, gridX);
	int id2 = game::getCell(pos_x - pad, pos_y + bbox_h + pad, cellW, cellH, gridX);
	int id3 = game::getCell(pos_x + bbox_w + pad, pos_y - pad, cellW, cellH, gridX);
	int id4 = game::getCell(pos_x + bbox_w + pad, pos_y + bbox_h + pad, cellW, cellH, gridX);

	vector<LPGAMEOBJECT> arr1 = gameInstance->movingGrid[id1];
	vector<LPGAMEOBJECT> arr2 = gameInstance->movingGrid[id2];
	vector<LPGAMEOBJECT> arr3 = gameInstance->movingGrid[id3];
	vector<LPGAMEOBJECT> arr4 = gameInstance->movingGrid[id4];

	vector<LPGAMEOBJECT> arr5 = gameInstance->surfaceGrid[id1];
	vector<LPGAMEOBJECT> arr6 = gameInstance->surfaceGrid[id2];
	vector<LPGAMEOBJECT> arr7 = gameInstance->surfaceGrid[id3];
	vector<LPGAMEOBJECT> arr8 = gameInstance->surfaceGrid[id4];

	int size = arr1.size() + arr2.size() + arr3.size() + arr4.size() +
		arr5.size() + arr6.size() + arr7.size() + arr8.size();
	arr.reserve(size);

	arr.insert(arr.begin(), arr1.begin(), arr1.end());
	arr.insert(arr.begin(), arr2.begin(), arr2.end());
	arr.insert(arr.begin(), arr3.begin(), arr3.end());
	arr.insert(arr.begin(), arr4.begin(), arr4.end());
	arr.insert(arr.begin(), arr5.begin(), arr5.end());
	arr.insert(arr.begin(), arr6.begin(), arr6.end());
	arr.insert(arr.begin(), arr7.begin(), arr7.end());
	arr.insert(arr.begin(), arr8.begin(), arr8.end());

	for (int i = 0; i < arr.size(); i++) {
		LPGAMEOBJECT object = arr[i];

		if (object->tag_id == this->tag_id)
			continue;

		if (object->isDestroyed || !object->canTouch || object == NULL)
			continue;

		LPCOLLISION collideEvent = sweptAABBEx(object, dt);

		if (collideEvent->t > 0 && collideEvent->t < 1.0f) {
			collideEvent->tag = SweptAABBCollision;

			collisionEvent.push_back(collideEvent);
		}
		else
			free(collideEvent);
	}
	std::sort(collisionEvent.begin(), collisionEvent.end(), compareCollision);
}

void gameObject::getCollisionAABB(
	std::vector<LPCOLLISION>& collisionEvent, float dt) {
	vector<LPGAMEOBJECT> arr;

	gamePlay* gameInstance = gamePlay::getInstance();

	int cellW = gameInstance->cellWidth;
	int cellH = gameInstance->cellHeight;
	int gridX = gameInstance->gridCountX;

	int pad = collisionPad;

	int id1 = game::getCell(pos_x - pad, pos_y - pad, cellW, cellH, gridX);
	int id2 = game::getCell(pos_x - pad, pos_y + bbox_h + pad, cellW, cellH, gridX);
	int id3 = game::getCell(pos_x + bbox_w + pad, pos_y - pad, cellW, cellH, gridX);
	int id4 = game::getCell(pos_x + bbox_w + pad, pos_y + bbox_h + pad, cellW, cellH, gridX);

	vector<LPGAMEOBJECT> arr1 = gameInstance->movingGrid[id1];
	vector<LPGAMEOBJECT> arr2 = gameInstance->movingGrid[id2];
	vector<LPGAMEOBJECT> arr3 = gameInstance->movingGrid[id3];
	vector<LPGAMEOBJECT> arr4 = gameInstance->movingGrid[id4];

	vector<LPGAMEOBJECT> arr5 = gameInstance->surfaceGrid[id1];
	vector<LPGAMEOBJECT> arr6 = gameInstance->surfaceGrid[id2];
	vector<LPGAMEOBJECT> arr7 = gameInstance->surfaceGrid[id3];
	vector<LPGAMEOBJECT> arr8 = gameInstance->surfaceGrid[id4];

	int size = arr1.size() + arr2.size() + arr3.size() + arr4.size() +
		arr5.size() + arr6.size() + arr7.size() + arr8.size();
	arr.reserve(size);

	arr.insert(arr.begin(), arr1.begin(), arr1.end());
	arr.insert(arr.begin(), arr2.begin(), arr2.end());
	arr.insert(arr.begin(), arr3.begin(), arr3.end());
	arr.insert(arr.begin(), arr4.begin(), arr4.end());
	arr.insert(arr.begin(), arr5.begin(), arr5.end());
	arr.insert(arr.begin(), arr6.begin(), arr6.end());
	arr.insert(arr.begin(), arr7.begin(), arr7.end());
	arr.insert(arr.begin(), arr8.begin(), arr8.end());

	for (int i = 0; i < arr.size(); i++) {
		LPGAMEOBJECT object = arr[i];

		if (object->tag_id == this->tag_id)
			continue;

		if (object->isDestroyed || !object->canTouch || object == NULL)
			continue;

		LPCOLLISION collideEvent = new collision(AABBCollision, 0.0f, 0.0f, object);

		if (game::getInstance()->AABB(this->getBoundingBox(),
			object->getBoundingBox())) {
			collideEvent->tag = AABBCollision;

			if (object->getX() >= this->pos_x) {
				collideEvent->t = std::abs(object->getX() - this->pos_x);
			}
			else if (object->getX() + object->getWidth() <= this->pos_x + this->bbox_w) {
				collideEvent->t = std::abs((object->getX() + object->getWidth()) -
					(this->pos_x + this->bbox_w));
			}
			collisionEvent.push_back(collideEvent);
		}
		else free(collideEvent);
	}
	std::sort(collisionEvent.begin(), collisionEvent.end(), compareCollision);
}

void gameObject::filterCollision(std::vector<LPCOLLISION>& coEvent,
	std::vector<LPCOLLISION>& coResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny) {

	min_tx = 1.0f;
	min_ty = 1.0f;

	nx = 0, ny = 0;

	int index_x = 0, index_y = 0;

	for (int i = 0; i < coEvent.size(); i++) {
		LPCOLLISION item = coEvent[i];

		if (item->t < min_tx && item->nx != 0) {
			min_tx = item->t;
			nx = item->nx;
			index_x = i;
		}

		if (item->t < min_ty && item->ny != 0) {
			min_ty = item->t;
			ny = item->ny;
			index_y = i;
		}
	}

	if (index_x == index_y)
		coResult.push_back(coEvent[index_x]);
	else {
		if (coEvent[index_x]->obj->tag_id == coEvent[index_y]->obj->tag_id) {
			coResult.push_back(coEvent[index_x]);
		}
		else {
			coResult.push_back(coEvent[index_x]);
			coResult.push_back(coEvent[index_y]);
		}
	}
}