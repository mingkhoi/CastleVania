#include "./gamePlay.h"
#include "./animationSet.h"
#include "./camera.h"
#include "./listGameobject.h"
#include "./score.h"
#include "./simon.h"
#include "./texture.h"
#include "./SoundManager.h"

#include "./define.h"

#include <iostream>

#pragma warning(disable : 6011)

gamePlay* gamePlay::Instance = NULL;

gamePlay* gamePlay::getInstance() {
	if (Instance == NULL)
		Instance = new gamePlay();

	return Instance;
}

void gamePlay::startGame() {
	loadNextScene = false;

	currentSceneID = gamePLay_first_scene;
	cellWidth = cellHeight = defaultCellSize;

	rootX = 0;
	rootY = 0;

	this->loadScene(gamePlay_load_current_scene);
}

// add cell to moving grid

void gamePlay::addCell(LPGAMEOBJECT obj) {
	if (obj == NULL)
		return;

	for (int index_y = obj->getY(); index_y <= obj->getY() + obj->getHeight(); index_y += defaultCellSize) {
		for (int index_x = obj->getX(); index_x <= obj->getX() + obj->getWidth(); index_x += defaultCellSize) {

			float temp_x = (obj->getX() + obj->getWidth() - index_x) / defaultCellSize;
			float temp_y = (obj->getY() + obj->getHeight() - index_y) / defaultCellSize;
			float pad_x = 0;
			float pad_y = 0;

			if (temp_x >= 1) pad_x = defaultCellSize;
			else pad_x = (obj->getX() + obj->getWidth() - index_x);

			if (temp_y >= 1) pad_y = defaultCellSize;
			else pad_y = (obj->getY() + obj->getHeight() - index_y);

			int id1 = game::getCell(index_x, index_y, cellWidth, cellHeight, gridCountX);
			int id2 = game::getCell(index_x + pad_x, index_y, cellWidth, cellHeight, gridCountX);
			int id3 = game::getCell(index_x, index_y + pad_y, cellWidth, cellHeight, gridCountX);
			int id4 = game::getCell(index_x + pad_x, index_y + pad_y, cellWidth, cellHeight, gridCountX);

			movingGrid[id1].push_back(obj);
			movingGrid[id2].push_back(obj);
			movingGrid[id3].push_back(obj);
			movingGrid[id4].push_back(obj);
		}
	}
}

// update moving grid

void gamePlay::updateGrid() {
	this->movingGrid.clear();

	for (int i = 0; i < objects.size(); i++) {
		LPGAMEOBJECT obj = objects[i];

		if (obj == NULL || obj->isDestroyed)
			continue;
		this->addCell(obj);
	}
	this->addCell(player);
}

// unload current scene

void gamePlay::unloadScene() {
	if (currentTileSet == NULL)
		return;

	for (auto& x : mapComponent)
		free(x);
	for (auto& x : objects)
		free(x);

	movingGrid.clear();
	surfaceGrid.clear();
	mapComponent.clear();
	objects.clear();

	animationSet::getInstance()->unloadSceneResource();
	texture::getInstance()->unLoadSceneTexture();
}

void gamePlay::getGameObject(int flag) {
	xml_n* game_objects = currentSceneNode->first_node("game_object");

	xml_n* obj;
	for (obj = game_objects->first_node(); obj; obj = obj->next_sibling()) {
		string className = (string)obj->name();

		LPGAMEOBJECT newObj = listGameobject::getInstance()->reflect(className);

		float x = Numeric::toFloat(get_atr(obj, "x")) + this->rootX;
		float y = Numeric::toFloat(get_atr(obj, "y")) + this->rootY;
		float width = Numeric::toFloat(get_atr(obj, "width"));
		float height = Numeric::toFloat(get_atr(obj, "height"));
		float vx = Numeric::toFloat(get_atr(obj, "vx"));
		float vy = Numeric::toFloat(get_atr(obj, "vy"));
		int directionX = Numeric::toInt(get_atr(obj, "directionX"));
		string tag = (string)get_atr(obj, "tag");

		newObj->setPos(x, y);
		newObj->setSize(width, height);
		newObj->setSpeed(vx, vy);
		newObj->setDirecttionX(directionX);
		newObj->tag = tag;

		objects.push_back(newObj);
	}
}

void gamePlay::getSound(int flag) {
	if (flag == gamePLay_load_next_scene || flag == gamePlay_load_current_scene) {
		SoundManager::GetInstance()->loadSceneResource(currentSceneNode);
	}
}

void gamePlay::getHero(int flag) {
	xml_n* player = NULL;

	if (flag == gamePLay_load_previous_scene) {
		player = currentSceneNode->first_node("player_prev");
	}
	else if (flag == gamePLay_load_next_scene || flag == gamePlay_load_current_scene) {
		player = currentSceneNode->first_node("player");
	}

	float x, y, state;
	int directionX, stage;

	if ((string)player->value() != "NULL" && !this->loadNextScenePanCam) {
		print("[gamePlay] player is NULL\n");

		x = Numeric::toFloat(get_atr(player, "x")) + this->rootX;
		y = Numeric::toFloat(get_atr(player, "y")) + this->rootY;
		directionX = Numeric::toInt(get_atr(player, "directionX"));
		state = Numeric::toInt(get_atr(player, "state"));
		stage = Numeric::toInt(get_atr(player, "stage"));
		string tag = (string)get_atr(player, "tag");

		if (this->player == NULL)
			this->player = new simon(Counter::index++);

		this->player->setPos(x, y);
		this->player->setDirecttionX(directionX);
		this->player->setState(state);
		this->player->tag = tag;

		score::getInstance()->stages = stage;
	}
	if ((string)player->value() == "NULL" && this->player != NULL) {
		free(this->player);
		this->player = NULL;
	}
}

void gamePlay::setCam(int flag) {
	xml_n* camera = NULL;

	if (flag == gamePLay_load_previous_scene) {
		camera = currentSceneNode->first_node("camera_prev");
	}
	else if (flag == gamePLay_load_next_scene || flag == gamePlay_load_current_scene) {
		camera = currentSceneNode->first_node("camera");
	}

	camera::getInstance()->setLimitNegativeX(this->rootX);
	camera::getInstance()->setLimitPositiveX(this->rootX +
		currentTileSet->mapWidth - wndWidth);
	camera::getInstance()->setLimitNegativeY(this->rootY);
	camera::getInstance()->setLimitPositiveY(this->rootY);

	int x = Numeric::toInt(get_atr(currentSceneNode, "x"));
	int y = Numeric::toInt(get_atr(currentSceneNode, "y"));

	int camX = this->rootX + Numeric::toInt(get_atr(camera, "x"));
	int camY = this->rootY + Numeric::toInt(get_atr(camera, "y"));

	if (flag == gamePLay_load_next_scene) {
		if (this->loadNextScenePanCam) {
			camera::getInstance()->setAutoPan(camX, camY, camera_pan_speed);
			this->loadNextScenePanCam = false;
		}
		else camera::getInstance()->setPos(camX, camY);
	}
	else {
		camera::getInstance()->setPos(camX, camY);
	}
	camera::getInstance()->setMaster(this->player);
}

void gamePlay::clearScreen() {
	this->effectClear = true;
	this->startEffectClear = false;

	this->clearEnemyInScreen();
	SoundManager::GetInstance()->playSoundEffect("clear");

	game::gameFreeze = true;
}

void gamePlay::pauseScreen() {
	this->effectPause = true;
	this->startEffectPause = false;

	SoundManager::GetInstance()->playSoundEffect("clear");

	game::gameFreeze = true;
}

void gamePlay::turnOffEffect() {
	this->effectClear = false;
	this->effectPause = false;
	this->startEffectClear = false;
	this->startEffectPause = false;

	game::gameFreeze = true;
}

void gamePlay::clearEnemyInScreen() {
	float cam_x = camera::getInstance()->getX();
	float cam_y = camera::getInstance()->getY();

	for (auto& x : objects) {
		float screenX = x->getX() - cam_x;
		float screenY = x->getY() - cam_y;

		if (x->type != "enemy")
			continue;

		if (screenX >= 0 && screenX + x->getWidth() <= wndWidth &&
			screenY >= 0 && screenY + x->getHeight() <= wndHeight) {
			x->isDestroyed = true;
		}
	}
}

// load scene
void gamePlay::loadScene(int flag) {

	// reset flag + unload current scene + get sceneID

	SoundManager::GetInstance()->resetState();
	game::gameFreeze = false;

	loadNextScene = false;
	loadPreviousScene = false;
	loadCurrentScene = false;
	startBossMode = false;

	this->unloadScene();

	if (flag == gamePLay_load_previous_scene) {
		this->currentSceneID -= 1;

		if (player != NULL) {
			cast(player, simon*)->changeScene();
		}
	}
	else if (flag == gamePLay_load_next_scene) {
		this->currentSceneID += 1;

		if (this->currentSceneID >= gameMap::getInstance()->Count()) {
			this->currentSceneID = 0;

			if (player != NULL) {
				cast(player, simon*)->resetState();
			}
			score::getInstance()->resetScoreBoard();
		}
		else if (player != NULL) {
			cast(player, simon*)->changeScene();
		}
	}
	else if (flag == gamePlay_load_current_scene) {
		if (player != NULL) {
			cast(player, simon*)->changeScene();
		}
		score::getInstance()->resetBossHealth();
	}

	if (flag == gamePLay_load_next_scene) {
		this->oldTileSet = this->currentTileSet;
		this->currentTileSet = NULL;
	}
	else {
		if (this->currentTileSet != NULL) {
			free(this->currentTileSet);
			this->currentTileSet = NULL;
		}
	}

	// get xml root node of scene

	currentSceneNode = gameMap::getInstance()->getSceneNode(this->currentSceneID);

	// get texture and animation from scene

	texture::getInstance()->loadSceneTexture(currentSceneNode);
	animationSet::getInstance()->loadSceneResource(currentSceneNode);

	// get tiles from scene + update root coordinate

	this->rootX += Numeric::toInt(get_atr(currentSceneNode, "x"));
	this->rootY += Numeric::toInt(get_atr(currentSceneNode, "y"));

	currentTileSet = new tileSet();
	currentTileSet->loadData(currentSceneNode);

	// space partition parameter

	this->gridCountX = currentTileSet->mapWidth / cellWidth;

	this->surfaceGrid = currentTileSet->surfaceGrid;
	this->mapComponent = currentTileSet->mapComponent;
	this->objects = currentTileSet->objects;

	this->getGameObject(flag);
	this->getHero(flag);
	this->setCam(flag);
	this->getSound(flag);

	this->updateGrid();
}

// calculate state

void gamePlay::calculateState(const BYTE* keyState, const BYTE* keyEvent) {

	if (game::gameFreeze) {
		if (this->effectPause) {
			if (player != NULL)
				player->calculateState(keyState, keyEvent);
		}
		return;
	}

	if (game::isEventDown(keyEvent, DIK_N)) {
		this->loadNextScene = true;
	}
	else if (game::isEventDown(keyEvent, DIK_R)) {
		this->loadCurrentScene = true;
	}

	if (!camera::getInstance()->autoPan) {
		if (this->oldTileSet != NULL) {
			free(this->oldTileSet);
			this->oldTileSet = NULL;
		}
	}

	for (auto& x : objects)
		x->calculateState(keyState, keyEvent);

	if (player != NULL)
		player->calculateState(keyState, keyEvent);
}

// update function

void gamePlay::update(float dt) {

	// game special effect 

	if (game::gameFreeze) {
		if (this->effectClear) {
			if (!this->startEffectClear) {
				this->effectClearTime = GetTickCount64();
				this->startEffectClear = true;
			}

			if (GetTickCount64() - this->effectClearTime >= gamePlay_clear_time) {
				this->effectClear = false;
				this->startEffectClear = false;

				game::gameFreeze = false;
			}
			else {
				if (cast(player, simon*)->isHurted) {
					cast(player, simon*)->hurtStartTime = GetTickCount64();
				}
			}
		}
		else if (this->effectPause) {
			if (!this->startEffectPause) {
				this->effectPauseTime = GetTickCount64();
				this->startEffectPause = true;
			}

			if (GetTickCount64() - this->effectPauseTime >= gamePlay_pause_time) {
				this->effectPause = false;
				this->startEffectPause = false;

				game::gameFreeze = false;
			}
			else {
				for (int i = 0; i < objects.size(); i++) {
					if (objects[i]->isDestroyed || objects[i]->type == "enemy")
						continue;
					objects[i]->update(dt);
				}
				if (player != NULL)
					player->update(dt);

				this->updateGrid();

				camera::getInstance()->update(dt);
				score::getInstance()->process(dt);
				SoundManager::GetInstance()->playInBackground();
			}
		}
	}

	// =======================

	if (game::gameFreeze) {
		score::getInstance()->process(dt);
		return;
	}

	camera* cam = camera::getInstance();

	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->isDestroyed)
			continue;
		objects[i]->update(dt);
	}

	if (player != NULL)
		player->update(dt);

	this->updateGrid();

	camera::getInstance()->update(dt);
	score::getInstance()->process(dt);
	SoundManager::GetInstance()->playInBackground();

	if (player != NULL && player->isDestroyed) {
		player->isDestroyed = false;

		cast(player, simon*)->resetState();
		cast(player, simon*)->lives -= 1;

		if (cast(player, simon*)->lives > 0)
			this->loadScene(gamePlay_load_current_scene);
		else {
			this->currentSceneID = gameMap::getInstance()->Count() - 1;
			score::getInstance()->resetScoreBoard();

			this->loadScene(gamePlay_load_current_scene);
		}
	}
	else {
		if (this->loadNextScene || this->loadNextScenePanCam) {
			this->loadScene(gamePLay_load_next_scene);
		}
		else if (this->loadPreviousScene) {
			this->loadScene(gamePLay_load_previous_scene);
		}
		else if (this->loadCurrentScene) {
			this->loadScene(gamePlay_load_current_scene);
		}
	}
}

// render function

void gamePlay::render() {
	
	if (this->effectClear) {
		if (GetTickCount64() - flashTime >= gamePlay_flash) {
			flashTime = GetTickCount64();

			if (this->currentScreenBrightness == 0) {
				this->currentScreenBrightness = gamePlay_brightBackground;
			}

			if (this->currentScreenBrightness == gamePlay_brightBackground) {
				this->currentScreenBrightness = gamePlay_darkBackground;
			}
			else this->currentScreenBrightness = gamePlay_brightBackground;
		}
		if (oldTileSet != NULL)
			oldTileSet->draw(this->currentScreenBrightness);
		currentTileSet->draw(this->currentScreenBrightness);
	}
	else {
		if (oldTileSet != NULL)
			oldTileSet->draw(255);
		currentTileSet->draw(255);
	}

	camera* cam = camera::getInstance();

	for (int i = 0; i < mapComponent.size(); i++) {
		LPGAMEOBJECT obj = mapComponent[i];

		if (obj != NULL && !obj->isDestroyed) {
			float screenX = obj->getX() - cam->getX();
			float screenY = obj->getY() - cam->getY();

			if (screenX + obj->getWidth() < 0 || screenX > wndWidth ||
				screenY + obj->getHeight() < 0 || screenY > wndHeight) {
				continue;
			}
			else obj->render();
		}
	}

	for (int i = 0; i < objects.size(); i++) {
		if (objects[i] != NULL && !objects[i]->isDestroyed)
			objects[i]->render();
	}

	score::getInstance()->render();

	if (player != NULL)
		player->render();
}