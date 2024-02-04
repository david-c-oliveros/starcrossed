#pragma once

#include <vector>
#include <array>
#include <memory>

#include <glm/glm.hpp>

#include "global_enums.h"
#include "game_defines.h"
#include "sprite.h"
#include "tile_world.h"
#include "event.h"
#include "interactable.h"



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
    Room(glm::ivec2 _vUpperLeftPos = glm::ivec2(0), glm::ivec2 _vSize = glm::ivec2(4))
        : vUpperLeftPos(_vUpperLeftPos), vSize(_vSize), fAirPressure(1.0f) {}

    glm::ivec2 vSize;
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
        SetDoorPos();
    }

    std::pair<std::shared_ptr<Room>, std::shared_ptr<Room>> pConnectedRooms;
    std::unique_ptr<Interactable> pI;
    bool bOpen = false;
    glm::vec2 vPos;

    void SetDoorPos()
    {
        std::cout << "Room dimensions: " << glm::to_string(pConnectedRooms.first->vSize) << std::endl;
        vPos.x = pConnectedRooms.first->vUpperLeftPos.x + pConnectedRooms.first->vSize.x;
        vPos.y = pConnectedRooms.first->vUpperLeftPos.x + pConnectedRooms.first->vSize.y / 2.0f;
        glm::vec2 vSize = glm::vec2(0.2f, 2.0f);
        pI = std::make_unique<Interactable>(vPos, vSize);

        std::cout << "Door position: " << glm::to_string(vPos) << std::endl;
    }
};



class Ship
{
    public:
        Ship();
        ~Ship();

        bool Create(GameState _eGameState, const char* sFileName = "");
        void SetGameState(GameState _eGameState);

        bool EmptyTile(glm::ivec2 vTilePos);
        void AddTile(glm::ivec2 vTilePos);
        void RemoveTile(glm::ivec2 vTilePos);

        bool AddRoom(glm::ivec2 vPos, glm::ivec2 vSize);
        bool SaveToFile(std::string sFilename);
        void LoadFromFile(const char* cFilename);

        void Draw(SpriteRenderer &cRenderer, TileWorld &cTileWorld);
        void DrawDoorInteractables(SpriteRenderer &cRenderer, TileWorld &cTileWorld);

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


    private:
        GameState m_eGameState;
};
