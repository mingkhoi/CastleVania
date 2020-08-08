#include "./introShip.h"
#include "./animationSet.h"
#include "./database.h"
#include "./texture.h"

introShip::introShip(int id)
	: gameObject(id) {
	this->animation = animationSet::getInstance()->getAnimationSet("intro_ship");
}

void introShip::update(float dt) {
	this->pos_x += speed_x * dt;
	this->pos_y += speed_y * dt;
}

void introShip::render() {
	this->animation[0]->render(pos_x, pos_y, Left, 255);
}