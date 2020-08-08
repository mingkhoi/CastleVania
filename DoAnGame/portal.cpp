#include "./portal.h"

using namespace std;

portal::portal(int id)
	: gameObject(id) {
	this->type = "portal";
}

void portal::update(float dt) {
}

void portal::collisionHandle(LPCOLLISION collisionEvent,
	float dt) {
}

void portal::render() {
}