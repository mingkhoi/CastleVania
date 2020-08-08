#pragma once
#include "./animation.h"
#include "./database.h"
#include "./define.h"

#include <unordered_map>

using namespace std;

class animationSet
{
protected:
	unordered_map<string, vector<LPANIMATION>> data;
	vector<string> sceneResource;
	vector<string> singletonResource;

	static animationSet* Instance;

public:
	animationSet();
	~animationSet();

	static animationSet* getInstance();

	void loadSingletonResource();
	void loadSceneResource(xml_n* rootNode);

	void unloadSceneResource();
	void unloadSingletonResource();

	vector<LPANIMATION> getUniqueAnimationSet(string name);

	vector<LPANIMATION> getAnimationSet(string name);
};
