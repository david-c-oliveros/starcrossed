#pragma once

#include <vector>
#include <memory>

#include "game_defines.h"
#include "sprite_renderer.h"
#include "sprite.h"



struct Tile
{
    Tile(glm::ivec2 _vWorldPos)
        : vWorldPos(_vWorldPos) {}

    glm::vec2 vWorldPos;
//    glm::vec2 vSizeScalar;
};



class Room
{
    public:
        Room(uint32_t nrTiles = 1,
             glm::ivec2 _vUpperLeft = glm::ivec2(0),
             glm::vec2 _vTileSizeScalar = glm::vec2(1.0f));

        ~Room();


    public:
        std::unique_ptr<Sprite> pSprite;
        std::vector<std::unique_ptr<Tile>> vecTiles;
        glm::ivec2 vUpperLeft;
        glm::vec2 vTileSizeScalar;
};
