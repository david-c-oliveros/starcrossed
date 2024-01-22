#include "ship.h"



Ship::Ship()
{
    vecDebugInfo.push_back("Ship");
    for (int32_t i = 0; i < 8; i++)
        vecDebugInfo.push_back("");

    nScrap = 0;
    nFood = 200;

    pSpriteSpaceship = std::make_unique<Sprite>("spaceship_sheet");
    cEmptyTileSprite.SetColor(glm::vec3(0.15f, 0.22f, 0.5f));

    uint32_t nSheetWidth = 10;
    uint32_t nSheetHeight = 8;

    for (uint32_t y = 0; y < nSheetHeight; y++)
    {
        for (uint32_t x = 0; x < nSheetWidth; x++)
        {
            aTexOffsets[y * nSheetWidth + x] = glm::ivec2(x, y);
        }
    }

    Room r(glm::ivec2(0));

    r.vDim = glm::ivec2(64);
    vecRooms.push_back(r);
}



Ship::~Ship()
{
}



void Ship::Draw(SpriteRenderer &cRenderer, TileWorld &cTileWorld)
{
    for (auto &r : vecRooms)
    {
        for (auto &t : r.vecTiles)
        {
            glm::vec2 vTileScreenPos = cTileWorld.WorldToScreen(t.vWorldPos);
            glm::vec2 vScalar = cTileWorld.GetWorldScale();

            if (!t.bEmpty)
                pSpriteSpaceship->Draw(cRenderer, vTileScreenPos, vScalar, glm::vec2(0.1f), t.vTexOffset);
            else if (cTileWorld.GetGameState() == GameState::LEVEL_EDIT)
                cEmptyTileSprite.DrawColored(cRenderer, vTileScreenPos, vScalar);
        }
    }
}



void Ship::UpdateRooms()
{
    if (bDoorsOpen)
    {
        float fPressureFlow = 0.01f * vecRooms[0].fOxygenLevel - 0.01f * vecRooms[1].fOxygenLevel;
        vecRooms[0].fOxygenLevel -= fPressureFlow;
        vecRooms[1].fOxygenLevel += fPressureFlow;
    }
}



void Ship::ActivateEvent(Event &cEvent)
{
    nFood -= cEvent.mCost["food"];
}



bool Ship::EmptyTile(glm::ivec2 vTilePos)
{
    return vecRooms[0].vecTiles[vTilePos.y * vecRooms[0].vDim.x + vTilePos.x].bEmpty;
}



void Ship::AddTile(glm::ivec2 vTilePos)
{
    if (vTilePos.x >= 0 && vTilePos.y >= 0 &&
        vTilePos.x < vecRooms[0].vDim.x &&
        vTilePos.y < vecRooms[0].vDim.y)
    {
        uint32_t nIndex = vTilePos.y * vecRooms[0].vDim.x + vTilePos.x;
        vecRooms[0].vecTiles[nIndex].bEmpty = false;
        vecRooms[0].vecTiles[nIndex].vTexOffset = aTexOffsets[nCurTexOffset];
    }
}



void Ship::RemoveTile(glm::ivec2 vTilePos)
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



bool Ship::SaveToFile(std::string sFilename)
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
        /***************************************/
        /*        Write room dimensions        */
        /***************************************/
        std::stringstream ssa;
        ssa << "x" << std::hex << r.vDim.x << "y" << std::hex << r.vDim.y;
        ssa << "t";

        fOutFile << ssa.str();
        for (auto &t : r.vecTiles)
        {
            if (t.bEmpty)
            {
                fOutFile << "#";
                continue;
            }

            /**********************************/
            /*        Write room tiles        */
            /**********************************/
            std::stringstream ss;
            ss << std::hex << t.vTexOffset.x;
            ss << std::hex << t.vTexOffset.y;
            fOutFile << ss.str();
        }
    }

    fOutFile.close();

    return true;
}



void Ship::LoadFromFile(const char* cFilename)
{
    // TEMP - Currently only loads one room
    vecRooms.push_back(Room(glm::ivec2(0)));
    vecRooms.push_back(Room(glm::ivec2(65)));

    vecRooms[1].fOxygenLevel = 0.6f;

    glm::ivec2 vDim(0, 0);

    std::ifstream fin;
    if (std::filesystem::exists(cFilename))
    {
        fin.open(cFilename);
    }

    std::vector<std::string> vecWorld;
    std::string sLine;

    /**************************************************/
    /*    Read file into an std::vector of strings    */
    /**************************************************/
    while(fin >> sLine)
    {
        std::stringstream ss;
        ss << sLine;
        char c;

        while(ss.get(c))
        {
            uint32_t nElementSize = 0;

            if (c == 't')
                continue;

            if (c == '#')
                nElementSize = 1;
            else if (c == 'x' || c == 'y')
                nElementSize = 3;
            else
                nElementSize = 2;

            std::stringstream ssa;
            ssa << c;
            for (int32_t i = 1; i < nElementSize; i++)
            {
                ss.get(c);
                ssa << c;
            }
            vecWorld.push_back(ssa.str());
        }
    }

    /****************************************************/
    /*    Generate world from std::vector of strings    */
    /****************************************************/
    /*    Format is "x--y--t"
     *    'x' -> Indicates width of room
     *    'y' -> Indicates height of room
     *    't' -> Indicates that the remaining
     *           char's will be tile data,
     *           until another 'r' is read
     *    '-' -> A single hex digit each
     */
    /*****************************/
    /*    Read dimension data    */
    /*****************************/
    std::stringstream ss;
    std::stringstream ssa;
    ss << vecWorld[0][1] << vecWorld[0][2];
    ssa << std::hex << ss.str();
    ssa >> vecRooms[0].vDim.x;

    ss.str("");
    ssa.str("");
    ss.clear();
    ssa.clear();

    ss << vecWorld[1][1] << vecWorld[1][2];
    ssa << std::hex << ss.str();
    ssa >> vecRooms[0].vDim.y;

    /************************/
    /*    Read tile data    */
    /************************/
    int32_t i = 2;
    int32_t nEmptyTiles = 0;
    int32_t nNonEmptyTiles = 0;
    for (int32_t y = 0; y < vecRooms[0].vDim.y; y++)
    {
        for (int32_t x = 0; x < vecRooms[0].vDim.x; x++)
        {
            if (vecWorld[i] == "#")
            {
                Tile t(glm::ivec2(x, y));
                vecRooms[0].vecTiles.push_back(t);
                i++;
                nEmptyTiles++;
                continue;
            }

            glm::ivec2 _vTexOffset;
            std::stringstream ss;
            std::stringstream ssa;
            ss << vecWorld[i][0];
            ssa << std::hex << ss.str();
            ssa >> _vTexOffset.x;

            ss.str("");
            ssa.str("");
            ss.clear();
            ssa.clear();

            ss << vecWorld[i][1];
            ssa << std::hex << ss.str();
            ssa >> _vTexOffset.y;

            Tile t(glm::ivec2(x, y));
            t.vTexOffset = _vTexOffset;
            t.bEmpty = false;
            nNonEmptyTiles++;

            vecRooms[0].vecTiles.push_back(t);

            i++;
        }
    }
}
