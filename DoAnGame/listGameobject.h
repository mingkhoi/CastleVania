#pragma once

#include "./gameObject.h"

#include <string>
#include <unordered_map>

using namespace std;


class listGameobject
{
public:
    enum classType
    {
        _startBat,
        _startScript,
        _introBat,
        _introShip,
        _simon,
        _whip,
        _axe,
        _knife,
        _portal,
        _torch,
        _bow,
        _smallHeart,
        _bigHeart,
        _moneyLv1,
        _moneyLv2,
        _moneyLv3,
        _moneyMax,
        _holyWater,
        _vampireBat,
        _blackKnight,
        _enemySpawner,
        _ghost,
        _hunchBack,
        _movePaddle,
        _skeleton,
        _autoGoStair,
        _raven,
        _candle,
        _zombieSpawner,
        _zombie,
        _boomerang,
        _invisible,
        _boss,
        _emptyBrick,
        _doubleShot,
        _tripleShot,
        _selector,
        _timeMachine,
        _cross,
        _hidden,
        _crown,
        _treasure,
        _chicken,
        _NULL,
    };

    static listGameobject* Instance;
    unordered_map<string, classType> table;

    static listGameobject* getInstance();

    void init();

    LPGAMEOBJECT reflect(string className);
};
