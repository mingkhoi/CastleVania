#include "./animationSet.h"
#include "./texture.h"

animationSet* animationSet::Instance = NULL;

animationSet::animationSet() {
}

animationSet::~animationSet() {
	for (auto& arr : data) {
		for (auto& x : arr.second) {
			delete(x);
			x = NULL;
		}
	}
}

animationSet* animationSet::getInstance() {
	if (Instance == NULL)
		Instance = new animationSet();
	return Instance;
}

void animationSet::loadSingletonResource() {
	xml_n* rootNode = database::getInstance()->getRoot("singleton");
	LPDIRECT3DTEXTURE9 tex = texture::getInstance()->getTexture("singleton");

	xml_n* object, * ani, * rect;

	for (object = rootNode->first_node("score_board"); object; object = object->next_sibling()) {
		string nodeName = (string)object->name();

		this->singletonResource.push_back(nodeName);

		vector<LPANIMATION> aniSet;

		for (ani = object->first_node(); ani; ani = ani->next_sibling()) {
			LPANIMATION tempAni = new _animation();

			for (rect = ani->first_node(); rect; rect = rect->next_sibling()) {
				int l = Numeric::toInt(get_atr(rect, "left"));
				int t = Numeric::toInt(get_atr(rect, "top"));
				int w = Numeric::toInt(get_atr(rect, "width"));
				int h = Numeric::toInt(get_atr(rect, "height"));
				int time = Numeric::toInt(get_atr(rect, "time"));

				LPSPRITE sprite = new _sprite(l, t, w, h, tex);
				tempAni->add(sprite, time);
			}
			tempAni->addEndAnimation();
			aniSet.push_back(tempAni);
		}
		this->data[nodeName] = aniSet;
	}
}

void animationSet::loadSceneResource(xml_n* rootNode) {
	string rootNodeName = (string)rootNode->name();

	xml_n* root = rootNode->first_node("game_object_animation");

	LPDIRECT3DTEXTURE9 tex = texture::getInstance()->getTexture(rootNodeName);

	xml_n* object, * ani, * rect;

	for (object = root->first_node(); object; object = object->next_sibling()) {
		string nodeName = (string)object->name();

		this->sceneResource.push_back(nodeName);

		vector<LPANIMATION> aniSet;

		for (ani = object->first_node(); ani; ani = ani->next_sibling()) {
			LPANIMATION tempAni = new _animation();

			for (rect = ani->first_node(); rect; rect = rect->next_sibling()) {
				int l = Numeric::toInt(get_atr(rect, "left"));
				int t = Numeric::toInt(get_atr(rect, "top"));
				int w = Numeric::toInt(get_atr(rect, "width"));
				int h = Numeric::toInt(get_atr(rect, "height"));
				int time = Numeric::toInt(get_atr(rect, "time"));

				LPSPRITE sprite = new _sprite(l, t, w, h, tex);
				tempAni->add(sprite, time);
			}
			tempAni->addEndAnimation();
			aniSet.push_back(tempAni);
		}
		this->data[nodeName] = aniSet;
	}
}

void animationSet::unloadSingletonResource() {
	for (auto& x : singletonResource) {
		vector<LPANIMATION> aniSet = this->data[x];

		for (auto& item : aniSet)
			delete (item);
		this->data.erase(x);
	}
	singletonResource.clear();
}

void animationSet::unloadSceneResource() {
	for (auto& x : sceneResource) {
		vector<LPANIMATION> aniSet = this->data[x];

		for (auto& item : aniSet)
			free(item);
		this->data.erase(x);
	}
	sceneResource.clear();
}

vector<LPANIMATION> animationSet::getUniqueAnimationSet(string name) {
	vector<LPANIMATION> res;
	vector<LPANIMATION> defVal = this->data[name];

	for (int i = 0; i < defVal.size(); i++) {
		res.push_back(new _animation(*defVal[i]));
	}

	return res;
}

vector<LPANIMATION> animationSet::getAnimationSet(string name) {
	return this->data[name];
}
