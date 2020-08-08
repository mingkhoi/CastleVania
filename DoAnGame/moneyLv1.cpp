#include "./moneyLv1.h"
#include "./gamePlay.h"
#include "./point.h"

void moneyLv1::die() {
	if (isDestroyed == false)
		isDestroyed = true;
	else
		return;

	point* Point = new point(Counter::index++);
	Point->setPos(this->pos_x, this->pos_y);
	Point->setNum(_point_100);

	gamePlay::getInstance()->objects.push_back(Point);
}