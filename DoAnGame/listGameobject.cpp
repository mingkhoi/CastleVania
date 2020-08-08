#include "./listGameobject.h"

#include "./axe.h"
#include "./bigHeart.h"
#include "./blackKnight.h"
#include "./bow.h"
#include "./enemySpawner.h"
#include "./ghost.h"
#include "./holyWater.h"
#include "./hunchBack.h"
#include "./introBat.h"
#include "./introShip.h"
#include "./knife.h"
#include "./moneyLv1.h"
#include "./moneyLv2.h"
#include "./moneyLv3.h"
#include "./moneyMax.h"
#include "./movePaddle.h"
#include "./portal.h"
#include "./simon.h"
#include "./skeleton.h"
#include "./smallHeart.h"
#include "./startBat.h"
#include "./startScript.h"
#include "./torch.h"
#include "./vampireBat.h"
#include "./raven.h"
#include "./autoGoStair.h";
#include "./candle.h"
#include "./zombieSpawner.h"
#include "./zombie.h"
#include "./boomerang.h"
#include "./invisible.h"
#include "./boss.h"
#include "./emptyBrick.h"
#include "./selector.h"
#include "./doubleShot.h"
#include "./tripleShot.h"
#include "./emptyBrick.h"
#include "./cross.h"
#include "./timeMachine.h"
#include "./hidden.h"
#include "./crown.h"
#include "./treasure.h"
#include "./chicken.h"

#include "./gameObject.h"

listGameobject* listGameobject::Instance;

listGameobject* listGameobject::getInstance() {
    if (Instance == NULL)
        Instance = new listGameobject();
    return Instance;
}

void listGameobject::init() {
    table["startBat"] = _startBat;
    table["startScript"] = _startScript;
    table["introBat"] = _introBat;
    table["introShip"] = _introShip;
    table["axe"] = _axe;
    table["knife"] = _knife;
    table["portal"] = _portal;
    table["torch"] = _torch;
    table["bow"] = _bow;
    table["smallHeart"] = _smallHeart;
    table["bigHeart"] = _bigHeart;
    table["moneyLv1"] = _moneyLv1;
    table["moneyLv2"] = _moneyLv2;
    table["moneyLv3"] = _moneyLv3;
    table["moneyMax"] = _moneyMax;
    table["holyWater"] = _holyWater;
    table["vampireBat"] = _vampireBat;
    table["blackKnight"] = _blackKnight;
    table["enemySpawner"] = _enemySpawner;
    table["ghost"] = _ghost;
    table["hunchBack"] = _hunchBack;
    table["movePaddle"] = _movePaddle;
    table["skeleton"] = _skeleton;
    table["autoGoStair"] = _autoGoStair;
    table["raven"] = _raven;
    table["candle"] = _candle;
    table["zombieSpawner"] = _zombieSpawner;
    table["zombie"] = _zombie;
    table["boomerang"] = _boomerang;
    table["invisible"] = _invisible;
    table["boss"] = _boss;
    table["emptyBrick"] = _emptyBrick;
    table["selector"] = _selector;
    table["doubleShot"] = _doubleShot; 
    table["tripleShot"] = _tripleShot;
    table["cross"] = _cross;
    table["timeMachine"] = _timeMachine;
    table["hidden"] = _hidden;
    table["crown"] = _crown;
    table["treasure"] = _treasure;
    table["chicken"] = _chicken;
    table[""] = _NULL;
}

LPGAMEOBJECT listGameobject::reflect(string className) {
    int id = table[className];

    switch (id) {
    case _startBat:
        return new startBat(Counter::index++);
    case _startScript:
        return new startScript(Counter::index++);
    case _introBat:
        return new introBat(Counter::index++);
    case _introShip:
        return new introShip(Counter::index++);
    case _axe:
        return new axe(Counter::index++);
    case _knife:
        return new knife(Counter::index++);
    case _portal:
        return new portal(Counter::index++);
    case _torch:
        return new torch(Counter::index++);
    case _bow:
        return new bow(Counter::index++);
    case _smallHeart:
        return new smallHeart(Counter::index++);
    case _bigHeart:
        return new bigHeart(Counter::index++);
    case _moneyLv1:
        return new moneyLv1(Counter::index++);
    case _moneyLv2:
        return new moneyLv2(Counter::index++);
    case _moneyLv3:
        return new moneyLv3(Counter::index++);
    case _moneyMax:
        return new moneyMax(Counter::index++);
    case _holyWater:
        return new holyWater(Counter::index++);
    case _vampireBat:
        return new vampireBat(Counter::index++);
    case _blackKnight:
        return new blackKnight(Counter::index++);
    case _enemySpawner:
        return new enemySpawner(Counter::index++);
    case _ghost:
        return new ghost(Counter::index++);
    case _hunchBack:
        return new hunchBack(Counter::index++);
    case _movePaddle:
        return new movePaddle(Counter::index++);
    case _skeleton:
        return new skeleton(Counter::index++);
    case _autoGoStair:
        return new autoGoStair(Counter::index++);
    case _raven:
        return new raven(Counter::index++);
    case _candle: 
        return new candle(Counter::index++);
    case _zombie:
        return new zombie(Counter::index++);
    case _zombieSpawner:
        return new zombieSpawner(Counter::index++);
    case _boomerang:
        return new boomerang(Counter::index++);
    case _invisible:
        return new invisible(Counter::index++);
    case _boss:
        return new boss(Counter::index++);
    case _emptyBrick:
        return new emptyBrick(Counter::index++);
    case _selector:
        return new selector(Counter::index++); 
    case _doubleShot:
        return new doubleShot(Counter::index++);
    case _tripleShot:
        return new tripleShot(Counter::index++);
    case _cross:
        return new cross(Counter::index++);
    case _timeMachine:
        return new timeMachine(Counter::index++);
    case _hidden:
        return new hidden(Counter::index++);
    case _crown:
        return new crown(Counter::index++);
    case _treasure:
        return new treasure(Counter::index++);
    case _chicken:
        return new chicken(Counter::index++);
    case _NULL:
        return NULL;
    }
}
