#include "app.h"



/************************************/
/*                                  */
/*        L:Global Variables        */
/*                                  */
/************************************/
float fDeltaTime = 0.0f;
float fLastFrame = 0.0f;

glm::vec2 vLastCursorPos = glm::vec2(0.0f);
glm::vec2 vOffset = glm::vec2(0.0f);



App::App(uint32_t _nCanvasWidth, uint32_t _nCanvasHeight)
    : nCanvasWidth(_nCanvasWidth), nCanvasHeight(_nCanvasHeight)
{
}



App::~App()
{
    glfwTerminate();
}



/**************************/
/*                        */
/*        L:Create        */
/*                        */
/**************************/
bool App::Create(const char* str)
{
    std::string sInput;
    if (str == nullptr)
        sInput = "default";
    else
        sInput = std::string(str);

    if (!GLInit())
        return false;

    GLConfig();
    UIConfig();

    LoadResources();

    std::map<std::string, int32_t> mMode = {{ "edit", 0 }, { "play", 1 }, { "default", -1 }};

    switch(mMode[sInput])
    {
        case(-1):
        {
            SetGameState(GameState::PLAY);
            break;
        }

        case(0):
        {
            SetGameState(GameState::LEVEL_EDIT);
            break;
        }

        case(1):
        {
            SetGameState(GameState::PLAY);
            break;
        }
    }

    if (!ConfigEntities())
        return false;



    m_vecThisDebugInfo.push_back("App Debug");
    for (int i = 0; i < 8; i++)
        m_vecThisDebugInfo.push_back("");

    m_vecAllDebugInfo.push_back(&m_vecThisDebugInfo);



    for (int32_t i = 0; i < 10; i++)
    {
        Event e;
        e.mCost["food"] = randInRange(1, 30);
        m_vecAllEvents.push_back(e);
    }

    for (auto e : m_vecAllEvents)
        std::cout << e.mCost["food"] << std::endl;



    glm::vec3 vViewPos = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 vOrigin = glm::vec3(0.0f);
    view = glm::lookAt(vViewPos, vOrigin, glm::vec3(0, 1, 0));

    srand(time(0));

    return true;
}



void App::Destroy()
{
    cRenderer.Destroy();
    ResourceManager::Clear();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(pWindow);
}



/************************************/
/*                                  */
/*        L:GLFW Init/Config        */
/*                                  */
/************************************/
bool App::GLInit()
{
    if (!Renderer::Init_GLFW(&pWindow, nCanvasWidth, nCanvasHeight, bFullscreen))
    {
        std::cout << "ERROR::GLFW" << std::endl;
        return false;
    }


    return true;
}



void App::GLConfig()
{
    glfwSetFramebufferSizeCallback(pWindow, FramebufferSizeCallback);

    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetWindowUserPointer(pWindow, this);
    glfwSetMouseButtonCallback(pWindow, MouseButtonCallback);
    glfwSetKeyCallback(pWindow, KeyCallback);
//    glfwSetCursorPosCallback(pWindow, MouseCallback);
    glfwSetScrollCallback(pWindow, ScrollCallback);
    glfwSetWindowAspectRatio(pWindow, 16, 9);

//    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, nCanvasWidth, nCanvasHeight);
}



/*****************************/
/*                           */
/*        L:UI Config        */
/*                           */
/*****************************/
void App::UIConfig()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::GetIO().DisplaySize.x = 4000.0f;
    ImGui::GetIO().DisplaySize.y = 4000.0f;

    ImGui::GetIO().Fonts->AddFontFromFileTTF("../../../external/imgui/misc/fonts/ProggyClean.ttf", 26);

    ImGui::GetStyle().ScaleAllSizes(2.0f);

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}



/**************************/
/*                        */
/*        L:Update        */
/*                        */
/**************************/
bool App::Update()
{
    if(glfwWindowShouldClose(pWindow))
        return false;

    SetDeltaTime();

    UpdateCursorTile();

    pPlayer->Update();
    pNPC->Update();
    cShip.UpdateRooms();

    RenderApp();
    RenderUI();

    if (m_cUI.bNewEvent)
    {
        ActivateEvent();
        m_cUI.bNewEvent = false;
    }


    if (m_cUI.bNewRoomNorth)
    {
        m_pSelectedRoom = cShip.AddRoomFromSelected(m_pSelectedRoom, CarDir::NORTH, m_vNextRoomSize);
        m_cUI.bNewRoomNorth = false;
    }

    if (m_cUI.bNewRoomSouth)
    {

        m_pSelectedRoom = cShip.AddRoomFromSelected(m_pSelectedRoom, CarDir::SOUTH, m_vNextRoomSize);
        m_cUI.bNewRoomSouth = false;
    }

    if (m_cUI.bNewRoomEast)
    {
        m_pSelectedRoom = cShip.AddRoomFromSelected(m_pSelectedRoom, CarDir::EAST, m_vNextRoomSize);
        m_cUI.bNewRoomEast = false;
    }

    if (m_cUI.bNewRoomWest)
    {
        m_pSelectedRoom = cShip.AddRoomFromSelected(m_pSelectedRoom, CarDir::WEST, m_vNextRoomSize);
        m_cUI.bNewRoomWest = false;
    }

    cI.IsHovered(cTileWorld, GetCursorScreenPos());

    ProcessInput();

    glfwSwapBuffers(pWindow);
    glfwPollEvents();

    return true;
}



void App::UpdateCursorTile()
{
    glm::vec2 vCursorTileFloat = GetCursorWorldPos();
    m_vCursorTile.x = vCursorTileFloat.x < 0.0 ? (int32_t)(vCursorTileFloat.x - 1) : (int32_t(vCursorTileFloat.x));
    m_vCursorTile.y = vCursorTileFloat.y < 0.0 ? (int32_t)(vCursorTileFloat.y - 1) : (int32_t(vCursorTileFloat.y));
    m_vecThisDebugInfo[1] = "Current cursor tile: " + glm::to_string(m_vCursorTile);
}



/**************************/
/**************************/
/*                        */
/*        L:Render        */
/*                        */
/**************************/
/**************************/

/*******************************/
/*                             */
/*        L:Render App        */
/*                             */
/*******************************/
void App::RenderApp()
{
    Renderer::Clear(glm::vec4(0.0f, 0.0f, 0.21f, 1.0f));

    float fWidth  = static_cast<float>(nCanvasWidth);
    float fHeight = static_cast<float>(nCanvasHeight);

    glm::vec2 vScreenSize = bFullscreen ? Renderer::GetScreenSize() : glm::vec2(fWidth, fHeight);

    projection = glm::ortho<float>(0.0f, vScreenSize.x, vScreenSize.y, 0.0f, -1000.0f, 1000.0f);

    ResourceManager::GetShader("sprite").SetMat4("view", view);
    ResourceManager::GetShader("sprite").SetMat4("projection", projection);

    cShip.Draw(cRenderer, cTileWorld);

//    cShip.DrawDoorInteractables(cRenderer, cTileWorld);

//    if (cI.IsHovered(cTileWorld, GetCursorScreenPos()))
//        cI.Draw(cRenderer, cTileWorld);

//    for (auto &d : cShip.vecDoors)
//        d.cI.Draw(cRenderer, cTileWorld);

    switch(eGameState)
    {
        case(GameState::PLAY):
        {

//            pBGSprite->Draw(cRenderer, glm::vec2(0.0f), glm::vec2(vScreenSize.x));

            cCursorTileSprite.DrawColored(cRenderer, cTileWorld.WorldToScreen((glm::vec2)pNPC->GetMoveGoal()), cTileWorld.GetWorldScale());
            cCursorTileSprite.DrawColored(cRenderer, cTileWorld.WorldToScreen((glm::vec2)m_vCursorTile), cTileWorld.GetWorldScale());

            pPlayer->Draw(cRenderer, cTileWorld);
            pNPC->Draw(cRenderer, cTileWorld);

            break;
        }

        case(GameState::LEVEL_EDIT):
        {

            std::shared_ptr<Room> pHoveredRoom = cShip.GetCurrentRoom(cTileWorld.ScreenToWorld(GetCursorScreenPos()));
            if (pHoveredRoom != nullptr && m_bShiftHeld)
            {
                cShip.pOutlineSprite->SetColor(glm::vec3(1.0f));
                cShip.DrawSelectedOutline(cRenderer, cTileWorld, pHoveredRoom);
            }

            if (m_pSelectedRoom != nullptr)
            {
                cShip.pOutlineSprite->SetColor(glm::vec3(0.5f, 0.5f, 0.8f));
                cShip.DrawSelectedOutline(cRenderer, cTileWorld, m_pSelectedRoom);
            }

            if (m_bErase)
                cCursorTileSprite.DrawColored(cRenderer, cTileWorld.WorldToScreen((glm::vec2)m_vCursorTile), cTileWorld.GetWorldScale());
            else if (!m_bShiftHeld)
                cShip.pSpriteSpaceship->Draw(cRenderer, cTileWorld.WorldToScreen((glm::vec2)m_vCursorTile), cTileWorld.GetWorldScale(), glm::vec2(0.1f), cShip.aTexOffsets[cShip.nCurTexOffset]);

            break;
        }
    }
}



/*****************************/
/*                           */
/*        L:Render UI        */
/*                           */
/*****************************/
void App::RenderUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (bShowDemoWindow)
        ImGui::ShowDemoWindow(&bShowDemoWindow);

    std::string sStr = "Resources\n\nFood: " + std::to_string(cShip.nFood) +
                                "\nScrap: " + std::to_string(cShip.nScrap);// +
//                               "\nRoom 1 Air Pressure: " + std::to_string((int32_t)(cShip.vecRooms[0]->fAirPressure * 100)) +
//                               "\nRoom 2 Air Pressure: " + std::to_string((int32_t)(cShip.vecRooms[1]->fAirPressure * 100)) +
//                               "\nRoom 3 Air Pressure: " + std::to_string((int32_t)(cShip.vecRooms[2]->fAirPressure * 100));

    switch(eGameState)
    {
        case(GameState::LEVEL_EDIT):
        {
            m_cUI.RenderLevelEditPanel(glm::ivec2(400, 400));
            break;
        }

        case(GameState::PLAY):
        {
            m_cUI.RenderOverlayPanel(sStr.c_str(), glm::ivec2(400, 200));
            m_cUI.RenderControlPanel(glm::ivec2(400, 200));

            break;
        }
    }

    m_vecThisDebugInfo[1] = "Cursor Screen Position: " + glm::to_string(GetCursorScreenPos());
    m_vecThisDebugInfo[2] = "Cursor World Position: " + glm::to_string(cTileWorld.ScreenToWorld(GetCursorScreenPos()));
    m_vecThisDebugInfo[3] = "World Offset: " + glm::to_string(cTileWorld.GetWorldOffset());
    m_vecThisDebugInfo[4] = "Screen Size: " + glm::to_string(Renderer::GetScreenSize());

    if (bShowDebugPanel)
        m_cUI.RenderDebugPanel(m_vecAllDebugInfo);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



void App::ActivateEvent()
{
    static int32_t m_nCurEvent = 0;

    if (m_nCurEvent < m_vecAllEvents.size())
        cShip.ActivateEvent(m_vecAllEvents[m_nCurEvent++]);
    else
        std::cout << "No events remaining" << std::endl;
}



/**********************************/
/*                                */
/*        L:Set Delta Time        */
/*                                */
/**********************************/
void App::SetDeltaTime()
{
    float fCurFrame = static_cast<float>(glfwGetTime());
    fDeltaTime = fCurFrame - fLastFrame;
    fLastFrame = fCurFrame;
}



void App::SetGameState(GameState _eState)
{
    eGameState = _eState;
    cShip.SetGameState(_eState);
    cTileWorld.SetGameState(_eState);
}




/******************************************/
/*                                        */
/*        L:Process Mouse Movement        */
/*                                        */
/******************************************/



glm::vec2 App::GetCursorScreenPos()
{
    glm::dvec2 dvPos;
    glfwGetCursorPos(pWindow, &dvPos.x, &dvPos.y);

    return glm::vec2(dvPos);
}



glm::vec2 App::GetCursorWorldPos()
{
    return cTileWorld.ScreenToWorld(GetCursorScreenPos());
}



/**********************************/
/*                                */
/*        L:Load Resources        */
/*                                */
/**********************************/
void App::LoadResources()
{
    ResourceManager::LoadShader("../../shaders/sprite.vert", "../../shaders/sprite.frag", nullptr, "sprite");
    //ResourceManager::GetShader("sprite").Use();
    //ResourceManager::GetShader("sprite").SetInt("sImage", 0);

    Shader s = ResourceManager::GetShader("sprite");
    cRenderer.Create(s);

    ResourceManager::LoadTexture("../../res/Texture/awesomeface.png", true, "debug");
    ResourceManager::LoadTexture("../../res/Texture/TX Tileset Grass.png", true, "grass");
    ResourceManager::LoadTexture("../../res/asset_packs/simple_space_station_tileset/TileSet v1.0.png", true, "spaceship_sheet");
    ResourceManager::LoadTexture("../../res/asset_packs/TopDown_RockBoss_EBrosAssets/TopDown_RockBoss_EBrosAssets/walk/spritesheet.png", true, "rock_walk_backward");
    ResourceManager::LoadTexture("../../res/asset_packs/TopDown_RockBoss_EBrosAssets/TopDown_RockBoss_EBrosAssets/walk/spritesheet-2.png", true, "rock_walk_forward");
    ResourceManager::LoadTexture("../../res/asset_packs/TopDown_RockBoss_EBrosAssets/TopDown_RockBoss_EBrosAssets/walk/spritesheet-3.png", true, "rock_walk_left");
    ResourceManager::LoadTexture("../../res/asset_packs/TopDown_RockBoss_EBrosAssets/TopDown_RockBoss_EBrosAssets/walk/spritesheet-4.png", true, "rock_walk_right");

    ResourceManager::LoadTexture("../../res/asset_packs/TopDown_RockBoss_EBrosAssets/TopDown_RockBoss_EBrosAssets/idle/spritesheet-5.png", true, "rock_idle_backward");
    ResourceManager::LoadTexture("../../res/asset_packs/TopDown_RockBoss_EBrosAssets/TopDown_RockBoss_EBrosAssets/idle/spritesheet-6.png", true, "rock_idle_forward");
    ResourceManager::LoadTexture("../../res/asset_packs/TopDown_RockBoss_EBrosAssets/TopDown_RockBoss_EBrosAssets/idle/spritesheet-7.png", true, "rock_idle_left");
    ResourceManager::LoadTexture("../../res/asset_packs/TopDown_RockBoss_EBrosAssets/TopDown_RockBoss_EBrosAssets/idle/spritesheet-8.png", true, "rock_idle_right");

    ResourceManager::LoadTexture("../../res/asset_packs/Pixel Crawler - FREE - 1.8/Heroes/Wizzard/Idle/Idle-Sheet.png", true, "wizard_idle");
    ResourceManager::LoadTexture("../../res/asset_packs/Pixel Crawler - FREE - 1.8/Heroes/Wizzard/Run/Run-Sheet.png", true, "wizard_walk");

    ResourceManager::LoadTexture("../../res/Space Background.png", true, "bg_space_01");
    ResourceManager::LoadTexture("../../res/Outline.png", true, "outline_sprite");

    // TODO - Figure out "cannot bind non-const lvalue reference of type 'Shader&' to an rvalue of type 'Shader'" error

    //pRenderer = std::make_shared<SpriteRenderer>(ResourceManager::GetShader("sprite"));
    //pRenderer->Create(ResourceManager::GetShader("sprite"));

}



bool App::ConfigEntities()
{
    cTileWorld.Create(glm::ivec2(64, 64), glm::vec2(100.0f));

    m_vecAllDebugInfo.push_back(&cI.vecDebugInfo);
    cI.vPos = glm::vec2(4.0f);

    switch(eGameState)
    {
        case(GameState::LEVEL_EDIT):
        {
            if (!cShip.Create(eGameState, "world_14.txt"))
                return false;

            m_pSelectedRoom = cShip.vecRooms[0];
            break;
        }

        case(GameState::PLAY):
        {
            if (!cShip.Create(eGameState, "world_20.txt"))
                return false;
            break;
        }
    }

    m_vecAllDebugInfo.push_back(&cShip.vecDebugInfo);

    cCursorTileSprite.SetColor(glm::vec3(0.15f, 0.25f, 0.40f));

    pBGSprite = std::make_unique<Sprite>("bg_space_01");

    m_vecAllDebugInfo.push_back(&cTileWorld.vecDebugInfo);

    /*********************************/
    /*                               */
    /*        L:Player Config        */
    /*                               */
    /*********************************/

    pPlayer = std::make_unique<Player>(glm::vec2(1.0f, 2.0f));

    pPlayer->AddAnimatedSprite("rock_walk_backward", "walk_backward");
    pPlayer->AddAnimatedSprite("rock_walk_forward",  "walk_forward");
    pPlayer->AddAnimatedSprite("rock_walk_left",     "walk_left");
    pPlayer->AddAnimatedSprite("rock_walk_right",    "walk_right");

    pPlayer->AddAnimatedSprite("rock_idle_backward", "idle_backward");
    pPlayer->AddAnimatedSprite("rock_idle_forward",  "idle_forward");
    pPlayer->AddAnimatedSprite("rock_idle_left",     "idle_left");
    pPlayer->AddAnimatedSprite("rock_idle_right",    "idle_right");

    // TPF -> Ticks Per Frame
    uint32_t nTPF = 2;
    uint32_t nNumFrames = 10;
    pPlayer->ConfigAnimatedSprite("walk_backward", nNumFrames, nTPF, glm::vec2(0), glm::vec2(0.1f, 1.0f), glm::vec2(0.1f, 1.0f), glm::vec2(1.5f));
    pPlayer->ConfigAnimatedSprite("walk_forward",  nNumFrames, nTPF, glm::vec2(0), glm::vec2(0.1f, 1.0f), glm::vec2(0.1f, 1.0f), glm::vec2(1.5f));
    pPlayer->ConfigAnimatedSprite("walk_left",     nNumFrames, nTPF, glm::vec2(0), glm::vec2(0.1f, 1.0f), glm::vec2(0.1f, 1.0f), glm::vec2(1.5f));
    pPlayer->ConfigAnimatedSprite("walk_right",    nNumFrames, nTPF, glm::vec2(0), glm::vec2(0.1f, 1.0f), glm::vec2(0.1f, 1.0f), glm::vec2(1.5f));

    nTPF = 8;
    nNumFrames = 7;
    pPlayer->ConfigAnimatedSprite("idle_backward", nNumFrames, nTPF, glm::vec2(0), glm::vec2(1.0f / 7.0f, 1.0f), glm::vec2(1.0f / 7.0f, 1.0f), glm::vec2(1.5f));
    pPlayer->ConfigAnimatedSprite("idle_forward",  nNumFrames, nTPF, glm::vec2(0), glm::vec2(1.0f / 7.0f, 1.0f), glm::vec2(1.0f / 7.0f, 1.0f), glm::vec2(1.5f));
    pPlayer->ConfigAnimatedSprite("idle_left",     nNumFrames, nTPF, glm::vec2(0), glm::vec2(1.0f / 7.0f, 1.0f), glm::vec2(1.0f / 7.0f, 1.0f), glm::vec2(1.5f));
    pPlayer->ConfigAnimatedSprite("idle_right",    nNumFrames, nTPF, glm::vec2(0), glm::vec2(1.0f / 7.0f, 1.0f), glm::vec2(1.0f / 7.0f, 1.0f), glm::vec2(1.5f));

    pPlayer->SetMoveSpeedScalar(0.050f);
    pPlayer->SetState(CharacterState::IDLE);
    pPlayer->StartSpriteAnim();

    /******************************/
    /*                            */
    /*        L:NPC Config        */
    /*                            */
    /******************************/
    pNPC = std::make_unique<CrewMember>(glm::vec2(1.0f, 4.0f));
//    pNPC = new Character(glm::vec2(1.0f, 4.0f));

    pNPC->AddAnimatedSprite("wizard_idle", "idle_right");
    pNPC->AddAnimatedSprite("wizard_idle", "idle_left");
    pNPC->AddAnimatedSprite("wizard_walk", "walk_right");
    pNPC->AddAnimatedSprite("wizard_walk", "walk_left");

    nTPF = 8;
    nNumFrames = 4;
    pNPC->ConfigAnimatedSprite("idle_right", nNumFrames, nTPF, glm::vec2(0), glm::vec2(1.0f / 4.0f, 1.0f), glm::vec2(1.0f / 4.0f, 1.0f), glm::vec2(1.0f));
    pNPC->ConfigAnimatedSprite("idle_left", nNumFrames, nTPF, glm::vec2(0), glm::vec2(1.0f / 4.0f, 1.0f), glm::vec2(1.0f / 4.0f, 1.0f), glm::vec2(-1.0f, 1.0f));

    nTPF = 6;
    nNumFrames = 6;
    pNPC->ConfigAnimatedSprite("walk_right", nNumFrames, nTPF, glm::vec2(0), glm::vec2(1.0f / 6.0f, 1.0f), glm::vec2(1.0f / 6.0f, 1.0f), glm::vec2(2.0f));
    pNPC->ConfigAnimatedSprite("walk_left", nNumFrames, nTPF, glm::vec2(0), glm::vec2(1.0f / 6.0f, 1.0f), glm::vec2(1.0f / 6.0f, 1.0f), glm::vec2(-2.0f, 2.0f));

    pNPC->SetAnimatedSpritePosOffset("idle_left", glm::vec2(1.0f, 0.0f));
    pNPC->SetAnimatedSpritePosOffset("idle_right", glm::vec2(1.0f, 0.0f));

    pNPC->SetAnimatedSpritePosOffset("walk_left", glm::vec2(1.5f, -1.0f));
    pNPC->SetAnimatedSpritePosOffset("walk_right", glm::vec2(-0.5f, -1.0f));

    pNPC->SetMoveSpeedScalar(0.05f);
    pNPC->SetState(CharacterState::WALK);
    pNPC->SetDir(Direction::LEFT);
    pNPC->SetMoveGoal(pNPC->vWorldPos);
    pNPC->StartSpriteAnim();

    m_vecAllDebugInfo.push_back(&pPlayer->vecDebugInfo);
    m_vecAllDebugInfo.push_back(&pNPC->vecDebugInfo);

    return true;
}



/*********************************/
/*                               */
/*        L:Process Input        */
/*                               */
/*********************************/
void App::ProcessInput()
{
    if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
        ProcessMouseInput();

    float fMoveScalar = 0.1f;
    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        pPlayer->Move(Direction::FORWARD);
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        pPlayer->Move(Direction::BACKWARD);
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        pPlayer->Move(Direction::LEFT);
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        pPlayer->Move(Direction::RIGHT);
    }
    else
    {
        pPlayer->SetState(CharacterState::IDLE);
    }

    if (eGameState == GameState::LEVEL_EDIT)
    {
        if (glfwGetKey(pWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            m_bCtrlHeld = true;
        }
        else
        {
            m_bCtrlHeld = false;
        }

        if (glfwGetKey(pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            m_bShiftHeld = true;
        }
        else
        {
            m_bShiftHeld = false;
        }
    }
}



/***************************************/
/*                                     */
/*        L:Process Mouse Input        */
/*                                     */
/***************************************/
void App::ProcessMouseInput()
{
    glm::vec2 vPos = GetCursorScreenPos();
    if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    {
        if (bPanning)
        {
            cTileWorld.UpdatePan(vPos);
            m_vCursorMoveDelta += cTileWorld.ScreenToWorld(vPos);
        }
        else
        {
            m_vCursorMoveDelta = glm::vec2(0.0f);
            cTileWorld.StartPan(vPos);
            bPanning = true;
        }
    }
    else if (bPanning)
    {
        cTileWorld.EndPan(vPos);
        bPanning = false;
    }

    if (eGameState == GameState::LEVEL_EDIT)
    {
        if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            m_bErase = true;
        else
            m_bErase = false;

        if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && cShip.EmptyTile(m_vCursorTile) && !m_bShiftHeld)
        {
            cShip.AddTile(m_vCursorTile);
        }

        if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !cShip.EmptyTile(m_vCursorTile) && !m_bShiftHeld)
        {
            cShip.RemoveTile(m_vCursorTile);
        }
    }
}




/**************************************/
/**************************************/
/*                                    */
/*        L:Callback Functions        */
/*                                    */
/**************************************/
/**************************************/



/**********************************/
/*                                */
/*        L:Mouse Callback        */
/*                                */
/**********************************/
void App::MouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods)
{
    App* pApp = static_cast<App*>(glfwGetWindowUserPointer(pWindow));
    /********************************************/
    /*                                          */
    /*        L:Level Edit mode specific        */
    /*                                          */
    /********************************************/
    if (pApp->eGameState != GameState::LEVEL_EDIT)
        return;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && pApp->m_bShiftHeld)
    {
        std::cout << "Selecting room" << std::endl;
        pApp->m_pSelectedRoom = pApp->cShip.GetCurrentRoom(pApp->cTileWorld.ScreenToWorld(pApp->GetCursorScreenPos()));
    }
}



void App::KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    App* pApp = static_cast<App*>(glfwGetWindowUserPointer(pWindow));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        Renderer::CloseWindow(pWindow);
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        if (pApp->bFullscreen)
        {
            Renderer::ExitFullscreen(pWindow, glm::ivec2(400), glm::ivec2(pApp->nCanvasWidth, pApp->nCanvasHeight));
            pApp->bFullscreen = false;
        }
        else
        {
            Renderer::EnterFullscreen(pWindow);

            pApp->bFullscreen = true;
        }
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        pApp->cTileWorld.SetWorldOffset(glm::vec2(0.0f));
        pApp->cTileWorld.SetWorldScale((glm::vec2(100.0f)));
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        pApp->cShip.bDoorsOpen = true;
    }

    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        if (++pApp->cShip.nCurTexOffset >= pApp->cShip.aTexOffsets.size())
            pApp->cShip.nCurTexOffset = 0;
    }

    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        if (--pApp->cShip.nCurTexOffset < 0)
            pApp->cShip.nCurTexOffset = pApp->cShip.aTexOffsets.size() - 1;
    }

    if (key == GLFW_KEY_B && action == GLFW_PRESS)
        pApp->bShowDebugPanel = !pApp->bShowDebugPanel;

    /********************************************/
    /*                                          */
    /*        L:Level Edit mode specific        */
    /*                                          */
    /********************************************/
    if (pApp->eGameState != GameState::LEVEL_EDIT)
        return;

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        std::stringstream ss;
        ss << "world_" << pApp->nCurFileNum << ".txt";
        std::string sFilename = ss.str();
        std::cout << "Filename: " << sFilename << std::endl;
        while (!pApp->cShip.SaveToFile(sFilename))
        {
            pApp->nCurFileNum++;
            std::stringstream ss;
            ss << "world_" << pApp->nCurFileNum << ".txt";
            sFilename = ss.str();
            std::cout << "Filename: " << sFilename << std::endl;
        }
    }
}



void App::ScrollCallback(GLFWwindow* pWindow, double xoffset, double yoffset)
{
    App* pApp = static_cast<App*>(glfwGetWindowUserPointer(pWindow));

    if (pApp->m_bCtrlHeld)
    {
        if (yoffset > 0)
        {
            if (++pApp->cShip.nCurTexOffset >= pApp->cShip.aTexOffsets.size())
                pApp->cShip.nCurTexOffset = 0;
        }
        else
        {
            if (--pApp->cShip.nCurTexOffset < 0)
                pApp->cShip.nCurTexOffset = pApp->cShip.aTexOffsets.size() - 1;
        }
    }
    else
    {
        if (yoffset > 0)
            pApp->cTileWorld.ZoomAtScreenPos(2.0f, pApp->GetCursorScreenPos());
        else
            pApp->cTileWorld.ZoomAtScreenPos(0.5f, pApp->GetCursorScreenPos());
    }
}



void App::FramebufferSizeCallback(GLFWwindow* pWindow, int nWidth, int nHeight)
{
    glViewport(0, 0, nWidth, nHeight);
}



int randInRange(int32_t nMin, int32_t nMax)
{
    int32_t nRange = nMax - nMin + 1;

    return rand() % nRange + nMin;
}
