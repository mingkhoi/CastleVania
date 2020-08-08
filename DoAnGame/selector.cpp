#include "./selector.h"
#include "./gamePlay.h"

selector::selector(int id) : gameObjectEx(id) {
	this->assignAnimationSet("big_heart"); 
	this->setSize(12, 10);

	this->chooseIndex = 0; 
	this->aniState = 0;
}

void selector::calculateState(const BYTE* keyState, 
	const BYTE* keyEvent) {

	if (game::isEventDown(keyEvent, DIK_UP)) {
		this->chooseIndex = 0;
		this->setPos(gamePlay::getInstance()->rootX + selector_yesX,
			gamePlay::getInstance()->rootY + selector_yesY);
	}
	else if (game::isEventDown(keyEvent, DIK_DOWN)) {
		this->setPos(gamePlay::getInstance()->rootX + selector_noX,
			gamePlay::getInstance()->rootY + selector_noY);
		this->chooseIndex = 1;
	}

	if (game::isEventDown(keyEvent, DIK_RETURN)) {
		if (this->chooseIndex == 0) {
			gamePlay::getInstance()->loadNextScene = true;
		}
		else if (this->chooseIndex = 1) {
			game::gameEnd = true;
		}
	}
}

void selector::setState(int) {

}

void selector::update(float dt) {
	
}

void selector::collisionHandle(LPCOLLISION collisionEvent, float dt) {

}

void selector::render() {
	this->animation[aniState]->render(this->pos_x, this->pos_y, Left, 255);
}
