#include "./skeleton.h"

#include "./camera.h"
#include "./gamePlay.h"
#include "./simon.h"
#include "./bone.h"

skeleton::skeleton(int id) : gameObjectEx(id) {

	this->type = "enemy";
	this->health = skeleton_health;

	this->assignAnimationSet("skeleton");

	aniState = 0;

	directionMoveX = Left;
}

void skeleton::spawnBullet() {
	LPGAMEOBJECT target = gamePlay::getInstance()->player;

	float dis = distanceXTo(target);
	float frame = skeleton_throw_speed_y / (skeleton_gravity * this->dt);
	float speedBulletX = dis / (frame * dt * 2.0f);

	bone* bullet = new bone(Counter::index++, speedBulletX,
		-skeleton_throw_speed_y, this->directionX);

	bullet->setPos(this->pos_x + this->getWidth(),
		this->pos_y - bullet->getHeight());

	gamePlay::getInstance()->objects.push_back(bullet);
}

void skeleton::shoot() {
	LPGAMEOBJECT target = gamePlay::getInstance()->player;

	if (target->canTouch == false)
		return;

	if (GetTickCount64() - shootTime >= skeleton_shoot_time) {

		if (this->distanceXTo(target) < skeleton_in_areaX) {
			if (GetTickCount64() - shootTimeEach >= sheleton_shoot_time_each) {

				if (shootCount >= skeleton_shoot_count_each) {
					shootTime = GetTickCount64();

					shootCount = 0;
				}
				else {
					this->spawnBullet();

					shootTimeEach = GetTickCount64();
					shootCount++;
				}
			}
		}
	}
}

float skeleton::distanceXTo(LPGAMEOBJECT obj) {
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

LPGAMEOBJECT skeleton::isGround(float x, float y) {
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
			return arr[i];
		}
	}
	return NULL;
}

void skeleton::moveToPointX(float x) {
	float norm = 0.0f;

	if (std::abs(x - this->pos_x) == 0.0f) {
		this->directionMoveX = Left;
		speed_x = skeleton_speedX * this->directionMoveX;
		return;
	}
	else
		norm = (x - this->pos_x) / std::abs(x - this->pos_x);

	if (norm > 0)
		this->directionMoveX = Right;
	else if (norm < 0)
		this->directionMoveX = Left;

	speed_x = this->directionMoveX * skeleton_speedX;
}

void skeleton::doSpecialJump() {
	gamePlay* gameInstance = gamePlay::getInstance();

	int pad = 0;

	int cellW = gameInstance->cellWidth;
	int cellH = gameInstance->cellHeight;
	int gridX = gameInstance->gridCountX;

	int id1, id2, id3, id4, id5, id6, id7, id8;

	if (this->directionMoveX == Right) {
		id1 = game::getCell(pos_x + bbox_w + pad, pos_y - pad, cellW, cellH, gridX);
		id2 = game::getCell(pos_x + bbox_w + pad, pos_y + bbox_h + pad, cellW, cellH, gridX);
		id3 = id1 + gridX;
		id4 = id2 - gridX;
		id5 = id1 + 1;
		id6 = id2 + 1;
		id7 = id3 + 1;
		id8 = id4 + 1;
	}
	else if (this->directionMoveX == Left) {
		id1 = game::getCell(pos_x - pad, pos_y - pad, cellW, cellH, gridX);
		id2 = game::getCell(pos_x - pad, pos_y + bbox_h + pad, cellW, cellH, gridX);
		id3 = id1 + gridX;
		id4 = id2 - gridX;
		id5 = id1 - 1;
		id6 = id2 - 1;
		id7 = id3 - 1;
		id8 = id4 - 1;
	}
	vector<LPGAMEOBJECT> arr;

	vector<LPGAMEOBJECT> arr1 = gameInstance->surfaceGrid[id1];
	vector<LPGAMEOBJECT> arr2 = gameInstance->surfaceGrid[id2];
	vector<LPGAMEOBJECT> arr3 = gameInstance->surfaceGrid[id3];
	vector<LPGAMEOBJECT> arr4 = gameInstance->surfaceGrid[id4];
	vector<LPGAMEOBJECT> arr5 = gameInstance->surfaceGrid[id5];
	vector<LPGAMEOBJECT> arr6 = gameInstance->surfaceGrid[id6];
	vector<LPGAMEOBJECT> arr7 = gameInstance->surfaceGrid[id7];
	vector<LPGAMEOBJECT> arr8 = gameInstance->surfaceGrid[id8];

	arr.reserve(arr1.size() + arr2.size() + arr3.size() + arr4.size() +
		arr5.size() + arr6.size() + arr7.size() + arr8.size());

	arr.insert(arr.begin(), arr1.begin(), arr1.end());
	arr.insert(arr.begin(), arr2.begin(), arr2.end());
	arr.insert(arr.begin(), arr3.begin(), arr3.end());
	arr.insert(arr.begin(), arr4.begin(), arr4.end());
	arr.insert(arr.begin(), arr5.begin(), arr5.end());
	arr.insert(arr.begin(), arr6.begin(), arr6.end());
	arr.insert(arr.begin(), arr7.begin(), arr7.end());
	arr.insert(arr.begin(), arr8.begin(), arr8.end());

	float endDistance = skeleton_special_jump_limit;
	float endHeight = 0.0f;

	D3DXVECTOR2 endPoint;

	for (auto& object : arr) {

		if (object->tag != "ground" || object->tag_id == this->standingGround->tag_id)
			continue;

		float distanceX = distanceXTo(object);
		float heightY = (this->pos_y + this->bbox_h) - object->getY();

		if (directionMoveX == Right && object->getX() > this->pos_x + this->bbox_w
			&& distanceX <= skeleton_special_jump_limit 
			&& heightY <= skeleton_special_jump_limit) {

			if (distanceX < endDistance) {
				endPoint = D3DXVECTOR2(object->getX(), object->getY());

				endDistance = distanceX;
				endHeight = heightY;
			}
		}
		else if (directionMoveX == Left && object->getX() + object->getWidth() < this->pos_x 
				&& distanceX <= skeleton_special_jump_limit 
				&& heightY <= skeleton_special_jump_limit) {

			if (distanceX < endDistance) {
				endPoint = D3DXVECTOR2(object->getX() + object->getWidth(), object->getY());

				endDistance = distanceX;
				endHeight = heightY;
			}
		}
	}

	if (endDistance != skeleton_special_jump_limit) {
		float jumpforce = endHeight + skeleton_jump_height_pad;

		D3DXVECTOR2 veloc = this->calcaulateLaunchForce(endPoint, skeleton_gravity * dt,
			jumpforce, dt, false);

		speed_x = veloc.x * this->directionMoveX;
		speed_y = veloc.y;
		speedJumpX = speed_x;
	}
	else {
		speed_x = skeleton_speedX * this->directionMoveX;
		speedJumpX = speed_x;

		speed_y = 0;
	}
}

int skeleton::observerAround() {
	float graveX = 0, graveY = 0;
	float groundX = 0, groundY = 0;

	if (this->directionMoveX == Right) {
		groundX = graveX = this->pos_x + this->bbox_w + skeleton_collision_point_pad;

		graveY = this->pos_y + this->bbox_h + skeleton_collision_point_pad;
		groundY = this->pos_y + this->bbox_h - skeleton_collision_point_pad;
	}
	else if (this->directionMoveX == Left) {
		groundX = graveX = this->pos_x - skeleton_collision_point_pad;

		graveY = this->pos_y + this->bbox_h + skeleton_collision_point_pad;
		groundY = this->pos_y + this->bbox_h - skeleton_collision_point_pad;
	}

	LPGAMEOBJECT ground = isGround(groundX, groundY);

	if (ground != NULL) {
		if (ground->getHeight() <= skeleton_ground_height) {
			return skeleton_special_jump;
		}
		else {
			this->limitXDirection = this->directionMoveX;

			return skeleton_touch_border;
		}
	}

	if (isGround(graveX, graveY) == NULL)
		return skeleton_special_jump;

	return skeleton_do_nothing;
}

void skeleton::chaseEnemy() {
	LPGAMEOBJECT target = gamePlay::getInstance()->player;

	if (!onTheGround)
		return;

	int index = rand() % skeleton_numpoint_rand;

	if (GetTickCount64() - randPointTime >= skeleton_rand_time)
		isRand = true;

	if (isRand == true) {
		if (this->pos_x <= target->getX() + target->getWidth()) {
			chaseX = target->getX() - skeleton_in_areaX;
			chaseX += index * skeleton_numpoint_distance;

			randPointTime = GetTickCount64();
			isRand = false;
		}
		else if (this->pos_x + this->bbox_w >= target->getX()) {
			chaseX = target->getX() + target->getWidth() + skeleton_in_areaX;
			chaseX -= index * skeleton_numpoint_distance;

			randPointTime = GetTickCount64();
			isRand = false;
		}
	}

	int signal = this->observerAround();

	if (limitXDirection != skeleton_direction_null) {
		if (limitXDirection == Left) {
			directionMoveX = Right;
		}
		else if (limitXDirection == Right) {
			directionMoveX = Left;
		}
		speed_x = skeleton_speedX * directionMoveX;

		return;
	}

	if (signal == skeleton_special_jump) {
		this->doSpecialJump();
		return;
	}

	this->moveToPointX(chaseX);
}

void skeleton::observeSimon() {
	LPGAMEOBJECT target = gamePlay::getInstance()->player;

	if (target->getX() + target->getWidth() >= this->pos_x)
		directionX = Right;
	else if (target->getX() <= this->pos_x + this->bbox_w)
		directionX = Left;
}

void skeleton::hurt(int damage) {
	this->health -= damage;

	if (this->health <= 0) {
		this->spawnDieEffect();

		isDestroyed = true;
	}
	this->spawnHitEffect();
}

void skeleton::calculateState(const BYTE* keyState, const BYTE* keyEvent) {
}

void skeleton::setState(int value) {
	state = value;
}

void skeleton::update(float dt) {
	this->dt = dt;
	speed_y += skeleton_gravity * dt;

	if (!onTheGround) {
		speed_x = speedJumpX;
		limitXDirection = skeleton_direction_null;
	}
	this->observeSimon();
	this->chaseEnemy();
	this->shoot();

	vector<string> AABBTag = { "torch", "candle", "flame" };
	vector<string> AABBType = { "enemy", "item", "portal", "subweapon" };

	this->updateSweptFilterAABB(game::makeFilterCollision(AABBTag),
		game::makeFilterCollision(AABBType), dt);

	camera* cam = camera::getInstance();

	float screen_x = this->pos_x - cam->getX();
	float screen_y = this->pos_y - cam->getY();

	if (screen_x + bbox_w < -skeleton_outside_screen || screen_x > wndWidth + skeleton_outside_screen ||
		screen_y + bbox_h < -skeleton_outside_screen || screen_y > wndHeight + skeleton_outside_screen) {
		isDestroyed = true;
	}
}

void skeleton::collisionHandle(LPCOLLISION colEvent, float dt) {
	LPGAMEOBJECT obj = colEvent->obj;

	if (obj->tag == "ground") {
		if (colEvent->ny == 1)
			this->removeCollisionBarrier(colEvent, dt);
	}

	if (obj->tag == "simon") {
		cast(obj, simon*)->hurt(colEvent->nx, colEvent->ny, skeleton_damage);

		this->removeCollisionBarrier(colEvent, dt);
	}
}

void skeleton::render() {
	this->animation[aniState]->render(pos_x, pos_y, directionX, 255);
}