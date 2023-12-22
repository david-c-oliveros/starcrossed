#pragma once

#include <vector>
#include <array>
#include <memory>
#include <filesystem>

#include "sprite.h"
#include "sprite_renderer.h"



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
        : vUpperLeftPos(_vUpperLeftPos) {}

    glm::ivec2 vDim;
    glm::ivec2 vUpperLeftPos;

    std::vector<Tile> vecTiles;
};




class World
{
    public:
        World() = default;

        bool Create(glm::ivec2 vViewArea, const glm::vec2& vPixelScale = glm::vec2(1.0f));
        void Draw(SpriteRenderer &cRenderer);

        bool EmptyTile(glm::ivec2 vTilePos);
        void AddTile(glm::ivec2 vTilePos);
        void RemoveTile(glm::ivec2 vTilePos);

        void LoadFromFile(const char* cFilename);
        bool SaveToFile(std::string sFilename);

        void SetWorldOffset(const glm::vec2& vOffset);
        void MoveWorldOffset(const glm::vec2& vDeltaOffset);
        void SetWorldScale(const glm::vec2& vScale);
        void SetViewArea(const glm::ivec2& vViewArea);

        glm::vec2 GetWorldTL() const;
        glm::vec2 GetWorldBR() const;
        glm::vec2 GetWorldVisibleArea() const;

        void ZoomAtScreenPos(const float fDeltaZoom, const glm::ivec2& ivPos);
        void SetZoom(const float fZoom, const glm::ivec2& ivPos);

        void StartPan(glm::vec2& vPos);
        void UpdatePan(glm::vec2& vPos);
        void EndPan(glm::vec2& vPos);

        const glm::vec2& GetWorldOffset() const;
        const glm::vec2& GetWorldScale() const;

        glm::ivec2 WorldToScreen(const glm::vec2& vWorldPos) const;
        glm::vec2 ScreenToWorld(const glm::ivec2& ivScreenPos) const;

        glm::vec2 ScaleToWorld(const glm::ivec2& vScreenSize) const;
        glm::ivec2 ScaleToScreen(const glm::vec2& vWorldSize) const;
        bool IsPointVisible(const glm::vec2& vPos) const;
        bool IsRectVisible(const glm::vec2& vPos, const glm::vec2& vSize) const;


    public:
        std::vector<Room> vecRooms;

        std::vector<std::string> vecDebugInfo = std::vector<std::string>(10);
        std::unique_ptr<Sprite> pSpriteSpaceship;

        std::array<glm::ivec2, 80> aTexOffsets;
        int32_t nCurTexOffset = 0;

    private:
        glm::vec2 m_vWorldOffset = glm::vec2(0.0f);
        glm::vec2 m_vWorldScale  = glm::vec2(1.0f);
        glm::vec2 m_vRecipPixel  = glm::vec2(1.0f);
        glm::vec2 m_vPixelScale  = glm::vec2(1.0f);
        bool m_bPanning = false;
        glm::vec2 m_vStartPan    = glm::vec2(0.0f);
        glm::ivec2 m_vViewArea;


        // TEMP!!!
        Sprite cEmptyTileSprite;
        std::unique_ptr<Sprite> pSprite;

        void printWorld(std::vector<std::string> vecWorld);
};
