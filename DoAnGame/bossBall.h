#pragma once
#include "./gameObjectEx.h"
#include "./score.h"
#include "./gamePlay.h"
#include "./SoundManager.h"

#define bossBall_gravity 450.0f

class bossBall : public gameObjectEx
{
public:
	bossBall(int id) : gameObjectEx(id) {
		this->tag = "bossBall"; 
		this->type = "item";
		this->assignAnimationSet("end_game_ball");

		this->setSizeItem("bossBall");

		aniState = 0;
	}

	virtual void calculateState(const BYTE* keyState,
		const BYTE* keyEvent) {
	}

	virtual void setState(int) {}

	virtual void update(float dt) {
		this->speed_y += bossBall_gravity * dt;

		if (standingGround != NULL && standingGround->type == "moving ground"
			&& onTheGround && state == item_state) {
			speed_x = standingGround->getSpeedX();
		}

		vector<string> AABBTag = { "simon" };
		vector<string> AABBType = { };

		this->updateSweptFilterAABB(game::makeFilterCollision(AABBTag),
			game::makeFilterCollision(AABBType), dt);
	}

	virtual void collisionHandle(LPCOLLISION colEvent,
		float dt) {
		LPGAMEOBJECT obj = colEvent->obj;

		if (colEvent->tag == AABBCollision) {
			if (obj->tag == "simon") {
				this->isDestroyed = true;

				score::getInstance()->endGame = true;
				SoundManager::GetInstance()->stop();
				SoundManager::GetInstance()->playSoundEffect("victory");
			}
			return;
		}
		if (obj->tag != "ground") {
			this->removeCollisionBarrier(colEvent, dt);
		}
	}

	virtual void render() {
		this->animation[aniState]->render(pos_x, pos_y, Left, 255);
	}
};

