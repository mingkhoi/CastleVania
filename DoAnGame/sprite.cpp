#include "./sprite.h"
#include "./game.h"

using namespace std;

_sprite::_sprite(int left, int top, int width, int height,
	LPDIRECT3DTEXTURE9 texture) {
	this->sourceRect.left = left;
	this->sourceRect.top = top;
	this->sourceRect.right = left + width;
	this->sourceRect.bottom = top + height;

	this->texture = texture;
}

LPDIRECT3DTEXTURE9 _sprite::getTexture() {
	return texture;
}

RECT _sprite::getSourceRect() {
	RECT res;
	res.left = sourceRect.left;
	res.top = sourceRect.top;
	res.right = sourceRect.right;
	res.bottom = sourceRect.bottom;

	return res;
}

float _sprite::getWidth() {
	return sourceRect.right - sourceRect.left;
}

float _sprite::getHeight() {
	return sourceRect.bottom - sourceRect.top;
}

void _sprite::draw(float x, float y, int directionX, D3DCOLOR color) {
	game::getInstance()->draw(texture, &sourceRect, x, y,
		directionX, color);
}

void _sprite::drawFromBottom(float x, float y, int directionX, D3DCOLOR color) {
	game::getInstance()->drawBottom(texture, &sourceRect, x, y,
		directionX, color);
}

void _sprite::drawCenter(float x, float y, int directionX, D3DCOLOR color) {
	game::getInstance()->drawCenter(texture, &sourceRect, x, y,
		directionX, color);
}

// ===============================================