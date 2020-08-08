#pragma once

#include "./game.h"
#include "./gameObject.h"
#include "./map.h"

#include <unordered_map>
#include <unordered_set>

using namespace std;

#define defaultCellSize 64
#define autoGoStair_nullFloat -1.07374e+08

#define gamePLay_load_previous_scene 0
#define gamePLay_load_next_scene 1
#define gamePlay_load_current_scene 2
#define gamePLay_first_scene 0
#define camera_pan_speed 120

#define gamePlay_brightBackground 255
#define gamePlay_darkBackground 155

#define gamePlay_clear_time 2500
#define gamePlay_pause_time 3500
#define gamePlay_flash 50

class gamePlay
{
public:
	static gamePlay* Instance;

	static gamePlay* getInstance();
	DirectX::SoundEffect* tempSound = NULL;


	int rootX, rootY;

	int cellWidth;
	int cellHeight;
	int gridCountX;
	int currentSceneID;

	bool loadNextScene;
	bool loadNextScenePanCam;
	bool loadPreviousScene;
	bool loadCurrentScene;

	bool startBossMode;

	bool effectClear, startEffectClear;
	
	bool effectPause, startEffectPause;

	int currentScreenBrightness; 

	float screenMoveHorizon;

	DWORD effectClearTime, effectPauseTime, flashTime;

	xml_n* currentSceneNode;

	unordered_map<int, vector<LPGAMEOBJECT>> movingGrid;
	unordered_map<int, vector<LPGAMEOBJECT>> surfaceGrid;

	vector<LPGAMEOBJECT> mapComponent;
	vector<LPGAMEOBJECT> objects;


	LPGAMEOBJECT player;

	LPTILESET currentTileSet;
	LPTILESET oldTileSet;

	// -----------------------------------

	void startGame();

	void updateGrid();
	void addCell(LPGAMEOBJECT obj);

	void calculateState(const BYTE* keyState, const BYTE* keyEvent);

	void loadScene(int flag);
	void getGameObject(int flag); 
	void getHero(int flag);
	void getSound(int flag); 
	void setCam(int flag); 

	void clearScreen();
	void pauseScreen(); 

	void turnOffEffect();

	void clearEnemyInScreen();

	void unloadScene();

	void update(float dt);

	void render();
};