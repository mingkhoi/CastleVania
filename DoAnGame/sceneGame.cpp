#include "sceneGame.h"
#include "database.h"
#include "texture.h"
#include "sprite.h"
#include "simon.h"
#include "score.h"
#include "portal.h"

#include <vector>

#define camStop 512

sceneGame::sceneGame()
{

}

void sceneGame::loadScene(LPGAMEOBJECT player)
{
    game::getInstance()->setCamPos(0, 0);

    xml_n* rect, * ani, * row;

    this->tex = texture::getInstance()->
        getTexture(123);

    this->root = database::getInstance()->getRoot("sceneGame");

    this->gridX = Numeric::toInt(get_atr(this->root, "gridx"));
    this->gridY = Numeric::toInt(get_atr(this->root, "gridy"));
    this->cellW = Numeric::toInt(get_atr(this->root, "cellw"));
    this->cellH = Numeric::toInt(get_atr(this->root, "cellh"));

    xml_n* tile = root->first_node("tile");

    int columnCount = Numeric::toInt(get_atr(tile,
        "columnCount"));

    this->tileSet.resize(12);

    int count = 0, brick_count = 0;
    loop_node(row, tile, "row")
    {
        loop_id(row->value())
        {
            int gid = Numeric::toInt(id.c_str()) - 1;

            LPSPRITE t = new _sprite(
                (gid % columnCount) * 16,
                (gid / columnCount) * 16,
                16, 16, this->tex);

            t->gid = gid;

            if (gid == 0)
            {
                objects.push_back(new brick(
                    Counter::index++));
                objects.back()->setWidth(18);
                objects.back()->setHeight(16);
            }
            tileSet[count].push_back(t);
        }
        count++;
    }

    objects.push_back(new portal(Counter::index++));
    objects.back()->setPos(743.08f, 146.1f);
    objects.back()->setWidth(20);
    objects.back()->setHeight(20);

    this->player = new simon(Counter::index++);

    this->player->setPos(32.26f, 176.1f);
    this->player->setState(simon_state_idle);

    game::getInstance()->setCamPos(0, 0.0f);
}

void sceneGame::keyboardHandle(const BYTE* keyState,
    const BYTE* keyEvent)
{
    player->keyboardHandle(keyState, keyEvent);
}

void sceneGame::update(float dt)
{
    if (objects.size() == 0)
        return;

    for (auto& x : objects)
    {
        x->update(dt);
    }
    this->player->update(dt);

    float temp_x = this->player->getX();
    float temp_y = this->player->getY();

    if (temp_x < 0) temp_x = 0;
    else if (temp_x >= camStop + wndWidth - 16)
        temp_x = camStop + wndWidth - 16;

    this->player->setPos(temp_x, temp_y);

    if (this->player->getX() > wndWidth / 2)
    {
        if (game::getInstance()->getCamX() < camStop)
            game::getInstance()->setCamPos(
                this->player->getX() - (wndWidth / 2), 0.0f);
    }
    if (this->player->getX() < camStop + (wndWidth / 2))
    {
        if (game::getInstance()->getCamX() > 0)
            game::getInstance()->setCamPos(
                this->player->getX() - (wndWidth / 2), 0.0f);
    }
}

void sceneGame::drawTile()
{
    if (tileSet.size() == 0)
        return;

    float cam_x = game::getInstance()->getCamX();
    float cam_y = game::getInstance()->getCamY();

    int count = 0;
    for (int i = 0; i < tileSet.size(); i++)
    {
        for (int j = 0; j < tileSet[i].size(); j++)
        {
            int gid = tileSet[i][j]->gid;

            if (gid == 0)
            {
                objects[count++]->setPos(j * 16, scoreBoard + (i * 16));
            }
            tileSet[i][j]->draw(j * 16,
                scoreBoard + (i * 16),
                Left);
        }
    }
}

void sceneGame::render()
{
    if (objects.size() == 0)
        return;

    for (auto& x : objects)
    {
        x->render();
    }

    this->drawTile();

    score::getInstance()->render();

    this->player->render();
}