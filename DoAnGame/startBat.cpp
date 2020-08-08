#include "./startBat.h"
#include "./animationSet.h"
#include "./texture.h"

startBat::startBat(int id)
	: gameObject(id) {
	this->animation = animationSet::getInstance()->getAnimationSet("start_bat");

	this->animation[0]->loop = false;

	aniState = fly_1;
}

void startBat::render() {
	if (aniState == fly_1 && animation[aniState]->isDone()) {
		aniState = fly_2;
	}

	animation[aniState]->render(pos_x, pos_y, Left, 255);
}