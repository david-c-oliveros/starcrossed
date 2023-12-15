#include "world.h"



bool World::Create(glm::ivec2 vViewArea, const glm::vec2& vPixelScale)
{
    SetViewArea(vViewArea);
    SetWorldScale(vPixelScale);
    m_vPixelScale = vPixelScale;
    m_vRecipPixel = 1.0f / m_vPixelScale;
    vecDebugInfo[0] = "World";

    for (int i = 0; i < 1; i++)
    {
        vecRooms.push_back(std::make_unique<Room>(glm::ivec2(4, 16), glm::ivec2(0, i)));
    }

    // TEMP!!!!!
    pSpriteGrass = std::make_unique<Sprite>("grass");

    return true;
}



void World::Draw(std::shared_ptr<SpriteRenderer> pRenderer, glm::vec2 vDebugPos)
{
//    glm::vec2 vFirstTileWorld = vecRooms[0]->vecTiles[0]->vWorldPos;
//    glm::vec2 vFirstTileScreen = WorldToScreen(vFirstTileWorld);

//    vecDebugInfo[1] = "World Offset: " + glm::to_string(m_vWorldOffset);
//    vecDebugInfo[2] = "vFirstTileWorld: " + glm::to_string(vFirstTileWorld);
//    vecDebugInfo[3] = "vFirstTileScreen: " + glm::to_string(vFirstTileScreen);
//    vecDebugInfo[4] = "vWorldScale: " + glm::to_string(m_vWorldScale);

    for (auto &r : vecRooms)
    {
        for (auto &t : r->vecTiles)
        {
            glm::vec2 vTileScreenPos = WorldToScreen(t->vWorldPos);
            glm::vec2 vScalar = BASE_TILE_SIZE * m_vWorldScale;

            pSpriteGrass->Draw(pRenderer, vTileScreenPos, vScalar);
        }
    }
}



void World::SetWorldOffset(const glm::vec2& vOffset)
{
    m_vWorldOffset = vOffset;
}



void World::MoveWorldOffset(const glm::vec2& vDeltaOffset)
{
    m_vWorldOffset += vDeltaOffset;
}



void World::SetWorldScale(const glm::vec2& vScale)
{
    m_vWorldScale = vScale;
}



void World::SetViewArea(const glm::ivec2& vViewArea)
{
    m_vViewArea = vViewArea;
}



glm::vec2 World::GetWorldTL() const
{
    return ScreenToWorld(glm::ivec2(0.0f));
}



glm::vec2 World::GetWorldBR() const
{
    return ScreenToWorld(m_vViewArea);
}



glm::vec2 World::GetWorldVisibleArea() const
{
    return GetWorldBR() - GetWorldTL();
}



void World::ZoomAtScreenPos(const float fDeltaZoom, const glm::ivec2& ivPos)
{
    glm::vec2 vOffsetBeforeZoom = ScreenToWorld(ivPos);
    m_vWorldScale *= fDeltaZoom;
    glm::vec2 vOffsetAfterZoom = ScreenToWorld(ivPos);
    m_vWorldOffset += vOffsetBeforeZoom - vOffsetAfterZoom;
}



void World::SetZoom(const float fZoom, const glm::ivec2& ivPos)
{
    glm::vec2 vOffsetBeforeZoom = ScreenToWorld(ivPos);
    m_vWorldScale = glm::vec2(fZoom);
    glm::vec2 vOffsetAfterZoom = ScreenToWorld(ivPos);
    m_vWorldOffset += vOffsetBeforeZoom - vOffsetAfterZoom;
}



void World::StartPan(glm::vec2& vPos)
{
    m_bPanning = true;
    m_vStartPan = vPos;
}



void World::UpdatePan(glm::vec2& vPos)
{
    m_vWorldOffset -= (vPos - m_vStartPan) / m_vWorldScale;
    m_vStartPan = vPos;
}



void World::EndPan(glm::vec2& vPos)
{
    UpdatePan(vPos);
    m_bPanning = false;
}



const glm::vec2& World::GetWorldOffset() const
{
    return m_vWorldOffset;
}



const glm::vec2& World::GetWorldScale() const
{
    return m_vWorldScale;
}



glm::ivec2 World::WorldToScreen(const glm::vec2& vWorldPos) const
{
    glm::vec2 vFloatScreenPos = ((vWorldPos - m_vWorldOffset) * m_vWorldScale);
    vFloatScreenPos = glm::vec2(std::floor(vFloatScreenPos.x), std::floor(vFloatScreenPos.y));

    return (glm::vec2)vFloatScreenPos;
}



glm::vec2 World::ScreenToWorld(const glm::ivec2& ivScreenPos) const
{
    return ((glm::vec2)ivScreenPos / m_vWorldScale) + m_vWorldOffset;
}



glm::vec2 World::ScaleToWorld(const glm::ivec2& vScreenSize) const
{
    return ((glm::vec2)vScreenSize / m_vWorldScale);
}



glm::ivec2 World::ScaleToScreen(const glm::vec2& vWorldSize) const
{
    glm::vec2 vFloat = vWorldSize * m_vWorldScale;
    return glm::floor(vFloat);
}



bool World::IsPointVisible(const glm::vec2& vPos) const
{
    glm::ivec2 vScreen = WorldToScreen(vPos);
    return vScreen.x >= 0 && vScreen.x < m_vViewArea.x &&
           vScreen.y >= 0 && vScreen.y < m_vViewArea.y;
}



bool World::IsRectVisible(const glm::vec2& vPos, const glm::vec2& vSize) const
{
    glm::ivec2 vScreenPos = WorldToScreen(vPos);
    glm::ivec2 vScreenSize = vSize * m_vWorldScale;
    return (vScreenPos.x < 0 + m_vViewArea.x && vScreenPos.x + vScreenSize.x > 0 &&
            vScreenPos.y < m_vViewArea.y && vScreenPos.y + vScreenSize.y > 0);
}
