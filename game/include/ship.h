#pragma once

#include <vector>
#include <array>
#include <memory>

#include <glm/glm.hpp>

#include "sprite.h"
#include "tile_world.h"
#include "event.h"



struct Tile
{
    Tile(glm::ivec2 _vWorldPos)
        : vWorldPos(_vWorldPos) {}

    glm::vec2 vWorldPos;
    glm::vec2 vTexOffset;
    bool bEmpty = true;
};



struct Room
{
    Room(glm::ivec2 _vUpperLeftPos = glm::ivec2(0))
        : vUpperLeftPos(_vUpperLeftPos), fAirPressure(1.0f) {}

    glm::ivec2 vDim;
    glm::ivec2 vUpperLeftPos;

    float fAirPressure;
    bool bOpenToVacuum = false;

    std::vector<Tile> vecTiles;
};



struct Door
{
    Door(std::shared_ptr<Room> r1, std::shared_ptr<Room> r2)
    {
        pConnectedRooms = std::make_pair(r1, r2);
    }

    std::pair<std::shared_ptr<Room>, std::shared_ptr<Room>> pConnectedRooms;
};



class Ship
{
    public:
        Ship();
        ~Ship();

        bool EmptyTile(glm::ivec2 vTilePos);
        void AddTile(glm::ivec2 vTilePos);
        void RemoveTile(glm::ivec2 vTilePos);

        bool SaveToFile(std::string sFilename);
        void LoadFromFile(const char* cFilename);

        void Draw(SpriteRenderer &cRenderer, TileWorld &cTileWorld);

        void UpdateRooms();
        void CalcAirFlow();
        void ActivateEvent(Event &cEvent);


    public:
        std::vector<std::string> vecDebugInfo;

        std::unique_ptr<Sprite> pSpriteSpaceship;
        // TEMP!!
        Sprite cEmptyTileSprite;

        std::array<glm::ivec2, 80> aTexOffsets;
        int32_t nCurTexOffset = 0;

        int32_t nFood;
        int32_t nScrap;

        std::vector<std::shared_ptr<Room>> vecRooms;
        std::vector<Door> vecDoors;

        bool bDoorsOpen = false;
};
