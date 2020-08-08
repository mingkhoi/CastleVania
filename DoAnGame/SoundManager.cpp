#include "./SoundManager.h"
#include "./database.h"
#include "./gamePlay.h"
#include "./camera.h"
#include "./game.h"

SoundManager* SoundManager::Instance = NULL;

SoundManager* SoundManager::GetInstance() {
	if (Instance == NULL) {
		Instance = new SoundManager();
	}
	return Instance;
}

SoundManager::SoundManager() {

}

void SoundManager::loadSingletonResource() {
	xml_n* rootNode = database::getInstance()->getRoot("singleton");
	xml_n* soundNode = rootNode->first_node("sound");

	for (xml_n* temp = soundNode->first_node("invisible"); temp; temp = temp->next_sibling()) {
		string nodeName = (string)temp->name();
		string dir = (string)temp->value();
		
		wstring dir_w = wstring(dir.begin(), dir.end());
		effectDir[nodeName] = dir_w;
		effect[nodeName] = new DirectX::SoundEffect(game::getInstance()->getAudioDevice(),
			effectDir[nodeName].c_str());
	}
}

void SoundManager::loadSceneResource(xml_n* sceneNode) {
	this->sceneNode = sceneNode;

	xml_n* backgroundNode = sceneNode->first_node("sound");

	if ((string)backgroundNode->value() == this->previousBackground) {
		return;
	}
	else {
		this->previousBackground = (string)backgroundNode->value();

		game::gameFreezeWindow = true;

		game::getInstance()->getAudioDevice()->Reset();
		game::getInstance()->getAudioDevice()->Resume();

		if (background != NULL) {
			delete[] background; 
			background = NULL;
		}
		string a = (string)backgroundNode->value();
		this->backgroundDir = wstring(a.begin(), a.end());
		this->background = new DirectX::SoundEffect(game::getInstance()->getAudioDevice(),
			this->backgroundDir.c_str());
	}
}

void SoundManager::playSoundEffect(string name) {
	effect[name]->Play();
}

void SoundManager::playInBackground() {
	if (this->isStopBackground)
		return;

	if (!gamePlay::getInstance()->startBossMode) {
		if (!background->IsInUse() && !camera::getInstance()->autoPan)
			background->Play();
	}
	else {
		if (boss == NULL) {
			game::gameFreezeWindow = true;

			game::getInstance()->getAudioDevice()->Reset();
			game::getInstance()->getAudioDevice()->Resume();

			xml_n* bossNode = this->sceneNode->first_node("boss");

			if (bossNode != NULL) {
				string b = (string)bossNode->value();
				this->bossDir = wstring(b.begin(), b.end());
				this->boss = new DirectX::SoundEffect(game::getInstance()->getAudioDevice(),
					this->bossDir.c_str());
			}
		}
		if (!boss->IsInUse())
			boss->Play();
	}
}

void SoundManager::stop() {
	isStopBackground = true;

	game::gameFreezeWindow = true;

	game::getInstance()->getAudioDevice()->Reset();
	game::getInstance()->getAudioDevice()->Resume();
}

void SoundManager::resetState() {
	isStopBackground = false;

	if (boss != NULL) {
		game::gameFreezeWindow = true;

		game::getInstance()->getAudioDevice()->Reset();
		game::getInstance()->getAudioDevice()->Resume();

		delete[] boss;
		boss = NULL;
	}
}