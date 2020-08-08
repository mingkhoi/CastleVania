#pragma once
#include "./define.h"
#include <unordered_map>
#include <vector>
#include <memory>

using namespace std;

class SoundManager
{
public:
	SoundManager();

	bool isStopBackground = false;

	static SoundManager* Instance;
	static SoundManager* GetInstance();

	unordered_map<string, wstring> effectDir;
	wstring backgroundDir;
	wstring bossDir;

	unordered_map<string, DirectX::SoundEffect*> effect;
	DirectX::SoundEffect* background;
	DirectX::SoundEffect* boss;

	string previousBackground = "";

	xml_n* sceneNode;

	void loadSingletonResource();
	void loadSceneResource(xml_n* sceneNode);

	void playInBackground(); 
	void playSoundEffect(string effect); 
	void stop();

	void resetState();
};