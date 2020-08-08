#pragma once
#include "./sprite.h"
#include <string>

class tile
{
public:
	LPSPRITE data;
	int x;
	int y;

	tile(LPSPRITE sprite) {
		this->data = sprite;
	}

	~tile() {
		free(data);
		data = NULL;
	}

	void render(D3DCOLOR color) {
		data->draw(x, y, Left, color);
	}
};
typedef tile* LPTILE;
