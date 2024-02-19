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
#include "room.h"



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

        std::shared_ptr<Room> GetCurrentRoom(glm::vec2 vTilePos);
        bool EmptyTile(glm::ivec2 vTilePos);
        void AddTile(glm::ivec2 vTilePos);
        void RemoveTile(glm::ivec2 vTilePos);

        std::shared_ptr<Room> AddRoom(glm::ivec2 vPos, glm::ivec2 vSize);
        std::shared_ptr<Room> AddRoomFromSelected(std::shared_ptr<Room> pSelectedRoom, CarDir eDir, glm::ivec2 vSize);
        std::shared_ptr<Room> GetFurthestRoom(CarDir eDir);

        bool SaveToFile(std::string sFilename);
        void LoadFromFile(const char* cFilename);

        void Draw(SpriteRenderer &cRenderer, TileWorld &cTileWorld);
        void DrawSelectedOutline(SpriteRenderer &cRenderer, TileWorld &cTileWorld, std::shared_ptr<Room> pCurrentRoom);
        void DrawDoorInteractables(SpriteRenderer &cRenderer, TileWorld &cTileWorld);

        void UpdateRooms();
        void CalcAirFlow();
        void ActivateEvent(Event &cEvent);


    public:
        std::vector<std::string> vecDebugInfo;

        std::unique_ptr<Sprite> pSpriteSpaceship;
        std::unique_ptr<Sprite> pOutlineSprite;
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

        bool m_bDebug = true;
};
