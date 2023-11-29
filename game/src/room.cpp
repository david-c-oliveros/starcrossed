#include "room.h"



Room::Room(uint32_t nrTiles, glm::ivec2 _vUpperLeft, glm::vec2 _vTileSizeScalar)
    : vUpperLeft(_vUpperLeft), vTileSizeScalar(_vTileSizeScalar)
{

    glm::ivec2 vTileSize = vTileSizeScalar * BASE_TILE_SIZE;
    for (uint32_t i = 0; i < nrTiles; i++)
    {
        std::unique_ptr<Tile> tile = std::make_unique<Tile>("../../res/Texture/awesomeface.png",
                                                            glm::ivec2(i, 0) * vTileSize + (vUpperLeft * vTileSize),
                                                            vTileSizeScalar);
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
