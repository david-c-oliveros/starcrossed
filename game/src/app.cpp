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

    m_vecDebugInfo = { "App Debug:" };
    for (int i = 0; i < 8; i++)
        m_vecDebugInfo.push_back("");

    SetAppState(AppState::PLAY);
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
    if (!Renderer::Init_GLFW(&pWindow, nCanvasWidth, nCanvasHeight))
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
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableApppad;

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

    RenderApp();
    RenderUI();
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
    m_vecDebugInfo[1] = "Current cursor tile: " + glm::to_string(m_vCursorTile);
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

    projection = glm::ortho<float>(0.0f, fWidth, fHeight, 0.0f, -1000.0f, 1000.0f);

    ResourceManager::GetShader("sprite").SetMat4("view", view);
    ResourceManager::GetShader("sprite").SetMat4("projection", projection);

    cWorld.Draw(cRenderer);
    pPlayer->Draw(cRenderer, cWorld);

    if (m_eAppState == AppState::LEVEL_EDIT)
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

    if (m_bShowUIWindow)
        ImGui::ShowDemoWindow(&m_bShowUIWindow);

    {

        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");

        ImGui::Text("This is some useful text.");
        ImGui::Checkbox("Demo Window", &m_bShowUIWindow);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&m_vUIClearColor);

        if (ImGui::Button("Button"))
            counter++;

        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    if (m_bShowDebugInfo)
    {
        ImGui::Begin("Debug");
        
        ImGui::Checkbox("Toggle player state", &bPlayerIdle);

        for (auto &s : m_vecDebugInfo)
            ImGui::Text(s.c_str());

        for (auto &s : cWorld.vecDebugInfo)
            ImGui::Text(s.c_str());

        for (auto &s : cRenderer.vecDebugInfo)
            ImGui::Text(s.c_str());

        for (auto &s : pPlayer->vecDebugInfo)
            ImGui::Text(s.c_str());

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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



void App::SetAppState(AppState _eState)
{
    m_eAppState = _eState;
    cWorld.SetAppState(_eState);
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
    cWorld.Create(glm::ivec2(32, 32), glm::vec2(100.0f));
    cWorld.LoadFromFile("world_1.txt");
    cCursorTileSprite.SetColor(glm::vec3(0.15f, 0.25f, 0.40f));

    pPlayer = std::make_unique<Character>(glm::vec2(1.0f, 2.0f));

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

    if (m_eAppState == AppState::LEVEL_EDIT)
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

    if (m_eAppState == AppState::LEVEL_EDIT)
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
        glfwSetWindowShouldClose(pWindow, true);
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        pApp->cWorld.SetWorldOffset(glm::vec2(0.0f));
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

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
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
