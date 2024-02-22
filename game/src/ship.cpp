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
    pOutlineSprite = std::make_unique<Sprite>("outline_sprite");
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
            if (strlen(sFileName) != 0)
                LoadFromFile(sFileName);
            else
                AddRoom(glm::ivec2(0), glm::ivec2(8));
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
        int32_t i = 0;
        float end = r->vecTiles.size();
        for (auto &t : r->vecTiles)
        {
            glm::vec2 vTileScreenPos = cTileWorld.WorldToScreen(t.vWorldPos);
            glm::vec2 vScalar = cTileWorld.GetWorldScale();

            if (!t.bEmpty)
                pSpriteSpaceship->Draw(cRenderer, vTileScreenPos, vScalar, glm::vec2(0.1f), t.vTexOffset);
            else if (cTileWorld.GetGameState() == GameState::LEVEL_EDIT)
                cEmptyTileSprite.DrawColored(cRenderer, vTileScreenPos, vScalar);

            i++;
        }
    }
}



void Ship::DrawSelectedOutline(SpriteRenderer &cRenderer, TileWorld &cTileWorld, std::shared_ptr<Room> pCurrentRoom)
{
    glm::vec2 vScreenPos = cTileWorld.WorldToScreen(pCurrentRoom->vUpperLeftPos);
    glm::vec2 vScalar = cTileWorld.GetWorldScale() * (glm::vec2)pCurrentRoom->vSize;

    pOutlineSprite->Draw(cRenderer, vScreenPos, vScalar, glm::vec2(1.0f), glm::vec2(0.0f));
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



std::shared_ptr<Room> Ship::GetCurrentRoom(glm::vec2 vTilePos)
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
    {
        return false;
    }

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

    if (pCurRoom == nullptr)
        return;

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

    if (pCurRoom == nullptr)
        return;

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



std::shared_ptr<Room> Ship::AddRoom(glm::ivec2 vPos, glm::ivec2 vSize)
{
    vecRooms.push_back(std::make_shared<Room>(true, vPos, vSize));
    vecRooms[vecRooms.size() - 1]->nIndex = vecRooms.size() - 1;

    return vecRooms[vecRooms.size() - 1];
}



std::shared_ptr<Room> Ship::AddRoomFromSelected(std::shared_ptr<Room> pSelectedRoom, CarDir eDir, glm::ivec2 vSize)
{
    if (pSelectedRoom != nullptr && pSelectedRoom->aOpenSides[(int32_t)eDir])
    {
        pSelectedRoom->aOpenSides[(int32_t)eDir] = false;

        glm::ivec2 vNextPos;

        std::shared_ptr<Room> pNewRoom;

        switch(eDir)
        {
            case(CarDir::NORTH):
            {
                vNextPos = pSelectedRoom->vUpperLeftPos - glm::ivec2(0, vSize.y);
                pNewRoom = AddRoom(vNextPos, vSize);
                pNewRoom->aOpenSides[(int32_t)CarDir::SOUTH] = false;
                break;
            }

            case(CarDir::SOUTH):
            {
                vNextPos = pSelectedRoom->vUpperLeftPos + glm::ivec2(0, pSelectedRoom->vSize.y);
                pNewRoom = AddRoom(vNextPos, vSize);
                pNewRoom->aOpenSides[(int32_t)CarDir::NORTH] = false;
                break;
            }

            case(CarDir::EAST):
            {
                vNextPos = pSelectedRoom->vUpperLeftPos + glm::ivec2(pSelectedRoom->vSize.x, 0);
                pNewRoom = AddRoom(vNextPos, vSize);
                pNewRoom->aOpenSides[(int32_t)CarDir::WEST] = false;
                break;
            }

            case(CarDir::WEST):
            {
                vNextPos = pSelectedRoom->vUpperLeftPos - glm::ivec2(vSize.x, 0);
                pNewRoom = AddRoom(vNextPos, vSize);
                pNewRoom->aOpenSides[(int32_t)CarDir::EAST] = false;
                break;
            }
        }

        return pNewRoom;
    }
    else
        return nullptr;
}



std::shared_ptr<Room> Ship::GetFurthestRoom(CarDir eDir)
{
    switch(eDir)
    {
        case(CarDir::NORTH):
        {
            int32_t nFurthest = 0;
            uint32_t nIndex = 0;
            for (int32_t i = 0; i < vecRooms.size(); i++)
            {
                if (nFurthest < vecRooms[i]->vUpperLeftPos.y)
                    continue;

                nFurthest = vecRooms[i]->vUpperLeftPos.y;
                nIndex = i;
            }

            return vecRooms[nIndex];

            break;
        }

        case(CarDir::SOUTH):
        {
            int32_t nFurthest = 0;
            uint32_t nIndex = 0;
            for (int32_t i = 0; i < vecRooms.size(); i++)
            {
                if (nFurthest > vecRooms[i]->vUpperLeftPos.y)
                    continue;

                nFurthest = vecRooms[i]->vUpperLeftPos.y;
                nIndex = i;
            }

            return vecRooms[nIndex];

            break;
        }

        case(CarDir::EAST):
        {
            int32_t nFurthest = 0;
            uint32_t nIndex = 0;
            for (int32_t i = 0; i < vecRooms.size(); i++)
            {
                if (nFurthest > vecRooms[i]->vUpperLeftPos.x)
                    continue;

                nFurthest = vecRooms[i]->vUpperLeftPos.x;
                nIndex = i;
            }

            return vecRooms[nIndex];

            break;
        }

        case(CarDir::WEST):
        {
            int32_t nFurthest = 0;
            uint32_t nIndex = 0;
            for (int32_t i = 0; i < vecRooms.size(); i++)
            {
                if (nFurthest < vecRooms[i]->vUpperLeftPos.x)
                    continue;

                nFurthest = vecRooms[i]->vUpperLeftPos.x;
                nIndex = i;
            }

            return vecRooms[nIndex];

            break;
        }
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

        int32_t vAbsTmp;

        std::cout << "Upper Left: " << glm::to_string(r->vUpperLeftPos) << std::endl;
        ssa << "x";
        vAbsTmp = std::abs(r->vUpperLeftPos.x);
        if (r->vUpperLeftPos.x < 0)
            ssa << "-";

        ssa << std::hex << std::setfill('0') << std::setw(2) << vAbsTmp;

        ssa << "y";
        vAbsTmp = std::abs(r->vUpperLeftPos.y);

        if (r->vUpperLeftPos.y < 0)
            ssa << "-";

        ssa << std::hex << std::setfill('0') << std::setw(2) << vAbsTmp;

        ssa << "w" << std::hex << std::setfill('0') << std::setw(2) << r->vSize.x << "h"
            << std::hex << std::setfill('0') << std::setw(2) << r->vSize.y;

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

        fOutFile << '\n';
    }

    fOutFile.close();

    return true;
}



void Ship::LoadFromFile(const char* cFilename)
{
    glm::ivec2 vSize(0, 0);

    std::ifstream fin;
    if (std::filesystem::exists(cFilename))
    {
        fin.open(cFilename);
    }

    std::vector<std::vector<std::string>> vecWorld;
    std::string sLine;

    /**************************************************/
    /*    Read file into an std::vector of strings    */
    /**************************************************/
    int32_t nLineNumber = 0;
    while(fin >> sLine)
    {
        std::stringstream ss;
        ss << sLine;
        char c;

        std::vector<std::string> vecRoomStr;

        /*************************************/
        /*    Parse out map data elements    */
        /*************************************/
        ss.get(c);
        int32_t it = -1;
        while(true)
        {
            std::stringstream ssa;
            ssa << c;

            std::string sKeys = "xywh";

            if (sKeys.find(c) != std::string::npos)
            {
                ss.get(c);
                while(sKeys.find(c) == std::string::npos && c != 't')
                {
                    ssa << c;
                    ss.get(c);
                }

                vecRoomStr.push_back(ssa.str());
                continue;
            }
            else if (c == 't')
            {
                ss.get(c);
                continue;
            }
            else
            {
                int32_t nElementSize = c == '#' ? 1 : 2;
                bool bEOF = false;
                for (int32_t i = 1; i < nElementSize; i++)
                {
                    ss.get(c);
                    ssa << c;
                }

                vecRoomStr.push_back(ssa.str());

                if (!ss.get(c))
                    break;

                if (bEOF)
                    break;
            }
        }

        vecWorld.push_back(vecRoomStr);
    }

    std::cout << "vecWorld size: " << vecWorld.size() << std::endl;
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
    for (int32_t n = 0; n < vecWorld.size(); n++)
    {
        vecRooms.push_back(std::make_shared<Room>(false));
        std::stringstream ss;
        std::stringstream ssa;

        if (vecWorld[n][0][1] == '-')
        {
            ss << vecWorld[n][0][2] << vecWorld[n][0][3];
            ssa << std::hex << ss.str();
            ssa >> vecRooms[n]->vUpperLeftPos.x;
            vecRooms[n]->vUpperLeftPos.x = -vecRooms[n]->vUpperLeftPos.x;
        }
        else
        {
            ss << vecWorld[n][0][1] << vecWorld[n][0][2];
            ssa << std::hex << ss.str();
            ssa >> vecRooms[n]->vUpperLeftPos.x;
        }


        ss.str("");
        ssa.str("");
        ss.clear();
        ssa.clear();

        if (vecWorld[n][1][1] == '-')
        {
            ss << vecWorld[n][1][2] << vecWorld[n][1][3];
            ssa << std::hex << ss.str();
            ssa >> vecRooms[n]->vUpperLeftPos.y;
            vecRooms[n]->vUpperLeftPos.y = -vecRooms[n]->vUpperLeftPos.y;
        }
        else
        {
            ss << vecWorld[n][1][1] << vecWorld[n][1][2];
            ssa << std::hex << ss.str();
            ssa >> vecRooms[n]->vUpperLeftPos.y;
        }


        /*****************************/
        /*    Read dimension data    */
        /*****************************/
        ss.str("");
        ssa.str("");
        ss.clear();
        ssa.clear();

        ss << vecWorld[n][2][1] << vecWorld[n][2][2];
        ssa << std::hex << ss.str();
        ssa >> vecRooms[n]->vSize.x;

        ss.str("");
        ssa.str("");
        ss.clear();
        ssa.clear();

        ss << vecWorld[n][3][1] << vecWorld[n][3][2];
        ssa << std::hex << ss.str();
        ssa >> vecRooms[n]->vSize.y;

        /************************/
        /*    Read tile data    */
        /************************/
        int32_t i = 4;
        int32_t nEmptyTiles = 0;
        int32_t nNonEmptyTiles = 0;
        std::cout << "Upper left position: " << glm::to_string(vecRooms[n]->vUpperLeftPos) << std::endl;

        glm::ivec2 vStart = vecRooms[n]->vUpperLeftPos;;
        glm::ivec2 vEnd   = vecRooms[n]->vUpperLeftPos + vecRooms[n]->vSize;

        int32_t nCounter = 0;
        for (int32_t y = vStart.y; y < vEnd.y; y++)
        {
            for (int32_t x = vStart.x; x < vEnd.x; x++)
            {
                if (vecWorld[n][i] == "#")
                {
                    Tile t(glm::ivec2(x, y));
                    vecRooms[n]->vecTiles.push_back(t);
                    i++;
                    nEmptyTiles++;
                    nCounter++;
                    continue;
                }

                glm::ivec2 _vTexOffset;
                std::stringstream ss;
                std::stringstream ssa;
                ss << vecWorld[n][i][0];
                ssa << std::hex << ss.str();
                ssa >> _vTexOffset.x;

                ss.str("");
                ssa.str("");
                ss.clear();
                ssa.clear();

                ss << vecWorld[n][i][1];
                ssa << std::hex << ss.str();
                ssa >> _vTexOffset.y;

                Tile t(glm::ivec2(x, y));
                t.vTexOffset = _vTexOffset;
                t.bEmpty = false;
                nNonEmptyTiles++;

                vecRooms[n]->vecTiles.push_back(t);

                i++;
                nCounter++;
            }
        }
    }
}
