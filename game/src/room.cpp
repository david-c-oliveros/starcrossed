#include "room.h"



Room::Room(glm::ivec2 vDim, glm::ivec2 _vUpperLeft, glm::vec2 _vTileSizeScalar)
    : vUpperLeft(_vUpperLeft), vTileSizeScalar(_vTileSizeScalar)
{
    glm::ivec2 vTileSize = vTileSizeScalar * BASE_TILE_SIZE;
    for (uint32_t x = 0; x < vDim.x; x++)
        for (uint32_t y = 0; y < vDim.y; y++)
    {
        std::unique_ptr<Tile> tile = std::make_unique<Tile>(glm::ivec2(x, y) * vTileSize + (vUpperLeft * vTileSize));
        vecTiles.push_back(std::move(tile));
    }
}



Room::~Room()
{
}
