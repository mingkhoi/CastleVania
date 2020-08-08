#pragma once
#include "./game.h"
#include <string>
#include <unordered_map>

using namespace std;

#define transparentColor D3DCOLOR_XRGB(0, 128, 0)

class texture
{
protected:
	static texture* Instance;

	unordered_map<string, LPDIRECT3DTEXTURE9> data;

	xml_n* sceneTexture;

public:
	bool isEmpty();

	static texture* getInstance();

	static LPDIRECT3DTEXTURE9 createTexture(const char* path,
		D3DCOLOR transparent);

	void add(string id, LPDIRECT3DTEXTURE9 value);

	void loadSingletonTexture();
	void loadSceneTexture(xml_n* root);

	LPDIRECT3DTEXTURE9 getTexture(string id);

	void unLoadSceneTexture();
};
typedef texture* LPTEXTURE;