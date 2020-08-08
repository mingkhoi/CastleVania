#pragma once
#include "./animation.h"
#include "./gameObject.h"

#define score_subWeapon_rect_pos 145, 16
#define score_heart_screen_pos 183, 16
#define score_subWeapon_pos 156.5f, 23.5f
#define score_item_pos 235, 15.25f

#define score_start_health_posX 50
#define score_start_health_posY 16
#define score_healthW 4
#define score_healthH 16
#define score_max_health 16
#define score_1_second 1000

#define score_score_rect 0, 0, 128, 16
#define score_player_rect 0, 16, 128, 32
#define score_enemy_rect 0, 32, 128, 48
#define score_time_rect 115, 0, 179, 16
#define score_stage_rect 190, 0, 254, 16
#define score_heart_rect 198, 14, 252, 30
#define score_live_rect 185, 28, 242, 44

#define score_6_digit 6
#define score_3_digit 3
#define score_2_digit 2

#define score_subWeapon_rect_ani 0
#define score_health_bar_enemy_ani 1
#define score_health_bar_hero_ani 2
#define score_health_bar_none_ani 3
#define score_heart_ani 4

#define score_default_player_health 16
#define score_default_boss_health 16
#define score_default_player_hearts 100
#define score_default_player_lives 3
#define score_default_time 300

#define score_decrease 10
#define score_low_range 30

#define score_low_range_freq 200
#define score_end_tick 45
#define score_waitEnd 5000
#define score_waitGameover 2500

#define score_color_default D3DCOLOR_ARGB(255, 255, 255, 255)
#define score_red D3DCOLOR_ARGB(255, 255, 0, 0)
#define score_blue D3DCOLOR_ARGB(255, 0, 162, 232)

class score
{
protected:
	static score* Instance;

	DWORD startTime, startLowRange, scoreTick, waitEndGame;
	DWORD waitGameover;

	vector<LPANIMATION> data;

	LPSPRITE mainBoard;

	LPGAMEOBJECT player;
	LPD3DXFONT Font;

	LPSPRITE subWeaponRect;
	LPSPRITE healthBarHero;
	LPSPRITE healthBarEnemy;
	LPSPRITE healthBarNone;
	LPSPRITE heart;

	RECT scoreRect;
	RECT playerRect;
	RECT enemyRect;
	RECT timeRect;
	RECT stageRect;
	RECT heartRect;
	RECT liveRect;

public:
	bool endGame = false;
	bool startEnd = false;

	score();

	int pointDecrease; 
	int scoreVal;
	int playerHealth;
	int BossHealth;
	int stages;
	int time;
	int hearts;
	int lives;

	LPGAMEOBJECT subWeapon;
	LPGAMEOBJECT item;

	void setPlayer(LPGAMEOBJECT val);

	static score* getInstance();

	void update(LPGAMEOBJECT hero);
	void updateEnemy(int enemyHealth);

	void updateSubWeapon(LPGAMEOBJECT);

	void updateItem(LPGAMEOBJECT);
	
	void resetScoreBoard();
	void resetBossHealth(); 

	void render();
	void process(float dt);
};