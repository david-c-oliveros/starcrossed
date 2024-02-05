#include "room.h"



Room::Room(glm::ivec2 _vUpperLeftPos, glm::ivec2 _vSize)
    : vUpperLeftPos(_vUpperLeftPos), vSize(_vSize), fAirPressure(1.0f)
{
    for (int32_t y = 0; y < _vSize.y; y++)
    {
        for (int32_t x = 0; x < _vSize.x; x++)
        {
            vecTiles.push_back(Tile(glm::ivec2(x, y)));
        }
    }
}



Room::~Room()
{
}
