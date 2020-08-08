#pragma once
#include "./database.h"
#include "./gameObject.h"
#include "./tile.h"

#include <unordered_map>

class tileSet
{
public:
	~tileSet();

	LPDIRECT3DTEXTURE9 tileTex;

	unordered_map<int, vector<LPGAMEOBJECT>> surfaceGrid;
	vector<vector<LPTILE>> tiles;

	vector<LPGAMEOBJECT> mapComponent;
	vector<LPGAMEOBJECT> objects;

	int columnCount;
	int mapWidth, mapHeight;
	int tileWidth, tileHeight;

	int rootX, rootY;

	string backgroundSound;

	void getMapComponent(xml_n* root);

	vector<int> getCellList(const char* stream);
	void loadData(xml_n* root);

	void draw(int alpha);
};
typedef tileSet* LPTILESET;

class gameMap
{
protected:
	static gameMap* Instance;

	vector<xml_n*> sceneList;
	
public:
	gameMap();
	~gameMap();

	static gameMap* getInstance();

	void loadData();

	xml_n* getSceneNode(int sceneId);

	int Count();
};
