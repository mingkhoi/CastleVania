#include "./movePaddle.h"

movePaddle::movePaddle(int id)
	: gameObjectEx(id) {
	this->type = "moving ground";
	this->assignAnimationSet("move_paddle");

	aniState = 0;
}

void movePaddle::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {
}

void movePaddle::setState(int) {
}

void movePaddle::update(float dt) {
	speed_x = movePaddle_speedX * directionX;
	speed_y = movePaddle_speedY;

	vector<string> AABBTag = {  };	
	vector<string> AABBType = { "", "item" };

	this->updateSweptFilterAABB(game::makeFilterCollision(AABBTag),
		game::makeFilterCollision(AABBType), dt);
}

void movePaddle::collisionHandle(LPCOLLISION colEvent,
	float dt) {
	if (colEvent->obj->tag == "ground") {
		if (colEvent->nx != 0) 
			directionX = colEvent->nx;
	}
}

void movePaddle::render() {
	animation[aniState]->render(pos_x, pos_y, directionX, 255);
}