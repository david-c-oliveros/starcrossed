#pragma once

#include <vector>
#include <array>

#include <glm/glm.hpp>




struct Tile
{
    Tile(glm::ivec2 _vWorldPos)
        : vWorldPos(_vWorldPos) {}

    glm::vec2 vWorldPos;
    glm::vec2 vTexOffset;
    bool bEmpty = true;
    bool bSolid = false;
};



class Room
{
    public:
        Room(bool bNew, glm::ivec2 _vUpperLeftPos = glm::ivec2(0), glm::ivec2 _vSize = glm::ivec2(4));
        ~Room();


    public:
        glm::ivec2 vSize;
        glm::ivec2 vUpperLeftPos;

        int32_t nIndex = 0;

        float fAirPressure;
        bool bOpenToVacuum = false;

        std::vector<Tile> vecTiles;
        std::array<bool, 4> aOpenSides;
};
