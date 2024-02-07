#include "ship.h"



Ship::Ship()
{
    vecDebugInfo.push_back("Ship");
    for (int32_t i = 0; i < 8; i++)
        vecDebugInfo.push_back("");

    nScrap = 0;
    nFood = 200;

    /*******************************/
    /*        Sprite things        */
    /*******************************/
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

//     auto r = std::make_shared<Room>(glm::ivec2(0));
//
//    r->vSize = glm::ivec2(64);
//    vecRooms.push_back(r);
}



Ship::~Ship()
{
}



bool Ship::Create(GameState _eGameState, const char* sFileName)
{
    switch(_eGameState)
    {
        case(GameState::LEVEL_EDIT):
        {
            vecRooms.push_back(std::make_shared<Room>(glm::ivec2(), glm::ivec2(8)));

            break;
        }

        case(GameState::PLAY):
        {
            if (strlen(sFileName) == 0)
            {
                std::cout << "ERROR: No filename provided" << std::endl;
                return false;
            }

            LoadFromFile(sFileName);

            break;
        }
    }

    return true;
}



void Ship::SetGameState(GameState _eState)
{
    m_eGameState = _eState;
}



void Ship::Draw(SpriteRenderer &cRenderer, TileWorld &cTileWorld)
{
    for (auto &r : vecRooms)
    {
        for (auto &t : r->vecTiles)
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



// TEMP!!
void Ship::DrawDoorInteractables(SpriteRenderer &cRenderer, TileWorld &cTileWorld)
{
    for (auto &d : vecDoors)
    {
        d.pI->Draw(cRenderer, cTileWorld);
    }
}



void Ship::UpdateRooms()
{
    if (bDoorsOpen)
        CalcAirFlow();
}



void Ship::CalcAirFlow()
{
    const static float fC = 0.01f;
    for (auto &d : vecDoors)
    {
        if (!d.bOpen)
            continue;

        float fAirFlow = fC * d.pConnectedRooms.first->fAirPressure - fC * d.pConnectedRooms.second->fAirPressure;

        d.pConnectedRooms.first->fAirPressure  -= fAirFlow;
        d.pConnectedRooms.second->fAirPressure += fAirFlow;
    }
}



void Ship::ActivateEvent(Event &cEvent)
{
    nFood -= cEvent.mCost["food"];
}



std::shared_ptr<Room> Ship::GetCurrentRoom(glm::ivec2 vTilePos)
{
    for (auto room : vecRooms)
    {
        glm::ivec2 vUL = room->vUpperLeftPos;

        if (vTilePos.x >= vUL.x && vTilePos.y >= vUL.y &&
            vTilePos.x < vUL.x + room->vSize.x &&
            vTilePos.y < vUL.y + room->vSize.y)
        {
            return room;
        }
    }

    return nullptr;
}



bool Ship::EmptyTile(glm::ivec2 vTilePos)
{
    std::shared_ptr<Room> pCurRoom = GetCurrentRoom(vTilePos);

    if (pCurRoom == nullptr)
        return false;

    glm::ivec2 vUL = pCurRoom->vUpperLeftPos;

    if (vTilePos.x < vUL.x || vTilePos.y < vUL.y ||
        vTilePos.x >= vUL.x + pCurRoom->vSize.x ||
        vTilePos.y >= vUL.y + pCurRoom->vSize.y)
        return false;

    uint32_t nIndex = (vTilePos.y - vUL.y) * pCurRoom->vSize.x + (vTilePos.x - vUL.x);

    return pCurRoom->vecTiles[nIndex].bEmpty;
}



/********************************/
/********************************/
/*        Edit mode only        */
/********************************/
/********************************/
void Ship::AddTile(glm::ivec2 vTilePos)
{
    std::shared_ptr<Room> pCurRoom = GetCurrentRoom(vTilePos);
    glm::ivec2 vUL = pCurRoom->vUpperLeftPos;

    if (vTilePos.x >= vUL.x && vTilePos.y >= vUL.y &&
        vTilePos.x < vUL.x + pCurRoom->vSize.x &&
        vTilePos.y < vUL.y + pCurRoom->vSize.y)
    {
        uint32_t nIndex = (vTilePos.y - vUL.y) * pCurRoom->vSize.x + (vTilePos.x - vUL.x);

        pCurRoom->vecTiles[nIndex].bEmpty = false;
        pCurRoom->vecTiles[nIndex].vTexOffset = aTexOffsets[nCurTexOffset];
    }
}



void Ship::RemoveTile(glm::ivec2 vTilePos)
{
    std::shared_ptr<Room> pCurRoom = GetCurrentRoom(vTilePos);
    glm::ivec2 vUL = pCurRoom->vUpperLeftPos;

    if (vTilePos.x >= vUL.x && vTilePos.y >= vUL.y &&
        vTilePos.x < vUL.x + pCurRoom->vSize.x &&
        vTilePos.y < vUL.y + pCurRoom->vSize.y)
    {
        uint32_t nIndex = (vTilePos.y - vUL.y) * pCurRoom->vSize.x + (vTilePos.x - vUL.x);
        pCurRoom->vecTiles[nIndex].bEmpty = true;
        pCurRoom->vecTiles[nIndex].vTexOffset = glm::vec2(0);
    }
}



bool Ship::AddRoom(glm::ivec2 vPos, glm::ivec2 vSize)
{
    vecRooms.push_back(std::make_shared<Room>(vPos, vSize));

    return true;
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
        ssa << "x" << std::hex << r->vSize.x << "y" << std::hex << r->vSize.y;
        ssa << "t";

        fOutFile << ssa.str();
        for (auto &t : r->vecTiles)
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
//    vecRooms.push_back(std::make_shared<Room>(glm::ivec2(0)));
//    vecRooms.push_back(std::make_shared<Room>(glm::ivec2(64)));
//    vecRooms.push_back(std::make_shared<Room>(glm::ivec2(128)));

//    vecRooms[0]->fAirPressure = 0.6f;
//    vecRooms[1]->fAirPressure = 8.0f;
//
//    vecDoors.push_back(Door(vecRooms[0], vecRooms[1]));
//    vecDoors.push_back(Door(vecRooms[1], vecRooms[2]));
//    vecDoors[0].bOpen = true;

    glm::ivec2 vSize(0, 0);

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
    int32_t nLineNumber = 0;
    while(fin >> sLine)
    {
        std::cout << "reading line " << ++nLineNumber << std::endl;
        std::stringstream ss;
        ss << sLine;
        char c;

        /*************************************/
        /*    Parse out map data elements    */
        /*************************************/
        while(ss.get(c))
        {
            uint32_t nElementSize = 0;

            if (c == 't')
                continue;

            if (c == '#')
                nElementSize = 1;
            else if (c == 'x' || c == 'y' || c == 'w' || c == 'h')
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
    /*    Format is "x--y--w--h--t"
     *    'x' -> Indicates x position of upper left corner of room
     *    'y' -> Indicates y position of upper left corner of room
     *    'w' -> Indicates width of room
     *    'h' -> Indicates height of room
     *    't' -> Indicates that the remaining
     *           char's will be tile data,
     *           until another 'r' is read
     *    '-' -> A single hex digit each
     */

    /****************************/
    /*    Read position data    */
    /****************************/
    vecRooms.push_back(std::make_shared<Room>());
    std::stringstream ss;
    std::stringstream ssa;
    ss << vecWorld[0][1] << vecWorld[0][2];
    ssa << std::hex << ss.str();
    ssa >> vecRooms[0]->vSize.x;

    ss.str("");
    ssa.str("");
    ss.clear();
    ssa.clear();

    ss << vecWorld[1][1] << vecWorld[1][2];
    ssa << std::hex << ss.str();
    ssa >> vecRooms[0]->vSize.y;

    /*****************************/
    /*    Read dimension data    */
    /*****************************/
    ss.str("");
    ssa.str("");
    ss.clear();
    ssa.clear();

    ss << vecWorld[2][1] << vecWorld[2][2];
    ssa << std::hex << ss.str();
    ssa >> vecRooms[0]->vSize.x;

    ss.str("");
    ssa.str("");
    ss.clear();
    ssa.clear();

    ss << vecWorld[3][1] << vecWorld[3][2];
    ssa << std::hex << ss.str();
    ssa >> vecRooms[0]->vSize.y;

    /************************/
    /*    Read tile data    */
    /************************/
    int32_t i = 2;
    int32_t nEmptyTiles = 0;
    int32_t nNonEmptyTiles = 0;
    for (int32_t y = 0; y < vecRooms[0]->vSize.y; y++)
    {
        for (int32_t x = 0; x < vecRooms[0]->vSize.x; x++)
        {
            if (vecWorld[i] == "#")
            {
                Tile t(glm::ivec2(x, y));
                vecRooms[0]->vecTiles.push_back(t);
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

            vecRooms[0]->vecTiles.push_back(t);

            i++;
        }
    }
}
