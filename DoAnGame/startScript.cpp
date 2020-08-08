#include "./startScript.h"
#include "./animationSet.h"
#include "./database.h"
#include "./gamePlay.h"
#include "./texture.h"

startScript::startScript(int id)
	: gameObject(id) {
	this->animation = animationSet::getInstance()->getAnimationSet("start_script");

	print("[ERROR] animationSize=%s\n", to_string(animation.size()).c_str());
	animation[1]->clone(5);
	animation[1]->loop = false;
}

void startScript::calculateState(const BYTE* keyState,
	const BYTE* keyEvent) {
	if (game::isKeyDown(keyState, DIK_RETURN)) {
		aniState = flash;
	}
}

void startScript::update(float dt) {
	if (aniState == flash && animation[aniState]->isDone()) {
		gamePlay::getInstance()->loadNextScene = true;
	}
}

void startScript::render() {
	animation[aniState]->render(pos_x, pos_y, Left, 255);
}