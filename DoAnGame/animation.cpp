#include "./animation.h"
#include "./game.h"

using namespace std;

frame::frame(LPSPRITE img, DWORD t) {
	this->img = img;
	this->time = t;
}

frame::~frame() {
	if (img != NULL) {
		delete(img);
		img = NULL;
	}
}

DWORD frame::getTime() {
	return time;
}

LPSPRITE frame::getImg() {
	return img;
}

void frame::draw(float x, float y, int directionX, D3DCOLOR color) {
	this->img->draw(x, y, directionX, color);
}

void frame::drawFromBottom(float x, float y, int directionX, D3DCOLOR color) {
	this->img->drawFromBottom(x, y, directionX, color);
}

void frame::drawCenter(float x, float y, int directionX, D3DCOLOR color) {
	this->img->drawCenter(x, y, directionX, color);
}

// ===================================================

_animation::_animation() {
	currentFrame = -1;
}

_animation::~_animation() {
	for (auto& x : this->data) {
		free(x);
		x = NULL;
	}
	this->data.clear();
}

LPSPRITE _animation::getFrame(int id) {
	return data[id]->img;
}

_animation::_animation(const _animation& value) {
	data.resize(value.data.size());

	for (int i = 0; i < value.data.size(); i++) {
		data[i] = value.data[i];
	}
	currentFrame = -1;
}

void _animation::add(LPSPRITE sprite, DWORD time) {
	data.push_back(new frame(sprite, time));
}

int _animation::remainFrame() {
	if (currentFrame == -1)
		return data.size() - 1;
	return data.size() - currentFrame - 1;
}

LPSPRITE _animation::getCurrentFrame() {
	if (currentFrame == -1)
		return data[0]->getImg();
	return data[currentFrame]->getImg();
}

void _animation::addEndAnimation() {
	LPSPRITE s = this->data.back()->img;

	this->add(s, 1);
}

bool _animation::isDone() {
	if (currentFrame == data.size() - 1)
		return true;
	return false;
}

void _animation::clone(int value) {
	vector<frame*> clone = data;

	for (int i = 0; i < value; i++) {
		for (auto x : clone)
			data.push_back(x);
	}
}

void _animation::increaseFrame() {
	DWORD now = GetTickCount64();

	if (currentFrame == -1) {
		currentFrame = 0;
		lastTime = now;
	}
	else {
		if (now - lastTime - pauseTime >= data[currentFrame]->getTime()) {
			currentFrame++;
			pauseTime = 0;

			if (currentFrame >= data.size()) {
				if (loop == true) {
					currentFrame = 0;
					lastTime = now;
				}
				else
					currentFrame = data.size() - 1;
			}
			else
				lastTime = now;
		}
	}
}

void _animation::render(float x, float y, int directionX, int alpha) {
	if (this->pause == true || game::gameFreeze) {
		this->stopAtCurrentFrame(x, y, directionX, alpha);
		return;
	}
	else if (this->hasPause) {
		this->pauseTime = GetTickCount64() - this->lastPause;
		this->hasPause = false;
	}

	this->increaseFrame();

	data[currentFrame]->draw(x, y, directionX, 
		D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

void _animation::renderNoPause(float x, float y, int directionX, int alpha) {
	this->increaseFrame();

	data[currentFrame]->draw(x, y, directionX,
		D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

void _animation::renderFromBottom(float x, float y, int directionX, int alpha) {
	if (this->pause == true || game::gameFreeze) {
		this->stopAtCurrentFrameFromBottom(x, y, directionX, alpha);
		return;
	}
	else if (this->hasPause) {
		this->pauseTime = GetTickCount64() - this->lastPause;
		this->hasPause = false;
	}
	
	this->increaseFrame();

	data[currentFrame]->drawFromBottom(x, y, directionX, 
		D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

void _animation::renderCenter(float x, float y, int directionX, int alpha) {
	if (this->pause == true || game::gameFreeze) {
		this->stopAtCurrentFrameCenter(x, y, directionX, alpha);
		return;
	}
	else if (this->hasPause) {
		this->pauseTime = GetTickCount64() - this->lastPause;
		this->hasPause = false;
	}

	this->increaseFrame();

	data[currentFrame]->drawCenter(x, y, directionX,
		D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

void _animation::render(float x, float y, int directionX, D3DCOLOR color) {
	if (this->pause == true || game::gameFreeze) {
		this->stopAtCurrentFrame(x, y, directionX, color);
		return;
	}
	else if (this->hasPause) {
		this->pauseTime = GetTickCount64() - this->lastPause;
		this->hasPause = false;
	}

	this->increaseFrame();

	data[currentFrame]->draw(x, y, directionX, color);
}

void _animation::renderNoPause(float x, float y, int directionX, D3DCOLOR color) {
	this->increaseFrame();

	data[currentFrame]->draw(x, y, directionX, color);
}

void _animation::renderFromBottom(float x, float y, int directionX, D3DCOLOR color) {
	if (this->pause == true || game::gameFreeze) {
		this->stopAtCurrentFrameFromBottom(x, y, directionX, color);
		return;
	}
	else if (this->hasPause) {
		this->pauseTime = GetTickCount64() - this->lastPause;
		this->hasPause = false;
	}

	this->increaseFrame();

	data[currentFrame]->drawFromBottom(x, y, directionX, color);
}

void _animation::renderCenter(float x, float y, int directionX, D3DCOLOR color) {
	if (this->pause == true || game::gameFreeze) {
		this->stopAtCurrentFrameCenter(x, y, directionX, color);
		return;
	}
	else if (this->hasPause) {
		this->pauseTime = GetTickCount64() - this->lastPause;
		this->hasPause = false;
	}

	this->increaseFrame();

	data[currentFrame]->drawCenter(x, y, directionX, color);
}

void _animation::stopAtCurrentFrame(float x, float y, int directionX, int alpha) {
	if (!hasPause) {
		this->lastPause = GetTickCount64();
		hasPause = true;
	}
	if (currentFrame == -1)
		data[0]->draw(x, y, directionX,
			D3DCOLOR_ARGB(alpha, 255, 255, 255));
	else
		data[currentFrame]->draw(x, y, directionX, 
			D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

void _animation::stopAtCurrentFrame(float x, float y, int directionX, D3DCOLOR color) {
	if (!hasPause) {
		this->lastPause = GetTickCount64();
		hasPause = true;
	}
	if (currentFrame == -1)
		data[0]->draw(x, y, directionX, color);
	else
		data[currentFrame]->draw(x, y, directionX, color);
}

void _animation::stopAtCurrentFrameFromBottom(float x, float y, int directionX, int alpha) {
	if (!hasPause) {
		this->lastPause = GetTickCount64();
		hasPause = true;
	}
	if (currentFrame == -1)
		data[0]->drawFromBottom(x, y, directionX, 
			D3DCOLOR_ARGB(alpha, 255, 255, 255));
	else
		data[currentFrame]->drawFromBottom(x, y, directionX, 
			D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

void _animation::stopAtCurrentFrameFromBottom(float x, float y, int directionX, D3DCOLOR color) {
	if (!hasPause) {
		this->lastPause = GetTickCount64();
		hasPause = true;
	}
	if (currentFrame == -1)
		data[0]->drawFromBottom(x, y, directionX, color);
	else
		data[currentFrame]->drawFromBottom(x, y, directionX, color);
}

void _animation::stopAtCurrentFrameCenter(float x, float y, int directionX, int alpha) {
	if (!hasPause) {
		this->lastPause = GetTickCount64();
		hasPause = true;
	}
	if (currentFrame == -1)
		data[0]->drawCenter(x, y, directionX,
			D3DCOLOR_ARGB(alpha, 255, 255, 255));
	else
		data[currentFrame]->drawCenter(x, y, directionX,
			D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

void _animation::stopAtCurrentFrameCenter(float x, float y, int directionX, D3DCOLOR color) {
	if (!hasPause) {
		this->lastPause = GetTickCount64();
		hasPause = true;
	}
	if (currentFrame == -1)
		data[0]->drawCenter(x, y, directionX, color);
	else
		data[currentFrame]->drawCenter(x, y, directionX, color);
}
// =====================================
