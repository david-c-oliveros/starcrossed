#include "tile_world.h"



bool TileWorld::Create(glm::ivec2 vViewArea, const glm::vec2& vPixelScale)
{
    SetViewArea(vViewArea);
    SetWorldScale(vPixelScale);
    m_vPixelScale = vPixelScale;
    m_vRecipPixel = 1.0f / m_vPixelScale;
    vecDebugInfo[0] = "World";

    return true;
}



void TileWorld::SetGameState(GameState _eState)
{
    m_eGameState = _eState;
}



GameState TileWorld::GetGameState()
{
    return m_eGameState;
}



void TileWorld::SetWorldOffset(const glm::vec2& vOffset)
{
    m_vWorldOffset = vOffset;
}



void TileWorld::MoveWorldOffset(const glm::vec2& vDeltaOffset)
{
    m_vWorldOffset += vDeltaOffset;
}



void TileWorld::SetWorldScale(const glm::vec2& vScale)
{
    m_vWorldScale = vScale;
}



void TileWorld::SetViewArea(const glm::ivec2& vViewArea)
{
    m_vViewArea = vViewArea;
}



glm::vec2 TileWorld::GetWorldTL() const
{
    return ScreenToWorld(glm::ivec2(0.0f));
}



glm::vec2 TileWorld::GetWorldBR() const
{
    return ScreenToWorld(m_vViewArea);
}



glm::vec2 TileWorld::GetWorldVisibleArea() const
{
    return GetWorldBR() - GetWorldTL();
}



void TileWorld::ZoomAtScreenPos(const float fDeltaZoom, const glm::ivec2& ivPos)
{
    glm::vec2 vOffsetBeforeZoom = ScreenToWorld(ivPos);
    m_vWorldScale *= fDeltaZoom;
    glm::vec2 vOffsetAfterZoom = ScreenToWorld(ivPos);
    m_vWorldOffset += vOffsetBeforeZoom - vOffsetAfterZoom;
}



void TileWorld::SetZoom(const float fZoom, const glm::ivec2& ivPos)
{
    glm::vec2 vOffsetBeforeZoom = ScreenToWorld(ivPos);
    m_vWorldScale = glm::vec2(fZoom);
    glm::vec2 vOffsetAfterZoom = ScreenToWorld(ivPos);
    m_vWorldOffset += vOffsetBeforeZoom - vOffsetAfterZoom;
}



void TileWorld::StartPan(glm::vec2& vPos)
{
    m_bPanning = true;
    m_vStartPan = vPos;
}



void TileWorld::UpdatePan(glm::vec2& vPos)
{
    m_vWorldOffset -= (vPos - m_vStartPan) / m_vWorldScale;
    m_vStartPan = vPos;
}



void TileWorld::EndPan(glm::vec2& vPos)
{
    UpdatePan(vPos);
    m_bPanning = false;
}



const glm::vec2& TileWorld::GetWorldOffset() const
{
    return m_vWorldOffset;
}



const glm::vec2& TileWorld::GetWorldScale() const
{
    return m_vWorldScale;
}



glm::ivec2 TileWorld::WorldToScreen(const glm::vec2& vWorldPos) const
{
    glm::vec2 vFloatScreenPos = ((vWorldPos - m_vWorldOffset) * m_vWorldScale);
    vFloatScreenPos = glm::vec2(std::floor(vFloatScreenPos.x), std::floor(vFloatScreenPos.y));

    return (glm::vec2)vFloatScreenPos;
}



glm::vec2 TileWorld::ScreenToWorld(const glm::ivec2& ivScreenPos) const
{
    return ((glm::vec2)ivScreenPos / m_vWorldScale) + m_vWorldOffset;
}



glm::vec2 TileWorld::ScaleToWorld(const glm::ivec2& vScreenSize) const
{
    return ((glm::vec2)vScreenSize / m_vWorldScale);
}



glm::ivec2 TileWorld::ScaleToScreen(const glm::vec2& vWorldSize) const
{
    glm::vec2 vFloat = vWorldSize * m_vWorldScale;
    return glm::floor(vFloat);
}



bool TileWorld::IsPointVisible(const glm::vec2& vPos) const
{
    glm::ivec2 vScreen = WorldToScreen(vPos);
    return vScreen.x >= 0 && vScreen.x < m_vViewArea.x &&
           vScreen.y >= 0 && vScreen.y < m_vViewArea.y;
}



bool TileWorld::IsRectVisible(const glm::vec2& vPos, const glm::vec2& vSize) const
{
    glm::ivec2 vScreenPos = WorldToScreen(vPos);
    glm::ivec2 vScreenSize = vSize * m_vWorldScale;
    return (vScreenPos.x < 0 + m_vViewArea.x && vScreenPos.x + vScreenSize.x > 0 &&
            vScreenPos.y < m_vViewArea.y && vScreenPos.y + vScreenSize.y > 0);
}
