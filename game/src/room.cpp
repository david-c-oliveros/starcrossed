#include "room.h"



Room::Room(uint32_t nrTiles, glm::ivec2 _vUpperLeft)
    : vUpperLeft(vUpperLeft)
{
    for (uint32_t i = 0; i < nrTiles; i++)
    {
        std::unique_ptr<Tile> tile = std::make_unique<Tile>(glm::ivec2(i, 0) + vUpperLeft, "../../res/Texture/awesomeface.png");
        vecTiles.push_back(std::move(tile));
    }
}



Room::~Room()
{
}



void Room::Draw(SpriteRenderer &cRenderer)
{
    for (auto &t : vecTiles)
    {
        t->Draw(cRenderer);
    }
}
