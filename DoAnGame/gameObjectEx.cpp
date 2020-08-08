#include "./gameObjectEx.h"
#include "./effectDie.h"
#include "./effectHit.h"
#include "./point.h"
#include "./brokenBrick.h"

#include "./gamePlay.h"

using namespace std;

#define gameObjectEx_broken1 -30, -250
#define gameObjectEx_broken2 -30, -220
#define gameObjectEx_broken3 30, -260
#define gameObjectEx_broken4 30, -250
#define gameObjectEx_broken5 10, -200


gameObjectEx::gameObjectEx(int id)
	: gameObject(id) {
}

void gameObjectEx::spawnHitEffect() {
	effectHit* hit = new effectHit(Counter::index++);

	hit->setPos(this->pos_x, this->pos_y + this->bbox_h / 2);

	gamePlay::getInstance()->objects.push_back(hit);
}

void gameObjectEx::spawnDieEffect() {
	effectDie* die = new effectDie(Counter::index++);

	die->setPos(this->pos_x + this->bbox_w / 2, this->pos_y);

	gamePlay::getInstance()->objects.push_back(die);
}

void gameObjectEx::spawnPoint(int value) {
	point* Point = new point(Counter::index++);
	Point->setPos(this->pos_x, this->pos_y);
	Point->setNum(value);

	gamePlay::getInstance()->objects.push_back(Point);
}

void gameObjectEx::spawnBrokenBrickEffect() {
	D3DXVECTOR2 bricksSpeed[5];
	bricksSpeed[0] = D3DXVECTOR2(gameObjectEx_broken1);
	bricksSpeed[1] = D3DXVECTOR2(gameObjectEx_broken2);
	bricksSpeed[2] = D3DXVECTOR2(gameObjectEx_broken3);
	bricksSpeed[3] = D3DXVECTOR2(gameObjectEx_broken4);
	bricksSpeed[4] = D3DXVECTOR2(gameObjectEx_broken5);

	brokenBrick* bricks[5]; 
	for (int i = 0; i < 5; i++) {
		bricks[i] = new brokenBrick(Counter::index++);
		bricks[i]->setSpeed(bricksSpeed[i].x, bricksSpeed[i].y); 
		bricks[i]->setPos(this->pos_x + (this->bbox_w / 2),
			this->pos_y + (this->bbox_h / 2)); 

		gamePlay::getInstance()->objects.push_back(bricks[i]);
	}
}

void gameObjectEx::setSizeItem(string itemName) {
	xml_n* rootNode = database::getInstance()->getRoot("singleton")
		->first_node("item")->first_node(itemName.c_str());

	float w = Numeric::toFloat(get_atr(rootNode, "width"));
	float h = Numeric::toFloat(get_atr(rootNode, "height"));
	this->setSize(w, h);
}

void gameObjectEx::groundHandle() {
	if (this->speed_y < 0) {
		onTheGround = false;
		return;
	}
	FRect heroArea = this->getBoundingBox();

	vector<LPGAMEOBJECT> arr;
	gamePlay* gameInstance = gamePlay::getInstance();

	int cellW = gameInstance->cellWidth;
	int cellH = gameInstance->cellHeight;
	int gridX = gameInstance->gridCountX;

	int id1 = game::getCell(pos_x - collisionPad,
		pos_y + bbox_h + collisionPad, cellW, cellH, gridX);

	int id2 = game::getCell(pos_x + bbox_w + collisionPad,
		pos_y + bbox_h + collisionPad, cellW, cellH, gridX);

	vector<LPGAMEOBJECT> arr1 = gameInstance->surfaceGrid[id1];
	vector<LPGAMEOBJECT> arr2 = gameInstance->surfaceGrid[id2];
	vector<LPGAMEOBJECT> arr3 = gameInstance->movingGrid[id1];
	vector<LPGAMEOBJECT> arr4 = gameInstance->movingGrid[id2];

	int size = arr1.size() + arr2.size() + arr3.size() + arr4.size();
	arr.reserve(size);

	arr.insert(arr.begin(), arr1.begin(), arr1.end());
	arr.insert(arr.begin(), arr2.begin(), arr2.end());
	arr.insert(arr.begin(), arr3.begin(), arr3.end());
	arr.insert(arr.begin(), arr4.begin(), arr4.end());

	standingGround = NULL;
	onTheGround = false;

	for (int i = 0; i < arr.size(); i++) {
		FRect objArea = arr[i]->getBoundingBox();

		if (arr[i]->tag == "ground" && objArea.top > heroArea.bottom &&
			(objArea.left <= heroArea.right && objArea.right >= heroArea.left)) {

			if (std::abs(heroArea.bottom - objArea.top) <= groundPad) {
				standingGround = arr[i];

				onTheGround = true;
				return;
			}
		}
	}
}

void gameObjectEx::performCollisionBarrier(float dt) {
	float dx = speed_x * dt;
	float dy = speed_y * dt;

	// perform collision barrier

	pos_x += this->collisionMinTx * dx + this->collisionNx * surfaceNormalForce;
	pos_y += this->collisionMinTy * dy + this->collisionNy * surfaceNormalForce;

	if (this->collisionNx != 0)
		speed_x = 0;
	if (this->collisionNy != 0)
		speed_y = 0;

	checkCollisionBorderX = false;
	checkCollisionBorderY = false;
}

void gameObjectEx::removeCollisionX(LPCOLLISION col, float dt) {
	pos_x -= col->nx * surfaceNormalForce;

	pos_x += (1 - col->t) * collisionOldSpeedX * dt;
	speed_x = collisionOldSpeedX;
}

void gameObjectEx::removeCollisionY(LPCOLLISION col, float dt) {
	if (col->ny == 1)
		pos_y -= col->ny * surfaceNormalForce * 2;
	else pos_y -= col->ny * surfaceNormalForce;

	pos_y += (1 - col->t) * collisionOldSpeedY * dt;
	speed_y = collisionOldSpeedY;
}

void gameObjectEx::removeCollisionBarrier(LPCOLLISION col, float dt) {
	if (col->nx != 0)
		this->removeCollisionX(col, dt);
	if (col->ny != 0)
		this->removeCollisionY(col, dt);
}

D3DXVECTOR2 gameObjectEx::calcaulateLaunchForce(D3DXVECTOR2 target, float gravity, float jumpHeight,
	float dt, bool reverseGravity) {

	D3DXVECTOR2 result;

	float Py = (this->pos_y + this->bbox_h) - target.y;
	float Px = 0.0f;
	float g = -(std::abs(gravity) / dt);

	if (target.x >= this->pos_x) {
		Px = std::abs(target.x - (this->pos_x + this->bbox_w)); 

		if (Px <= Px_too_small) {
			Px = std::abs(target.x - this->pos_x);
		}
	}
	else if (target.x <= this->pos_x + this->bbox_w) {
		Px = std::abs(target.x - this->pos_x);

		if (Px <= Px_too_small) {
			Px = std::abs(target.x - (this->pos_x + this->bbox_w));
		}
	}
	
	result.x = (Px / (std::sqrtf((-2 * std::abs(Py)) / g) + std::sqrtf(((2 * (Py - jumpHeight)) / g))));
	result.y = -std::sqrtf(-2 * g * std::abs(jumpHeight));

	if (reverseGravity)
		result.y *= -1;

	return result;
}

void gameObjectEx::updateSweptAABB(float dt) {
	this->collisionOldSpeedX = this->speed_x;
	this->collisionOldSpeedY = this->speed_y;

	float dx = speed_x * dt;
	float dy = speed_y * dt;

	vector<LPCOLLISION> col, res;

	this->getCollisionSweptAABB(col, dt);

	this->groundHandle();

	if (col.size() == 0) {
		this->pos_x += dx;
		this->pos_y += dy;
	}
	else {
		this->filterCollision(col, res, this->collisionMinTx, this->collisionMinTy,
			this->collisionNx, this->collisionNy);

		this->performCollisionBarrier(dt);

		for (int i = 0; i < res.size(); i++) {
			this->collisionHandle(res[i], dt);
		}
	}
	for (auto& x : col)
		free(x);
}

void gameObjectEx::updateAABB(float dt) {
	this->collisionOldSpeedX = this->speed_x;
	this->collisionOldSpeedY = this->speed_y;

	float dx = speed_x * dt;
	float dy = speed_y * dt;

	this->pos_x += dx;
	this->pos_y += dy;

	vector<LPCOLLISION> col;

	this->getCollisionAABB(col, dt);

	for (int i = 0; i < col.size(); i++) {
		this->collisionHandle(col[i], dt);
	}
	for (auto& x : col)
		free(x);
}

void gameObjectEx::updateSweptFilterAABB(std::unordered_map<string, bool> filterTag,
	std::unordered_map<string, bool> filterType, float dt) {
	vector<LPGAMEOBJECT> arr;

	int pad = collisionPad;

	gamePlay* gameInstance = gamePlay::getInstance();

	int cellW = gameInstance->cellWidth;
	int cellH = gameInstance->cellHeight;
	int gridX = gameInstance->gridCountX;

	int id1 = game::getCell(pos_x - pad, pos_y - pad, cellW, cellH, gridX);
	int id2 = game::getCell(pos_x - pad, pos_y + bbox_h + pad, cellW, cellH, gridX);
	int id3 = game::getCell(pos_x + bbox_w + pad, pos_y, cellW, cellH, gridX);
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

	this->collisionOldSpeedX = this->speed_x;
	this->collisionOldSpeedY = this->speed_y;

	float dx = speed_x * dt;
	float dy = speed_y * dt;

	vector<LPCOLLISION> sweptAABBCol, AABBCol, res;

	for (int i = 0; i < arr.size(); i++) {
		LPGAMEOBJECT object = arr[i];

		if (object->isDestroyed || !object->canTouch || object == NULL)
			continue;

		if (object->tag_id == this->tag_id)
			continue;

		if (filterType[object->type] == true || filterTag[object->tag] == true) {
			LPCOLLISION AABBColide = new collision(AABBCollision, 0.0f, 0.0f, object);

			if (game::getInstance()->AABB(this->getBoundingBox(),
				object->getBoundingBox())) {

				AABBColide->tag = AABBCollision; 
				
				if (object->getX() >= this->pos_x) {
					AABBColide->t = std::abs(object->getX() - this->pos_x);
				}
				else if (object->getX() + object->getWidth() <= this->pos_x + this->bbox_w) {
					AABBColide->t = std::abs((object->getX() + object->getWidth()) -
						(this->pos_x + this->bbox_w));
				}
				AABBCol.push_back(AABBColide); 
			}
			else free(AABBColide);
			continue;
		}
		else {
			LPCOLLISION colide = this->sweptAABBEx(object, dt);

			if (colide->t > 0.0f && colide->t < 1.0f) {

				colide->tag = SweptAABBCollision;
				sweptAABBCol.push_back(colide);
			}
			else
				free(colide);
		}
	}
	std::sort(AABBCol.begin(), AABBCol.end());

	for (auto& x : AABBCol) {
		this->collisionHandle(x, dt);
	}

	std::sort(sweptAABBCol.begin(), sweptAABBCol.end());

	groundHandle();

	if (sweptAABBCol.size() == 0) {
		this->pos_x += dx;
		this->pos_y += dy;
	}
	else {
		this->filterCollision(sweptAABBCol, res, this->collisionMinTx, this->collisionMinTy,
			this->collisionNx, this->collisionNy);

		this->performCollisionBarrier(dt);

		for (int i = 0; i < res.size(); i++) {
			this->collisionHandle(res[i], dt);
		}
	}
	for (auto& x : AABBCol)
		free(x);
	for (auto& x : sweptAABBCol)
		free(x);

	filterTag.clear();
	filterType.clear();
}