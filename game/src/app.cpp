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
bool App::Create()
{
    if (!GLInit())
        return false;

    GLConfig();
    UIConfig();

    LoadResources();

    m_vecThisDebugInfo.push_back("App Debug");
    for (int i = 0; i < 8; i++)
        m_vecThisDebugInfo.push_back("");

    m_vecAllDebugInfo.push_back(&m_vecThisDebugInfo);

    SetGameState(GameState::PLAY);

    for (int32_t i = 0; i < 10; i++)
    {
        Event e;
        e.mCost["food"] = randInRange(1, 30);
        m_vecAllEvents.push_back(e);
    }

    for (auto e : m_vecAllEvents)
        std::cout << e.mCost["food"] << std::endl;

    ConfigEntities();

    glm::vec3 vViewPos = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 vOrigin = glm::vec3(0.0f);
    view = glm::lookAt(vViewPos, vOrigin, glm::vec3(0, 1, 0));

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

    m_pPlayer->Update();

    RenderApp();
    RenderUI();

    if (m_cUI.bNewEvent)
    {
        EventUpdate();
        m_cUI.bNewEvent = false;
    }

    ProcessInput();

    glfwSwapBuffers(pWindow);
    glfwPollEvents();

    return true;
}



void App::UpdateCursorTile()
{
    glm::vec2 vCursorTileFloat = GetCursorWorldPos() / BASE_TILE_SIZE;
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

    cWorld.Draw(cRenderer);
    m_pPlayer->Draw(cRenderer, cWorld);

    if (eGameState == GameState::LEVEL_EDIT)
    {
        if (m_bErase)
            cCursorTileSprite.DrawColored(cRenderer, (glm::vec2)m_vCursorTile * BASE_TILE_SIZE * cWorld.GetWorldScale() - cWorld.GetWorldOffset() * cWorld.GetWorldScale(), cWorld.GetWorldScale());
        else
            cWorld.pSpriteSpaceship->Draw(cRenderer, (glm::vec2)m_vCursorTile * BASE_TILE_SIZE * cWorld.GetWorldScale() - cWorld.GetWorldOffset() * cWorld.GetWorldScale(), cWorld.GetWorldScale(), glm::vec2(0.1f), cWorld.aTexOffsets[cWorld.nCurTexOffset]);
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

    std::string sStr = "Resources\n\nFood: " + std::to_string(m_pShip->nFood) +
                       "\nScrap: " + std::to_string(m_pShip->nScrap);

    m_vecThisDebugInfo[1] = "Cursor Position: " + glm::to_string(GetCursorScreenPos());
    m_vecThisDebugInfo[2] = "World Offset: " + glm::to_string(cWorld.GetWorldOffset());
    m_vecThisDebugInfo[3] = "Screen Size: " + glm::to_string(Renderer::GetScreenSize());
    m_cUI.RenderOverlayPanel(sStr.c_str(), glm::ivec2(400, 200));
    m_cUI.RenderControlPanel(glm::ivec2(400, 200));

    if (bShowDebugPanel)
        m_cUI.RenderDebugPanel(m_vecAllDebugInfo);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



void App::EventUpdate()
{
    static int32_t m_nCurEvent = 0;

    if (m_nCurEvent < m_vecAllEvents.size())
        m_pShip->EventUpdate(m_vecAllEvents[m_nCurEvent++]);
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
    cWorld.SetGameState(_eState);
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
    return cWorld.ScreenToWorld(GetCursorScreenPos());
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

    // TODO - Figure out "cannot bind non-const lvalue reference of type 'Shader&' to an rvalue of type 'Shader'" error

    //pRenderer = std::make_shared<SpriteRenderer>(ResourceManager::GetShader("sprite"));
    //pRenderer->Create(ResourceManager::GetShader("sprite"));

}



void App::ConfigEntities()
{
    m_pShip = std::make_unique<Ship>();
    m_vecAllDebugInfo.push_back(&m_pShip->vecDebugInfo);

    cWorld.Create(glm::ivec2(64, 64), glm::vec2(100.0f));
    cWorld.LoadFromFile("world_1.txt");
    cCursorTileSprite.SetColor(glm::vec3(0.15f, 0.25f, 0.40f));

    m_vecAllDebugInfo.push_back(&cWorld.vecDebugInfo);

    m_pPlayer = std::make_unique<Player>(glm::vec2(1.0f, 2.0f));

    m_pPlayer->AddAnimatedSprite("rock_walk_backward", "walk_backward");
    m_pPlayer->AddAnimatedSprite("rock_walk_forward",  "walk_forward");
    m_pPlayer->AddAnimatedSprite("rock_walk_left",     "walk_left");
    m_pPlayer->AddAnimatedSprite("rock_walk_right",    "walk_right");

    m_pPlayer->AddAnimatedSprite("rock_idle_backward", "idle_backward");
    m_pPlayer->AddAnimatedSprite("rock_idle_forward",  "idle_forward");
    m_pPlayer->AddAnimatedSprite("rock_idle_left",     "idle_left");
    m_pPlayer->AddAnimatedSprite("rock_idle_right",    "idle_right");

    // TPF -> Ticks Per Frame
    uint32_t nTPF = 2;
    uint32_t nNumFrames = 10;
    m_pPlayer->ConfigAnimatedSprite("walk_backward", nNumFrames, nTPF, glm::vec2(0), glm::vec2(0.1f, 1.0f), glm::vec2(0.1f, 1.0f), glm::vec2(1.5f));
    m_pPlayer->ConfigAnimatedSprite("walk_forward",  nNumFrames, nTPF, glm::vec2(0), glm::vec2(0.1f, 1.0f), glm::vec2(0.1f, 1.0f), glm::vec2(1.5f));
    m_pPlayer->ConfigAnimatedSprite("walk_left",     nNumFrames, nTPF, glm::vec2(0), glm::vec2(0.1f, 1.0f), glm::vec2(0.1f, 1.0f), glm::vec2(1.5f));
    m_pPlayer->ConfigAnimatedSprite("walk_right",    nNumFrames, nTPF, glm::vec2(0), glm::vec2(0.1f, 1.0f), glm::vec2(0.1f, 1.0f), glm::vec2(1.5f));

    nTPF = 8;
    nNumFrames = 7;
    m_pPlayer->ConfigAnimatedSprite("idle_backward", nNumFrames, nTPF, glm::vec2(0), glm::vec2(1.0f / 7.0f, 1.0f), glm::vec2(1.0f / 7.0f, 1.0f), glm::vec2(1.5f));
    m_pPlayer->ConfigAnimatedSprite("idle_forward",  nNumFrames, nTPF, glm::vec2(0), glm::vec2(1.0f / 7.0f, 1.0f), glm::vec2(1.0f / 7.0f, 1.0f), glm::vec2(1.5f));
    m_pPlayer->ConfigAnimatedSprite("idle_left",     nNumFrames, nTPF, glm::vec2(0), glm::vec2(1.0f / 7.0f, 1.0f), glm::vec2(1.0f / 7.0f, 1.0f), glm::vec2(1.5f));
    m_pPlayer->ConfigAnimatedSprite("idle_right",    nNumFrames, nTPF, glm::vec2(0), glm::vec2(1.0f / 7.0f, 1.0f), glm::vec2(1.0f / 7.0f, 1.0f), glm::vec2(1.5f));

    m_pPlayer->SetMoveSpeedScalar(0.050f);
    m_pPlayer->SetState(CharacterState::IDLE);
    m_pPlayer->StartSpriteAnim();

    m_vecAllDebugInfo.push_back(&m_pPlayer->vecDebugInfo);
}



void App::PrintVar(int32_t var)
{
    std::cout << var << std::endl;
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
        m_pPlayer->Move(Direction::FORWARD);
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_pPlayer->Move(Direction::BACKWARD);
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_pPlayer->Move(Direction::LEFT);
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_pPlayer->Move(Direction::RIGHT);
    }
    else
    {
        m_pPlayer->SetState(CharacterState::IDLE);
    }

    if (eGameState == GameState::LEVEL_EDIT)
    {
        if (glfwGetKey(pWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            m_bTileSelectMod = true;
        }
        else
        {
            m_bTileSelectMod = false;
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
            cWorld.UpdatePan(vPos);
            m_vCursorMoveDelta += cWorld.ScreenToWorld(vPos);
        }
        else
        {
            m_vCursorMoveDelta = glm::vec2(0.0f);
            cWorld.StartPan(vPos);
            bPanning = true;
        }
    }
    else if (bPanning)
    {
        cWorld.EndPan(vPos);
        bPanning = false;
    }

    if (eGameState == GameState::LEVEL_EDIT)
    {
        if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            m_bErase = true;
        else
            m_bErase = false;

        if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && cWorld.EmptyTile(m_vCursorTile))
        {
            cWorld.AddTile(m_vCursorTile);
        }

        if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !cWorld.EmptyTile(m_vCursorTile))
        {
            cWorld.RemoveTile(m_vCursorTile);
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
void MouseCallback(GLFWwindow* pWindow, double fPosInX, double fPosInY)
{
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
        pApp->cWorld.SetWorldOffset(glm::vec2(0.0f));
        pApp->cWorld.SetWorldScale((glm::vec2(100.0f)));
    }

    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        if (++pApp->cWorld.nCurTexOffset >= pApp->cWorld.aTexOffsets.size())
            pApp->cWorld.nCurTexOffset = 0;
    }

    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        if (--pApp->cWorld.nCurTexOffset < 0)
            pApp->cWorld.nCurTexOffset = pApp->cWorld.aTexOffsets.size() - 1;
    }

    if (key == GLFW_KEY_B && action == GLFW_PRESS)
        pApp->bShowDebugPanel = !pApp->bShowDebugPanel;

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS && pApp->eGameState == GameState::LEVEL_EDIT)
    {
        std::stringstream ss;
        ss << "world_" << pApp->nCurFileNum << ".txt";
        std::string sFilename = ss.str();
        std::cout << "Filename: " << sFilename << std::endl;
        while (!pApp->cWorld.SaveToFile(sFilename))
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

    if (pApp->m_bTileSelectMod)
    {
        if (yoffset > 0)
        {
            if (++pApp->cWorld.nCurTexOffset >= pApp->cWorld.aTexOffsets.size())
                pApp->cWorld.nCurTexOffset = 0;
        }
        else
        {
            if (--pApp->cWorld.nCurTexOffset < 0)
                pApp->cWorld.nCurTexOffset = pApp->cWorld.aTexOffsets.size() - 1;
        }
    }
    else
    {
        if (yoffset > 0)
            pApp->cWorld.ZoomAtScreenPos(2.0f, pApp->GetCursorScreenPos());
        else
            pApp->cWorld.ZoomAtScreenPos(0.5f, pApp->GetCursorScreenPos());
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
