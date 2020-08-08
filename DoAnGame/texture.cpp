#include "./texture.h"
#include "./database.h"

texture* texture::Instance;

texture* texture::getInstance() {
	if (Instance == NULL)
		Instance = new texture();
	return Instance;
}

bool texture::isEmpty() {
	if (data.size() == 0)
		return true;
	return false;
}

LPDIRECT3DTEXTURE9 texture::createTexture(const char* path,
	D3DCOLOR transparent) {
	LPDIRECT3DTEXTURE9 value;

	D3DXIMAGE_INFO imgInfo;

	D3DXGetImageInfoFromFileA(path, &imgInfo);

	D3DXCreateTextureFromFileExA(game::getInstance()->getDevice(),
		path, imgInfo.Width, imgInfo.Height,
		1, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
		transparent, &imgInfo, NULL, &value);

	return value;
}

void texture::unLoadSceneTexture() {
	LPDIRECT3DTEXTURE9 tex = this->getTexture((string)sceneTexture->name());

	if (tex != NULL)
		tex->Release();

	this->data.erase((string)sceneTexture->name());
	this->sceneTexture = NULL;
}

void texture::loadSingletonTexture() {
	xml_n* root = database::getInstance()->getRoot("singleton");

	const char* path = get_atr(root, "dir");

	this->add("singleton", this->createTexture(path, transparentColor));
}

void texture::loadSceneTexture(xml_n* root) {
	this->sceneTexture = root;
	xml_n* rootNode = sceneTexture->first_node("game_object_animation");

	if (rootNode->first_attribute("dir") == NULL)
		return;

	const char* path = get_atr(rootNode, "dir");

	this->add((string)sceneTexture->name(), this->createTexture(path, transparentColor));
}

void texture::add(string id, LPDIRECT3DTEXTURE9 value) {
	data.insert(make_pair(id, value));
}

LPDIRECT3DTEXTURE9 texture::getTexture(string id) {
	return data[id];
}