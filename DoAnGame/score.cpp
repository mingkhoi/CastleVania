#include "./score.h"
#include "./animationSet.h"
#include "./camera.h"
#include "./define.h"
#include "./simon.h"
#include "./texture.h"
#include "./SoundManager.h"
#include "./gamePlay.h"

score* score::Instance;

string to_6_digit(int num) {
	string res = to_string(num);

	while (res.size() < score_6_digit)
		res.insert(0, "0");
	return res;
}

string to_3_digit(int num) {
	string res = to_string(num);

	while (res.size() < score_3_digit)
		res.insert(0, "0");
	return res;
}

string to_2_digit(int num) {
	string res = to_string(num);

	while (res.size() < score_2_digit)
		res.insert(0, "0");
	return res;
}

score::score() {
	D3DXCreateFont(game::getInstance()->getDevice(), 15, 0, 600, -1, false,
		UNICODE, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DECORATIVE,
		L"", &this->Font);

	SetRect(&scoreRect, score_score_rect);
	SetRect(&playerRect, score_player_rect);
	SetRect(&enemyRect, score_enemy_rect);
	SetRect(&timeRect, score_time_rect);
	SetRect(&stageRect, score_stage_rect);
	SetRect(&heartRect, score_heart_rect);
	SetRect(&liveRect, score_live_rect);

	data = animationSet::getInstance()->getAnimationSet("score_board");

	subWeaponRect = data[score_subWeapon_rect_ani]->getFrame(0);
	healthBarEnemy = data[score_health_bar_enemy_ani]->getFrame(0);
	healthBarHero = data[score_health_bar_hero_ani]->getFrame(0);
	healthBarNone = data[score_health_bar_none_ani]->getFrame(0);
	heart = data[score_heart_ani]->getFrame(0);

	startTime = GetTickCount64();
	this->lives = score_default_player_lives;
	this->playerHealth = score_default_player_health;
	this->BossHealth = score_default_boss_health;
	this->time = score_default_time;
	this->scoreVal = 0;
	this->hearts = 0;
	this->stages = 1;

	this->subWeapon = NULL;
	this->item = NULL;
}

void score::setPlayer(LPGAMEOBJECT player) {
	this->player = player;
}

score* score::getInstance() {
	if (Instance == NULL)
		Instance = new score();
	return Instance;
}

void score::update(LPGAMEOBJECT hero) {
	simon* gameHero = cast(hero, simon*);

	subWeapon = gameHero->subWeapon;
	item = gameHero->item;

	lives = gameHero->lives;
	hearts = gameHero->hearts;
	scoreVal = gameHero->scores;
	playerHealth = gameHero->health;
}

void score::updateEnemy(int enemyHealth) {
	this->BossHealth = enemyHealth;
}

void score::updateSubWeapon(LPGAMEOBJECT value) {
	subWeapon = value;
}

void score::updateItem(LPGAMEOBJECT value) {
	item = value;
}

void score::resetScoreBoard() {
	startTime = GetTickCount64();

	this->endGame = this->startEnd = false;

	this->lives = score_default_player_lives;
	this->playerHealth = score_default_player_health;
	this->BossHealth = score_default_boss_health;
	this->time = score_default_time;
	this->scoreVal = 0;
	this->hearts = 0;
	this->stages = 1;

	this->subWeapon = NULL;
	this->item = NULL;
}

void score::resetBossHealth() {
	this->BossHealth = score_default_boss_health;
}

void score::process(float dt) {
	string scene_name = (string)gamePlay::getInstance()->currentSceneNode->name();
	if (gamePlay::getInstance()->currentSceneID <= 1 || scene_name == "end_game")
		return;

	if (this->endGame) {
		if (startEnd == false) {
			waitEndGame = GetTickCount64();
			startEnd = true;
		}
		if (GetTickCount64() - waitEndGame >= score_waitEnd) {
			game::gameFreeze = true;
		}

		if (GetTickCount64() - scoreTick >= score_end_tick &&
			GetTickCount64() - waitEndGame >= score_waitEnd) {

			if (this->pointDecrease == 0) {
				this->pointDecrease = score_decrease; 
			}
			this->pointDecrease += (this->pointDecrease * dt);

			this->scoreVal -= this->pointDecrease;
			this->scoreTick = GetTickCount64();

			if (scoreVal <= 0) {
				scoreVal = 0;

				if (GetTickCount() - this->waitGameover >= score_waitGameover) {
					gamePlay::getInstance()->loadScene(gamePLay_load_next_scene);
					return;
				}
			}
			else {
				SoundManager::GetInstance()->playSoundEffect("score");
				this->waitGameover = GetTickCount64();
			}
		}
	}

	if (GetTickCount64() - this->startTime >= score_1_second && BossHealth > 0) {
		this->startTime = GetTickCount64();
		this->time--;

		if (this->time <= score_low_range) {
			if (GetTickCount64() - this->startLowRange >= score_low_range_freq) {
				SoundManager::GetInstance()->playSoundEffect("timeout");
				this->startLowRange = GetTickCount64();
			}
		}
		if (this->time <= 0) {
			this->time = 0;
		}
	}
}

void score::render() {

	string scene_name = (string)gamePlay::getInstance()->currentSceneNode->name();
	if (gamePlay::getInstance()->currentSceneID <= 1 || scene_name == "end_game")
		return;

	// draw subWeapon rect + heart

	game::getInstance()->drawNoCam(subWeaponRect->getTexture(), &subWeaponRect->getSourceRect(),
		score_subWeapon_rect_pos, Left, score_color_default);

	game::getInstance()->drawNoCam(heart->getTexture(), &heart->getSourceRect(),
		score_heart_screen_pos, Left, score_color_default);

	// draw subWeapon

	if (hearts <= 0)
		subWeapon = NULL;
	else if (subWeapon != NULL) {
		LPSPRITE subWeaponSprite = subWeapon->getAnimation(0)->getFrame(0);

		game::getInstance()->drawNoCam(subWeaponSprite->getTexture(), &subWeaponSprite->getSourceRect(),
			score_subWeapon_pos, Left, score_color_default);
	}

	// draw item

	if (item != NULL) {
		LPSPRITE itemSprite = item->getAnimation(0)->getFrame(0);
		game::getInstance()->drawNoCam(itemSprite->getTexture(), &itemSprite->getSourceRect(),
			score_item_pos, Left, score_color_default);
	}

	// convert numer to display-string

	string _score = "SCORE-" + to_6_digit(scoreVal);
	string _time = "TIME " + to_3_digit(time);
	string _stage = "STAGE " + to_2_digit(stages);
	string _live = "P -" + to_2_digit(lives);
	string _heart = "-" + to_2_digit(hearts);

	// draw text

	Font->DrawTextA(NULL, _score.c_str(), -1, &scoreRect, DT_LEFT, score_color_default);
	Font->DrawTextA(NULL, _time.c_str(), -1, &timeRect, DT_LEFT, score_color_default);
	Font->DrawTextA(NULL, _stage.c_str(), -1, &stageRect, DT_LEFT, score_color_default);
	Font->DrawTextA(NULL, _heart.c_str(), -1, &heartRect, DT_LEFT, score_color_default);
	Font->DrawTextA(NULL, _live.c_str(), -1, &liveRect, DT_LEFT, score_color_default);
	Font->DrawTextA(NULL, "PLAYER ", -1, &playerRect, DT_LEFT, score_blue);
	Font->DrawTextA(NULL, "ENEMY ", -1, &enemyRect, DT_LEFT, score_red);

	// draw health bar

	float pos_x = score_start_health_posX;

	for (int i = 1; i <= playerHealth; i++) {
		pos_x += score_healthW + 1;
		game::getInstance()->drawNoCam(healthBarHero->getTexture(), &healthBarHero->getSourceRect(), pos_x,
			score_start_health_posY, Left, score_color_default);
	}
	for (int i = playerHealth + 1; i <= score_max_health; i++) {
		if (i <= 0)
			continue;

		pos_x += score_healthW + 1;
		game::getInstance()->drawNoCam(healthBarNone->getTexture(), &healthBarNone->getSourceRect(), pos_x,
			score_start_health_posY, Left, score_color_default);
	}

	pos_x = score_start_health_posX;
	for (int i = 1; i <= BossHealth; i++) {
		pos_x += score_healthW + 1;
		game::getInstance()->drawNoCam(healthBarEnemy->getTexture(), &healthBarEnemy->getSourceRect(), pos_x,
			score_start_health_posY + score_healthH, Left, score_color_default);
	}
	for (int i = BossHealth + 1; i <= score_max_health; i++) {
		if (i <= 0)
			continue;

		pos_x += score_healthW + 1;
		game::getInstance()->drawNoCam(healthBarNone->getTexture(), &healthBarNone->getSourceRect(), pos_x,
			score_start_health_posY + score_healthH, Left, score_color_default);
	}
}