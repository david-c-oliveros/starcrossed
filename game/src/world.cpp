#include "world.h"



bool World::Create(glm::ivec2 vViewArea, const glm::vec2& vPixelScale)
{
    SetViewArea(vViewArea);
    SetWorldScale(vPixelScale);
    m_vPixelScale = vPixelScale;
    m_vRecipPixel = 1.0f / m_vPixelScale;
    vecDebugInfo[0] = "World";

    pSpriteSpaceship = std::make_unique<Sprite>("spaceship_sheet");

    uint32_t nSheetWidth = 10;
    uint32_t nSheetHeight = 8;

    for (uint32_t y = 0; y < nSheetHeight; y++)
    {
        for (uint32_t x = 0; x < nSheetWidth; x++)
        {
            aTexOffsets[y * nSheetWidth + x] = glm::ivec2(x, y);
        }
    }

    cEmptyTileSprite.SetColor(glm::vec3(0.15f, 0.22f, 0.5f));

//    LoadFromFile("../../res/map_01.txt");

    Room r(glm::ivec2(0));

    r.vDim = glm::ivec2(64);
    vecRooms.push_back(r);

    for (int y = 0; y < r.vDim.y; y++)
        for (int x = 0; x < r.vDim.x; x++)
            vecRooms[0].vecTiles.push_back(Tile(glm::ivec2(x, y)));

    return true;
}



void World::Draw(SpriteRenderer &cRenderer)
{
    for (auto &r : vecRooms)
    {
        for (auto &t : r.vecTiles)
        {
            glm::vec2 vTileScreenPos = WorldToScreen(t.vWorldPos);
            glm::vec2 vScalar = m_vWorldScale;

            if (!t.bEmpty)
                pSpriteSpaceship->Draw(cRenderer, vTileScreenPos, vScalar, glm::vec2(0.1f), t.vTexOffset);
            else
                cEmptyTileSprite.DrawColored(cRenderer, vTileScreenPos, vScalar);
        }
    }
}



bool World::EmptyTile(glm::ivec2 vTilePos)
{
    return vecRooms[0].vecTiles[vTilePos.y * vecRooms[0].vDim.x + vTilePos.x].bEmpty;
}



void World::AddTile(glm::ivec2 vTilePos)
{
    std::cout << "Adding tile at position " << glm::to_string(vTilePos) << std::endl;
    if (vTilePos.x >= 0 && vTilePos.y >= 0 &&
        vTilePos.x < vecRooms[0].vDim.x &&
        vTilePos.y < vecRooms[0].vDim.y)
    {
        uint32_t nIndex = vTilePos.y * vecRooms[0].vDim.x + vTilePos.x;
        vecRooms[0].vecTiles[nIndex].bEmpty = false;
        vecRooms[0].vecTiles[nIndex].vTexOffset = aTexOffsets[nCurTexOffset];
    }
}



void World::RemoveTile(glm::ivec2 vTilePos)
{
    if (vTilePos.x >= 0 && vTilePos.y >= 0 &&
        vTilePos.x < vecRooms[0].vDim.x &&
        vTilePos.y < vecRooms[0].vDim.y)
    {
        uint32_t nIndex = vTilePos.y * vecRooms[0].vDim.x + vTilePos.x;
        vecRooms[0].vecTiles[nIndex].bEmpty = true;
        vecRooms[0].vecTiles[nIndex].vTexOffset = glm::vec2(0);
    }
}



bool World::SaveToFile(std::string sFilename)
{
    if (std::filesystem::exists(sFilename))
    {
        std::cerr << sFilename << " already exists" << std::endl;
        return false;
    }

    std::ofstream fOutFile;
    fOutFile.open(sFilename);

    for (auto &r : vecRooms)
    {
        for (auto &t : r.vecTiles)
        {
            if (t.bEmpty)
            {
                fOutFile << "#";
                continue;
            }

            std::stringstream ss;
            ss << std::hex << t.vTexOffset.x;
            ss << std::hex << t.vTexOffset.y;
            fOutFile << ss.str();
        }
    }

    fOutFile.close();

    return true;
}



void World::LoadFromFile(const char* cFilename)
{
    // TEMP - Currently only loads one room
    vecRooms.push_back(Room(glm::ivec2(0)));

    glm::ivec2 vDim(0, 0);

    std::ifstream fin;
    if (std::filesystem::exists(cFilename))
    {
        fin.open(cFilename);
    }

    std::vector<std::vector<char>> vecMap;
    std::string sLine;

    uint32_t x = 0;
    uint32_t y = 0;
    while(fin >> sLine)
    {
        std::stringstream ss;
        ss << sLine;
        char c;
        std::vector<char> vec;
        while(ss.get(c))
        {
            vec.push_back(c);
            x++;
        }
        vecMap.push_back(vec);
        y++;
    }

    // TODO - Please CLEAN UP THIS MESS!!
    for (int y = 0; y < vecMap.size(); y++)
    {
        for (int x = 0; x < vecMap[y].size(); x++)
        {
            Tile pTile(glm::vec2(x, y) * BASE_TILE_SIZE + (glm::vec2)vecRooms[0].vUpperLeftPos * BASE_TILE_SIZE);

            if (vecMap[y][x] == '.')
                pTile.vTexOffset = glm::vec2(1, 1);
            else if (vecMap[y][x] == '#' && x == 0 && y == 0)
                pTile.vTexOffset = glm::vec2(3, 0);

            else if (vecMap[y][x] == '#' && x == vecMap[y].size() - 1 && y == 0)
                pTile.vTexOffset = glm::vec2(5, 0);
            else if (vecMap[y][x] == '#' && x == 0 && y == vecMap.size() - 1)
                pTile.vTexOffset = glm::vec2(3, 2);

            else if (vecMap[y][x] == '#' && x == 0)
                pTile.vTexOffset = glm::vec2(3, 1);
            else if (vecMap[y][x] == '#' && y == 0)
                pTile.vTexOffset = glm::vec2(4, 0);

            else if (vecMap[y][x] == '#' && x == vecMap[y].size() - 1 && y == vecMap.size() - 1)
                pTile.vTexOffset = glm::vec2(5, 2);
            else if (vecMap[y][x] == '#' && x == vecMap[y].size() - 1)
                pTile.vTexOffset = glm::vec2(5, 1);
            else if (vecMap[y][x] == '#' && y == vecMap.size() - 1)
                pTile.vTexOffset = glm::vec2(4, 2);

            vecRooms[0].vecTiles.push_back(pTile);
        vecRooms[0].vDim.x = x;
        }
        vecRooms[0].vDim.y = y;
    }

    std::cout << "World size: " << glm::to_string(vecRooms[0].vDim) << std::endl;
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
