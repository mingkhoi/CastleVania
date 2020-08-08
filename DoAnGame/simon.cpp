#include "./simon.h"
#include "./animationSet.h"
#include "./timeMachine.h"
#include "./axe.h"
#include "./database.h"
#include "./gamePlay.h"
#include "./knife.h"
#include "./portal.h"
#include "./score.h"
#include "./autoGoStair.h"
#include "./moneyMax.h"
#include "./moneyLv1.h"
#include "./moneyLv2.h"
#include "./moneyLv3.h"
#include "./camera.h"
#include "./SoundManager.h"
#include "./listGameobject.h"
#include "./doubleShot.h"
#include "./tripleShot.h"
#include "./cross.h"
#include "./treasure.h"
#include "./hidden.h"
#include "./crown.h"


simon::simon(int id) : gameObjectEx(id) {
	this->setWidth(simon_normal_width);
	this->setHeight(simon_normal_height);

	this->animation = animationSet::getInstance()->getAnimationSet("simon");

	weaponSpawner = new subWeaponSpawner();
	whips = new whip(Counter::index++);
	cast(whips, whip*)->setHero(this);

	animation[simon_ani_jump]->loop = false;
	animation[simon_ani_attack]->loop = false;
	animation[simon_ani_duck_attack]->loop = false;
	animation[simon_ani_dead]->loop = false;
	whipLv = simon_start_whip_level;

	hearts = score_default_player_hearts;
	lives = score_default_player_lives;
	health = score_default_player_health;
	scores = 0;

	subWeaponName = "";
	aniState = simon_ani_idle;
}

void simon::simonMovementHandle(const BYTE* keyState, const BYTE* keyEvent) {
	isMove = false;
	isJump = false;

	if (game::isKeyUp(keyState, DIK_DOWN) && (state == simon_state_duck ||
		state == simon_state_duck_attack)) {
		this->standUp();
	}

	if (game::isKeyDown(keyState, DIK_LEFT) && state != simon_state_duck) {
		if (isClimbing)
			this->climbingStair(Left, simon_stair_null);
		else if (onTheGround)
			this->walk(Left);
	}

	if (game::isKeyDown(keyState, DIK_RIGHT) && state != simon_state_duck) {
		if (isClimbing)
			this->climbingStair(Right, simon_stair_null);
		else if (onTheGround)
			this->walk(Right);
	}

	if (game::isKeyDown(keyState, DIK_UP)) {
		isArrowUp = true;

		if (portalStair != NULL && isClimbing) {
			this->climbingStair(Left, simon_stair_up);
		}
	}
	if (game::isKeyUp(keyState, DIK_UP))
		isArrowUp = false;

	if (game::isKeyDown(keyState, DIK_DOWN)) {
		isArrowDown = true;

		if (portalStair != NULL && isClimbing) {
			this->climbingStair(Left, simon_stair_down);
		}
		if (onTheGround && !isClimbing)
			setState(simon_state_duck);
	}
	if (game::isKeyUp(keyState, DIK_DOWN))
		isArrowDown = false;

	if (game::isEventDown(keyEvent, DIK_SPACE) && state != simon_state_duck
		&& onTheGround && !isClimbing) {
		lockLanding = false;
		fallFromHighGround = false;
		isJump = true;

		setState(simon_state_jump);
	}

	if (!isMove && !isJump && state != simon_state_duck && onTheGround
		&& !isClimbing) {

		setState(simon_state_idle);
	}

	// hack
	if (game::isEventDown(keyEvent, DIK_1)) {
		subWeaponName = "knife";
		subWeapon = listGameobject::getInstance()->reflect(subWeaponName);
	}
	else if (game::isEventDown(keyEvent, DIK_2)) {
		subWeaponName = "axe";
		subWeapon = listGameobject::getInstance()->reflect(subWeaponName);
	}
	else if (game::isEventDown(keyEvent, DIK_3)) {
		subWeaponName = "holyWater";
		subWeapon = listGameobject::getInstance()->reflect(subWeaponName);
	}
	else if (game::isEventDown(keyEvent, DIK_4)) {
		subWeaponName = "boomerang";
		subWeapon = listGameobject::getInstance()->reflect(subWeaponName);
	}
	else if (game::isEventDown(keyEvent, DIK_5)) {
		subWeaponName = "timeMachine";
		subWeapon = listGameobject::getInstance()->reflect(subWeaponName);
	}
	else if (game::isEventDown(keyEvent, DIK_U)) {
		if (this->item == NULL) {
			this->item = new doubleShot(Counter::index++);
		}
	}
	else if (game::isEventDown(keyEvent, DIK_I)) {
		if (this->item != NULL) {
			free(this->item);
			this->item = NULL;
		}
		this->item = new tripleShot(Counter::index++);
	}

	if (game::isEventDown(keyEvent, DIK_H)) {
		lives = score_default_player_lives;
		health = score_default_player_health;
	}
}

void simon::calculateState(const BYTE* keyState, const BYTE* keyEvent) {

	this->landingProcess();

	if (state == simon_state_jump) {
		if (animation[aniState]->remainFrame() == 2)
			bbox_h = simon_normal_height;

		speed_x = jumpSpeedX;
	}

	if (state == simon_state_detect_stair) {
		if (portalStair != NULL) {
			this->detectStair();
		}
		else
			return;
	}

	if (state == simon_state_auto_walk) {
		speed_x = simon_move_speed * 0.5f * directionX;
		gravity = false;
		speed_y = 0;

		return;
	}

	if (state == simon_state_intro)
		return;

	if (state == simon_state_dead) {
		if (animation[aniState]->remainFrame() == 2) {
			pos_y += (bbox_h - simon_death_height_1);
			bbox_h = simon_death_height_1;
		}
		else if (animation[aniState]->remainFrame() == 1) {
			pos_y += (bbox_h - simon_death_height_2);
			bbox_h = simon_death_height_2;
		}
		return;
	}

	if (autoStairWalk)
		this->autoGoStairX(this->touchStairX);

	if (isAttack == true || state == simon_state_hurt || autoStairWalk || sceneTransition)
		return;

	if (!onTheGround && state == simon_state_walk) {
		setState(simon_state_idle);
	}

	if (!isLanding)
		this->simonMovementHandle(keyState, keyEvent);
	if (isClimbing) this->stairProcess();

	if (game::isKeyDown(keyState, DIK_Q))
		this->useWhip();

	if (game::isKeyDown(keyState, DIK_W) && !game::gameFreeze)
		this->useSubWeapon();
}

void simon::landingProcess() {
	// simon landing detect

	if (speed_y >= simon_landing_speedY && !fallFromHighGround)
		fallFromHighGround = true;

	if ((state == simon_state_hurt || state == simon_state_duck || isClimbing) 
		&& (onTheGround || isClimbing) &&
		isHurted && !lockLanding) {

		landingStartTime = GetTickCount64();
		lockLanding = true;
		isLanding = true;
	}

	if (health <= 0) {
		landingStartTime = GetTickCount64();
		lockLanding = true;
		isLanding = true;
	}

	if (fallFromHighGround && onTheGround && !lockLanding) {

		landingStartTime = GetTickCount64();
		lockLanding = true;
		isLanding = true;
	}

	// landing process

	if (isLanding == true) {
		if (GetTickCount64() - landingStartTime >= simon_landing_time) {
			isLanding = false;
		}
		else {
			if (health <= 0) {
				if (pos_y <= gamePlay::getInstance()->rootY + wndHeight - bbox_h) {
					if (onTheGround)
						setState(simon_state_dead);
				}
				else setState(simon_state_dead);
			}
			else if (onTheGround) {
				if (!isAttack)
					setState(simon_state_duck);
			}
		}
	}
}

void simon::standUp() {
	pos_y -= (simon_normal_height - bbox_h);
	bbox_h = simon_normal_height;

	setState(simon_state_idle);
}

void simon::duck() {
	pos_y += (bbox_h - simon_duck_height);
	bbox_h = simon_duck_height;
}

void simon::walk(int direction) {
	isMove = true;
	directionX = direction;

	setState(simon_state_walk);
}

bool simon::checkStair() {
	bool a = game::isSamePoint(pos_x, nextStairX, speed_x);
	bool b = game::isSamePoint(pos_y, nextStairY, speed_y);

	return a && b;
}

void simon::detectStair() {
	this->animation[simon_ani_descend]->reset();
	this->animation[simon_ani_ascend]->reset();

	float heightDifference = this->portalStair->getHeight() - this->getHeight();

	this->directionX = portalStair->getDirectionX();
	nextStairMove = true;

	if (this->portalStair->tag == "up stair") {
		this->pos_y = portalStair->getY() + heightDifference + simon_stair_size;
		this->pos_x = portalStair->getX();

		this->climbingStair(Left, simon_stair_down);
	}
	else if (this->portalStair->tag == "down stair") {
		this->pos_y = portalStair->getY() + heightDifference;
		this->pos_x = portalStair->getX();

		this->climbingStair(Left, simon_stair_up);
	}

	isClimbing = true;
}


void simon::stairProcess() {
	if (nextStairMove)
		return;

	if (checkStair()) {
		pos_x = nextStairX;
		pos_y = nextStairY;
		this->setState(simon_state_idle_stair);

		nextStairMove = true;
	}
	else {
		if (this->stairMode == simon_stair_up)
			this->setState(simon_state_ascend);
		else if (this->stairMode == simon_stair_down)
			this->setState(simon_state_descend);
	}
}

void simon::climbingStair(int direction, int upDown) {

	if (nextStairMove == false)
		return;

	if (upDown == simon_stair_up) {
		this->stairMode = simon_stair_up;
	}
	else if (upDown == simon_stair_down) {
		this->stairMode = simon_stair_down;
	}
	else {
		if (portalStair->tag == "down stair") {
			if (direction == portalStair->getDirectionX())
				this->stairMode = simon_stair_up;
			else
				this->stairMode = simon_stair_down;
		}
		else if (portalStair->tag == "up stair") {
			if (direction == portalStair->getDirectionX())
				this->stairMode = simon_stair_down;
			else
				this->stairMode = simon_stair_up;
		}
	}

	if (this->stairMode == simon_stair_up)
		this->setState(simon_state_ascend);
	else if (this->stairMode == simon_stair_down)
		this->setState(simon_state_descend);

	nextStairX = pos_x + (directionX * simon_stair_size);

	if (this->stairMode == simon_stair_up)
		nextStairY = pos_y - simon_stair_size;
	else if (this->stairMode == simon_stair_down)
		nextStairY = pos_y + simon_stair_size;

	nextStairMove = false;
}

void simon::autoGoStairX(float point) {
	if (isClimbing)
		return;

	if (!hasEnterAutoStair) {
		if (point - this->pos_x < 0) {
			hasEnterAutoStair = true;
			directionX = Left;
		}
		else if (point - this->pos_x > 0) {
			hasEnterAutoStair = true;
			directionX = Right;
		}
		else {
			pos_x = point;
			autoStairWalk = false;
			hasEnterAutoStair = false;

			this->detectStair();
			return;
		}
	}

	setState(simon_state_walk);

	if (directionX == Left) {
		if (this->pos_x <= point || std::abs(pos_x - point) <= surfaceNormalForce) {
			pos_x = point;
			autoStairWalk = false;
			hasEnterAutoStair = false;

			this->detectStair();
		}
	}
	else if (directionX == Right) {
		if (this->pos_x >= point || std::abs(pos_x - point) <= surfaceNormalForce) {
			pos_x = point;
			autoStairWalk = false;
			hasEnterAutoStair = false;

			this->detectStair();
		}
	}
}

void simon::useSubWeapon() {
	if (this->hearts <= 0 || subWeaponName == "" || subWeapon == NULL)
		return;

	if (isAttack == true)
		return;

	if (state == simon_state_jump || state == simon_state_walk || state == simon_state_idle) {
		isAttack = true;

		setState(simon_state_attack);
		weaponSpawner->spawn(subWeaponName, this->directionX);
	}
	else if (state == simon_state_duck) {
		isAttack = true;

		setState(simon_state_duck_attack);
		weaponSpawner->spawn(subWeaponName, this->directionX);
	}
	else if (state == simon_state_idle_ascend) {
		isAttack = true;

		setState(simon_state_ascend_attack);
		weaponSpawner->spawn(subWeaponName, this->directionX);
	}
	else if (state == simon_state_idle_descend) {
		isAttack = true;

		setState(simon_state_descend_attack);
		weaponSpawner->spawn(subWeaponName, this->directionX);
	}
}

void simon::useWhip() {
	if (isAttack == true)
		return;

	if (state == simon_state_jump || state == simon_state_walk || state == simon_state_idle) {
		isAttack = true;

		setState(simon_state_attack);
		cast(whips, whip*)->spawn(this->whipLv, this->directionX);
	}
	else if (state == simon_state_duck) {
		isAttack = true;

		setState(simon_state_duck_attack);
		cast(whips, whip*)->spawn(this->whipLv, this->directionX);
	}
	else if (state == simon_state_idle_ascend) {
		isAttack = true;

		setState(simon_state_ascend_attack);
		cast(whips, whip*)->spawn(this->whipLv, this->directionX);
	}
	else if (state == simon_state_idle_descend) {
		isAttack = true;

		setState(simon_state_descend_attack);
		cast(whips, whip*)->spawn(this->whipLv, this->directionX);
	}
}

void simon::startCantTouch() {
	SoundManager::GetInstance()->playSoundEffect("invisible");
	cannotTouchStartTime = GetTickCount64();
	cannotTouch = true;
}

bool simon::hurt(float nx, float ny, int damage) {
	if (isHurted == true || health <= 0)
		return false;

	SoundManager::GetInstance()->playSoundEffect("hurt");

	isHurted = true;
	lockLanding = false;

	isArrowDown = false;
	isArrowUp = false;

	cast(whips, whip*)->pullBack();
	cast(weaponSpawner, subWeaponSpawner*)->pullBack();

	hurtStartTime = GetTickCount64();
	health -= damage;

	if (health <= 0) {
		isClimbing = false;
		bbox_h = simon_normal_height;
		state = simon_state_null;
	}

	if (isAttack == true) {
		if (isClimbing) {
			this->setState(simon_state_idle_stair);
		}
		isAttack = false;
	}

	if (!isClimbing && state != simon_state_duck) {
		bbox_h = simon_normal_height;

		if (nx != 0)
			directionX = nx;

		if (nx != 0)
			speed_x = simon_hurt_speedX * directionX * (-1.0f);
		else
			speed_x = 0;

		speed_y = simon_hurt_speedY * (-1.0f);

		setState(simon_state_hurt);
	}

	return true;
}

void simon::setState(int value) {
	state = value;

	switch (value) {
	case simon_state_intro:
		speed_x = 0;
		aniState = simon_ani_intro;

		break;
	case simon_state_auto_walk:
		speed_x = simon_move_speed * 0.5f * directionX;
		speed_y = 0;

		gravity = false;

		aniState = simon_ani_walk;

		break;
	case simon_state_idle:
		speed_x = 0;

		if (!isJump)
			aniState = simon_ani_idle;

		break;
	case simon_state_walk:
		aniState = simon_ani_walk;
		speed_x = simon_move_speed * directionX;

		break;
	case simon_state_jump:
		speed_y = -simon_jump_speed;
		jumpSpeedX = speed_x;

		aniState = simon_ani_jump;
		bbox_h = simon_duck_height;

		animation[aniState]->reset();

		break;
	case simon_state_duck:
		speed_x = 0;
		if (bbox_h != simon_duck_height)
			this->duck();
		aniState = simon_ani_duck;

		break;
	case simon_state_attack:
		bbox_h = simon_normal_height;
		aniState = simon_ani_attack;

		animation[aniState]->reset();

		break;
	case simon_state_duck_attack:
		aniState = simon_ani_duck_attack;
		animation[aniState]->reset();

		break;
	case simon_state_ascend_attack:
		speed_x = speed_y = 0;

		aniState = simon_ani_ascend_attack;
		animation[aniState]->reset();

		break;
	case simon_state_descend_attack:
		speed_x = speed_y = 0;

		aniState = simon_ani_descend_attack;
		animation[aniState]->reset();

		break;
	case simon_state_ascend:
		this->directionX = portalStair->getDirectionX();

		aniState = simon_ani_ascend;
		animation[aniState]->pause = false;

		if (portalStair->tag == "up stair")
			this->directionX *= -1;

		speed_x = simon_stair_speedX * directionX;
		speed_y = -simon_stair_speedY;

		break;
	case simon_state_descend:
		this->directionX = -portalStair->getDirectionX();

		aniState = simon_ani_descend;
		animation[aniState]->pause = false;

		if (portalStair->tag == "up stair")
			this->directionX *= -1;

		speed_x = simon_stair_speedX * directionX;
		speed_y = simon_stair_speedY;

		break;
	case simon_state_idle_stair:
		if (portalStair->tag == "down stair") {
			if (directionX == portalStair->getDirectionX()) {
				aniState = simon_ani_ascend;
				state = simon_state_idle_ascend;
			}
			else {
				aniState = simon_ani_descend;
				state = simon_state_idle_descend;
			}
		}
		else if (portalStair->tag == "up stair") {
			if (directionX == portalStair->getDirectionX()) {
				aniState = simon_ani_descend;
				state = simon_state_idle_descend;
			}
			else {
				aniState = simon_ani_ascend;
				state = simon_state_idle_ascend;
			}
		}
		animation[aniState]->reset();
		animation[aniState]->pause = true;

		speed_x = speed_y = 0;

		break;
	case simon_state_detect_stair:

		break;
	case simon_state_hurt:
		aniState = simon_ani_hurt;

		animation[aniState]->reset();

		break;
	case simon_state_dead:
		speed_x = speed_y = 0;

		if (aniState != simon_ani_dead) {
			SoundManager::GetInstance()->stop();
			SoundManager::GetInstance()->playSoundEffect("die");

			aniState = simon_ani_dead;

			animation[aniState]->reset();
		}
		break;
	}
}

void simon::update(float dt) {

	if (cannotTouch) {
		if (GetTickCount64() - cannotTouchStartTime >= simon_cannot_touch_time) {
			SoundManager::GetInstance()->playSoundEffect("invisible");
			cannotTouch = false;
			canTouch = true;
		}
		else canTouch = false;
	}

	if (isHurted) {
		if (GetTickCount64() - hurtStartTime >= simon_hurt_time) {
			canTouch = true;
			isHurted = false;
		}
		else canTouch = false;
	}

	if (isAttack) {
		if (onTheGround)
			speed_x = 0;
		if (animation[aniState]->isDone()) {
			if (isClimbing)
				this->setState(simon_state_idle_stair);
			else if (bbox_h == simon_duck_height)
				this->setState(simon_state_duck);

			cast(whips, whip*)->pullBack();
			isAttack = false;
		}
	}

	if (score::getInstance()->stages == 0) {
		if (aniState == simon_ani_intro && animation[aniState]->isDone()) {
			gamePlay::getInstance()->loadNextScene = true;

			this->setState(simon_state_idle);
		}
	}

	if (score::getInstance()->time <= 0) {
		this->lives = 0;
		this->hurt(0, 0, score_default_player_health);
	}

	if (!isClimbing && gravity) {
		speed_y += simon_gravity * dt;
	}

	if (hearts == 0) {
		subWeaponName = "";

		free(subWeapon);
		subWeapon = NULL;
	}

	if (aniState == simon_ani_dead) {
		if (animation[aniState]->isDone()) {
			animation[aniState]->reset();
			isDestroyed = true;
		}
	}

	if (standingGround != NULL && standingGround->type == "moving ground"
		&& onTheGround && state == simon_state_idle) {
		speed_x = standingGround->getSpeedX();
	}

	// start collision

	vector<string> AABBTag = { "torch", "candle", "flame", "bossBall" };
	vector<string> AABBType = { "enemy", "item", "portal", "subweapon" };

	this->updateSweptFilterAABB(game::makeFilterCollision(AABBTag),
		game::makeFilterCollision(AABBType), dt);

	// limit position

	gamePlay* mainGame = gamePlay::getInstance();

	if (!mainGame->startBossMode) {
		if (pos_x < mainGame->rootX)
			pos_x = mainGame->rootX;
		else if (pos_x > mainGame->rootX + mainGame->currentTileSet->mapWidth - bbox_w)
			pos_x = mainGame->rootX + mainGame->currentTileSet->mapWidth - bbox_w;
	}
	else {
		float limitPosX = gamePlay::Instance->rootX + gamePlay::Instance->currentTileSet->mapWidth;
		float limitNegX = gamePlay::Instance->rootX + gamePlay::Instance->currentTileSet->mapWidth - wndWidth;

		if (pos_x < limitNegX)
			pos_x = limitNegX;
		else if (pos_x > limitPosX - bbox_w)
			pos_x = limitPosX - bbox_w;
	}

	if (pos_y > wndHeight + simon_out_screen) {
		this->hurt(0, 0, score_default_player_health);
	}

	// update HUD

	score::getInstance()->update(this);

	// update weapon

	weaponSpawner->update();
	whips->update(dt);
}

void simon::changeScene() {
	this->portalStair = NULL;

	speed_x = speed_y = 0;

	bbox_h = simon_normal_height;
	gravity = true;

	fallFromHighGround = false;
	canTouch = true;
	lockLanding = true;
	isLanding = false;
	isClimbing = false;
	isAttack = false;
	isHurted = false;
}

void simon::resetState() {
	this->portalStair = NULL;

	speed_x = speed_y = 0;

	bbox_h = simon_normal_height;
	gravity = true;

	fallFromHighGround = false;
	canTouch = true;
	lockLanding = true;
	isLanding = false;
	isClimbing = false;
	isAttack = false;
	isHurted = false;

	this->health = score_default_player_health;
	this->hearts = score_default_player_hearts;

	if (this->subWeapon != NULL) {
		free(this->subWeapon);
		this->subWeapon = NULL;
	}
	if (this->item != NULL) {
		free(this->item);
		this->item = NULL;
	}
	this->subWeaponName = "";

	score::getInstance()->update(this);
}

void simon::collisionHandle(LPCOLLISION colEvent,
	float dt) {
	LPGAMEOBJECT object = colEvent->obj;

	if (colEvent->tag == AABBCollision) {
		// collide with item

		if (object->type == "item" && !object->isDestroyed) {
			if (object->tag == "axe" || object->tag == "knife" || object->tag == "holyWater"
				|| object->tag == "boomerang" || object->tag == "timeMachine") {
				object->isDestroyed = true;

				subWeaponName = object->tag;
				subWeapon = listGameobject::getInstance()->reflect(subWeaponName);
			}
			else if (object->tag == "crown") {
				cast(object, crown*)->die();
				this->scores += _point_7000;
			}
			else if (object->tag == "treasure") {
				cast(object, treasure*)->die();
				this->scores += _point_7000;
			}
			else if (object->tag == "chicken") {
				this->health += simon_chicken;

				if (this->health >= score_default_player_health) {
					this->health = score_default_player_health; 
				}
				object->isDestroyed = true;
			}
			else if (object->tag == "moneyMax") {
				cast(object, moneyMax*)->die();
				this->scores += _point_2000;
			}
			else if (object->tag == "moneyLv1") {
				cast(object, moneyLv1*)->die();
				this->scores += _point_100;
			}
			else if (object->tag == "moneyLv2") {
				cast(object, moneyLv2*)->die();
				this->scores += _point_400;
			}
			else if (object->tag == "moneyLv3") {
				cast(object, moneyLv3*)->die();
				this->scores += _point_700;
			}
			else if (object->tag == "invisible") {
				this->startCantTouch();
				object->isDestroyed = true;
			}
			else if (object->tag == "smallHeart") {
				object->isDestroyed = true;
				this->hearts += simon_small_heart;
			}
			else if (object->tag == "bigHeart") {
				object->isDestroyed = true;
				this->hearts += simon_big_heart;
			}
			else if (object->tag == "doubleShot") {
				object->isDestroyed = true;

				if (this->item == NULL) {
					this->item = new doubleShot(Counter::index++);
				}
			}
			else if (object->tag == "cross") {
				object->isDestroyed = true;

				gamePlay::getInstance()->clearScreen();
			}
			else if (object->tag == "tripleShot") {
				object->isDestroyed = true;

				if (this->item != NULL) {
					free(this->item);
					this->item = NULL;
				}
				this->item = new tripleShot(Counter::index++);
			}
			else if (object->tag == "bow") {
				object->isDestroyed = true;
				this->whipLv++;

				if (this->whipLv >= 3)
					this->whipLv = simon_max_whip_level;
			}
		}
		if (object->tag == "hidden" && !object->isDestroyed) {
			cast(object, hidden*)->spawnObject();
		}
		if (object->tag == "end scene" && !isClimbing && !game::gameFreeze) {
			if (state != simon_state_hurt) {
				gamePlay::getInstance()->loadNextScene = true;
				standingGround = NULL;
			}
		}
		if (object->tag == "end scene transition start" && !isClimbing && !game::gameFreeze) {
			if (state != simon_state_hurt) {
				gamePlay::getInstance()->loadNextScenePanCam = true;
				standingGround = NULL;

				this->directionX = object->getDirectionX();
				this->bbox_h = simon_normal_height;

				this->isAttack = false;
				cast(this->whips, whip*)->pullBack();

				float heightDif = object->getHeight() - this->bbox_h;
				this->pos_y = object->getY() + heightDif - surfaceNormalForce;

				this->setState(simon_state_auto_walk);
				this->sceneTransition = true;
			}
		}
		if (object->tag == "end scene transition end" && this->sceneTransition) {
			this->setState(simon_state_idle);

			if (camera::getInstance()->autoPan == false) {
				this->pos_y -= surfaceNormalForce * 10;
				this->sceneTransition = false;
				this->gravity = true;
			}
		}
		if (object->tag == "previous scene" && !isClimbing) {
			if (state != simon_state_hurt) {
				gamePlay::getInstance()->loadPreviousScene = true;
				standingGround = NULL;
			}
		}
		// collide with stair portal

		if (object->tag == "end stair" && isClimbing == true &&
			this->directionX != object->getDirectionX()) {
			if (state == simon_state_idle_ascend || state == simon_state_ascend)
				pos_y -= simon_up_stair_force;

			isClimbing = false;
			portalStair = NULL;
		}

		if (object->tag == "end auto intro" && state != simon_state_intro) {
			this->setState(simon_state_intro);

			gravity = true;
		}

		if (object->tag == "auto stair" && !isClimbing && onTheGround && !hasEnterAutoStair) {
			bool condition1 = (cast(object, autoGoStair*)->checkDownStair() && isArrowUp);
			bool condition2 = (cast(object, autoGoStair*)->checkUpStair() && isArrowDown);

			if (condition1) {
				this->portalStair = (cast(object, autoGoStair*))->getDownStair();

				this->touchStairX = cast(object, autoGoStair*)->toXDownStair;
				this->autoStairWalk = true;
			}
			else if (condition2) {
				if (this->bbox_h == simon_duck_height)
					this->standUp();

				this->portalStair = (cast(object, autoGoStair*))->getUpStair();

				this->touchStairX = cast(object, autoGoStair*)->toXUpStair;
				this->autoStairWalk = true;
			}
		}

		if (!isClimbing) {
			if (state == simon_state_detect_stair) {
				if (object->tag == "down stair" || object->tag == "up stair")
					this->portalStair = object;
			}
		}
	}
	else {
		if (isClimbing) {
			this->removeCollisionBarrier(colEvent, dt);
			return;
		}

		if (object->tag == "ground") {
			if (colEvent->ny == 1 || this->sceneTransition)
				this->removeCollisionBarrier(colEvent, dt);

			if (!onTheGround) {
				this->speed_y = this->collisionOldSpeedY;
			}
		}
	}
}

void simon::render() {
	if (gamePlay::getInstance()->effectPause) {
		if (aniState >= 0) {
			if (isHurted == true || cannotTouch == true)
				this->animation[aniState]->renderNoPause(pos_x, pos_y, directionX, simon_transparent);
			else
				this->animation[aniState]->renderNoPause(pos_x, pos_y, directionX, simon_normal_color);
		}
	}
	else {
		if (aniState >= 0) {
			if (isHurted == true || cannotTouch == true)
				this->animation[aniState]->render(pos_x, pos_y, directionX, simon_transparent);
			else
				this->animation[aniState]->render(pos_x, pos_y, directionX, simon_normal_color);
		}
	}
	whips->render();
}