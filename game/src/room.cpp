#include "room.h"



Room::Room(uint32_t nrTiles, glm::ivec2 _vUpperLeft, glm::vec2 _vTileSize)
    : vUpperLeft(_vUpperLeft), vTileSize(_vTileSize)
{
    for (uint32_t i = 0; i < nrTiles; i++)
    {
        std::unique_ptr<Tile> tile = std::make_unique<Tile>("../../res/Texture/awesomeface.png",
                                                            glm::vec2(i, 0) * vTileSize + ((glm::vec2)vUpperLeft * vTileSize),
                                                            vTileSize);
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
