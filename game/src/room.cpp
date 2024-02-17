#include "room.h"



Room::Room(bool bNew, glm::ivec2 _vUpperLeftPos, glm::ivec2 _vSize)
    : vUpperLeftPos(_vUpperLeftPos), vSize(_vSize), fAirPressure(1.0f)
{
    if (bNew)
    {
        glm::ivec2 vEnd = vUpperLeftPos + vSize;
        for (int32_t y = vUpperLeftPos.y; y < vEnd.y; y++)
        {
            for (int32_t x = vUpperLeftPos.x; x < vEnd.x; x++)
            {
                vecTiles.push_back(Tile(glm::ivec2(x, y)));
            }
        }
    }
}



Room::~Room()
{
}
