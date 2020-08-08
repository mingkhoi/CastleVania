#pragma once
#include "./define.h"

class _sprite // rect in texture
{
protected:
	RECT sourceRect;

	LPDIRECT3DTEXTURE9 texture;

public:
	_sprite(int left, int top, int width, int height,
		LPDIRECT3DTEXTURE9 texture);

	LPDIRECT3DTEXTURE9 getTexture();

	RECT getSourceRect();

	float getWidth();
	float getHeight();

	void draw(float x, float y, int directionX, D3DCOLOR color);
	void drawFromBottom(float x, float y, int directionX, D3DCOLOR color);
	void drawCenter(float x, float y, int directionX, D3DCOLOR color);
};
typedef _sprite* LPSPRITE;
