#pragma once
#include "scene.h"
#include "brick.h"

using namespace std; 

class sceneGame : scene
{
public:
    sceneGame();

    virtual void drawTile();

    virtual void keyboardHandle(const BYTE* keyState,
        const BYTE* keyEvent);

    virtual void update(float dt);

    virtual void render();

    virtual void loadScene(LPGAMEOBJECT player);
};


