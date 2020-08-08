#include "./map.h"
#include "./gamePlay.h"
#include "./camera.h"
#include "./listGameobject.h"
#include "./texture.h"
#include "./userData.h"


tileSet::~tileSet() {
	for (auto& arr : tiles) {
		for (auto& item : arr) {
			free(item);
			item = NULL;
		}
	}
	tiles.clear();

	tileTex->Release();
	surfaceGrid.clear();

	mapComponent.clear();
}

vector<int> tileSet::getCellList(const char* stream) {
	stringstream ss;
	ss << stream;
	string temp;

	vector<int> res;
	while (getline(ss, temp, ','))
		res.push_back(Numeric::toInt(temp.c_str()));
	return res;
}

void tileSet::getMapComponent(xml_n* root) {
	xml_n* soundNode = root->first_node("sound");
	this->backgroundSound = (string)soundNode->value();

	xml_n* rootNode = root->first_node("map_component");

	xml_n* objType, * obj;
	for (objType = rootNode->first_node(); objType; objType = objType->next_sibling()) {
		string objTypeName = (string)objType->name();

		for (obj = objType->first_node(); obj; obj = obj->next_sibling()) {
			LPGAMEOBJECT component = listGameobject::getInstance()->reflect(objTypeName);

			print("[ERROR] objTypeName = %s\n", objTypeName.c_str());

			float x = Numeric::toFloat(get_atr(obj, "x")) + this->rootX;
			float y = Numeric::toFloat(get_atr(obj, "y")) + this->rootY;
			float width = Numeric::toFloat(get_atr(obj, "width"));
			float height = Numeric::toFloat(get_atr(obj, "height"));
			int directionX = Numeric::toInt(get_atr(obj, "directionX"));

			string tag = (string)get_atr(obj, "tag");

			component->tag = tag;
			component->setDirecttionX(directionX);
			component->setPos(x, y);
			component->setSize(width, height);

			if (component->tag == "ground") {
				component->type = "ground";
			}

			if (dynamic_cast<userData*>(component)) {
				component->loadData(obj);

				if (!component->isStaticObject) {
					this->objects.push_back(component);
					continue;
				}
			}

			vector<int> cellList = getCellList(get_atr(obj, "cell"));

			for (auto id : cellList)
				surfaceGrid[id].push_back(component);

			this->mapComponent.push_back(component);
		}
	}
}

void tileSet::loadData(xml_n* root) {
	xml_n* rootNode = root->first_node("tile");

	this->rootX = gamePlay::getInstance()->rootX;
	this->rootY = gamePlay::getInstance()->rootY;

	columnCount = Numeric::toInt(get_atr(rootNode, "columnCount"));
	mapWidth = Numeric::toInt(get_atr(rootNode, "mapw"));
	mapHeight = Numeric::toInt(get_atr(rootNode, "maph"));
	tileWidth = Numeric::toInt(get_atr(rootNode, "tilew"));
	tileHeight = Numeric::toInt(get_atr(rootNode, "tileh"));

	char* dir = get_atr(rootNode, "dir");

	print("[MAP] column: %s, mapW: %s, mapH: %s, dir: %s\n",
		to_string(columnCount).c_str(),
		to_string(mapWidth).c_str(),
		to_string(mapHeight).c_str(), dir);

	this->tileTex = texture::createTexture(dir, NULL);

	xml_n* row;
	for (row = rootNode->first_node(); row; row = row->next_sibling()) {
		tiles.push_back(vector<LPTILE>());

		string id;
		stringstream ss;
		ss << row->value();

		while (getline(ss, id, ',')) {
			int index = Numeric::toInt(id.c_str()) - 1;

			LPSPRITE sprite = new _sprite((index % columnCount) * tileWidth,
				(index / columnCount) * tileHeight, tileWidth, tileHeight, tileTex);

			LPTILE temp = new tile(sprite);

			tiles.back().push_back(temp);
		}
	}

	int pad = 0;

	if (mapHeight < wndHeight)
		pad = scoreBoard;

	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			tiles[i][j]->x = (j * tileWidth) + this->rootX;
			tiles[i][j]->y = (i * tileHeight) + pad + this->rootY;
		}
	}
	this->getMapComponent(root);
}

// implementing something......


void tileSet::draw(int alpha) {
	camera* cam = camera::getInstance();

	int screenCamX = cam->getX() - this->rootX;
	int screenCamY = cam->getY() - this->rootY;

	int startI = (screenCamY - (tileHeight * 2)) / tileHeight;
	int endI = (screenCamY + wndHeight + (tileHeight * 2)) / tileHeight;
	int startJ = (screenCamX - (tileWidth * 2)) / tileWidth;
	int endJ = (screenCamX + wndWidth + (tileWidth * 2)) / tileWidth;

	if (endI > tiles.size())
		endI = tiles.size();
	if (startI < 0)
		startI = 0;

	if (endJ > tiles[0].size())
		endJ = tiles[0].size();
	if (startJ < 0)
		startJ = 0;

	for (int i = startI; i < endI; i++) {
		for (int j = startJ; j < endJ; j++) {
			D3DCOLOR color = D3DCOLOR_ARGB(alpha, 200, 200, 200);
			tiles[i][j]->render(color);
		}
	}
}

// =================================================================

gameMap* gameMap::Instance = NULL;

gameMap::gameMap() {
}

gameMap::~gameMap() {
	sceneList.clear();
}

gameMap* gameMap::getInstance() {
	if (Instance == NULL)
		Instance = new gameMap();
	return Instance;
}

void gameMap::loadData() {
	xml_n* rootNode = database::getInstance()->getRoot("start");

	xml_n* scene;

	for (scene = rootNode; scene; scene = scene->next_sibling()) {
		string nodeName = (string)scene->name();

		if (nodeName == "singleton")
			return;
		sceneList.push_back(scene);
	}
}

xml_n* gameMap::getSceneNode(int sceneId) {
	return sceneList[sceneId];
}

int gameMap::Count() {
	return sceneList.size();
}