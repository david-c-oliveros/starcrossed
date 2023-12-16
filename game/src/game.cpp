#include "game.h"



/************************************/
/*                                  */
/*        L:Global Variables        */
/*                                  */
/************************************/
float fDeltaTime = 0.0f;
float fLastFrame = 0.0f;

glm::vec2 vLastCursorPos = glm::vec2(0.0f);
glm::vec2 vOffset = glm::vec2(0.0f);



Game::Game(uint32_t _nCanvasWidth, uint32_t _nCanvasHeight)
    : nCanvasWidth(_nCanvasWidth), nCanvasHeight(_nCanvasHeight)
{
}



Game::~Game()
{
    glfwTerminate();
}



/**************************/
/*                        */
/*        L:Create        */
/*                        */
/**************************/
bool Game::Create()
{
    if (!GLInit())
        return false;

    GLConfig();
    UIConfig();

    LoadResources();

    glm::vec3 vViewPos = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 vOrigin = glm::vec3(0.0f);
    view = glm::lookAt(vViewPos, vOrigin, glm::vec3(0, 1, 0));

    cWorld.Create(glm::ivec2(32, 32), glm::vec2(1.0f));
    
    pPlayer = std::make_unique<Character>(glm::vec2(0.0f));
    pPlayer->AddSpriteSheet("rogue_idle_sheet", "idle");
    pPlayer->AddSpriteSheet("rogue_run_sheet", "run");
    pPlayer->AddSpriteSheet("debug", "debug");

    return true;
}



/************************************/
/*                                  */
/*        L:GLFW Init/Config        */
/*                                  */
/************************************/
bool Game::GLInit()
{
    if (!Renderer::Init_GLFW(&pWindow, nCanvasWidth, nCanvasHeight))
    {
        std::cout << "ERROR::GLFW" << std::endl;
        return false;
    }

    return true;
}



void Game::GLConfig()
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
void Game::UIConfig()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}



/**************************/
/*                        */
/*        L:Update        */
/*                        */
/**************************/
bool Game::Update()
{
    if(glfwWindowShouldClose(pWindow))
        return false;

    SetDeltaTime();
    RenderGame();
    RenderUI();
    ProcessInput();
    PrintDebug();

    glfwSwapBuffers(pWindow);
    glfwPollEvents();

    return true;
}



void Game::Destroy()
{
    cRenderer.Destroy();
    ResourceManager::Clear();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(pWindow);
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
/*        L:Render Game        */
/*                             */
/*******************************/
void Game::RenderGame()
{
    Renderer::Clear(glm::vec4(0.0f, 0.0f, 0.21f, 1.0f));

    float fWidth  = static_cast<float>(nCanvasWidth);
    float fHeight = static_cast<float>(nCanvasHeight);

    projection = glm::ortho<float>(0.0f, fWidth, fHeight, 0.0f, -1000.0f, 1000.0f);

    ResourceManager::GetShader("sprite").SetMat4("view", view);
    ResourceManager::GetShader("sprite").SetMat4("projection", projection);

    if (m_bShowWorld)
        cWorld.Draw(cRenderer);

    if (m_bShowCharacter)
    pPlayer->Draw(cRenderer, cWorld);
}



/*****************************/
/*                           */
/*        L:Render UI        */
/*                           */
/*****************************/
void Game::RenderUI()
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
        std::string sCursorWorldPos = "Cursor World Pos: " + glm::to_string(cWorld.ScreenToWorld(GetCursorPos()));
        std::string sCursorMoveDelta = "Cursor Move Delta: " + glm::to_string(m_vCursorMoveDelta);

        ImGui::Begin("Debug");
        
        ImGui::Checkbox("Show map", &m_bShowWorld);
        ImGui::Checkbox("Show character", &m_bShowCharacter);

        for (auto &s : cWorld.vecDebugInfo)
            ImGui::Text(s.c_str());

        for (auto &s : cRenderer.vecDebugInfo)
            ImGui::Text(s.c_str());

        ImGui::Text(sCursorWorldPos.c_str());
        ImGui::Text(sCursorMoveDelta.c_str());

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
void Game::SetDeltaTime()
{
    float fCurFrame = static_cast<float>(glfwGetTime());
    fDeltaTime = fCurFrame - fLastFrame;
    fLastFrame = fCurFrame;
}




/******************************************/
/*                                        */
/*        L:Process Mouse Movement        */
/*                                        */
/******************************************/



glm::vec2 Game::GetCursorPos()
{
    glm::dvec2 dvPos;
    glfwGetCursorPos(pWindow, &dvPos.x, &dvPos.y);

    return glm::vec2(dvPos);
}



/**********************************/
/*                                */
/*        L:Load Resources        */
/*                                */
/**********************************/
void Game::LoadResources()
{
    ResourceManager::LoadShader("../../shaders/sprite.vert", "../../shaders/sprite.frag", nullptr, "sprite");
    //ResourceManager::GetShader("sprite").Use();
    //ResourceManager::GetShader("sprite").SetInt("sImage", 0);

    Shader s = ResourceManager::GetShader("sprite");
    cRenderer.Create(s);

    ResourceManager::LoadTexture("../../res/Texture/awesomeface.png", true, "debug");
    ResourceManager::LoadTexture("../../res/Texture/TX Tileset Grass.png", true, "grass");
    ResourceManager::LoadTexture("../../res/asset_packs/simple_space_station_tileset/TileSet v1.0.png", true, "spaceship_sheet");
    ResourceManager::LoadTexture("../../res/asset_packs/Pixel Crawler - FREE - 1.8/Heroes/Rogue/Idle/Idle-Sheet.png", true, "rogue_idle_sheet");
    ResourceManager::LoadTexture("../../res/asset_packs/Pixel Crawler - FREE - 1.8/Heroes/Rogue/Run/Run-Sheet.png", true, "rogue_run_sheet");

    // TODO - Figure out "cannot bind non-const lvalue reference of type 'Shader&' to an rvalue of type 'Shader'" error

    //pRenderer = std::make_shared<SpriteRenderer>(ResourceManager::GetShader("sprite"));
    //pRenderer->Create(ResourceManager::GetShader("sprite"));

}



void Game::PrintVar(int32_t var)
{
    std::cout << var << std::endl;
}



/*********************************/
/*                               */
/*        L:Process Input        */
/*                               */
/*********************************/
void Game::ProcessInput()
{
    if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
        ProcessMouseInput();
}



void Game::ProcessMouseInput()
{
    glm::vec2 vPos = GetCursorPos();
    if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
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
}




/*********************************/
/*                               */
/*        L:Console Debug        */
/*                               */
/*********************************/
void Game::DebugAddVec2(const char* str, glm::vec2 vVec)
{
    std::string sLine(str);
    sLine += glm::to_string(vVec);
    vecDebugMessage.push_back(sLine);
}



/*******************************/
/*                             */
/*        L:Print Debug        */
/*                             */
/*******************************/
void Game::PrintDebug()
{
//    glm::vec2 vCursorPos = GetCursorPos();
//    std::cout << "Mouse Pos - Screen: " << glm::to_string(vCursorPos) << " ---- World: "
//              << glm::to_string(cWorld.ScreenToWorld(vCursorPos)) << "\r" << std::flush;
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



void Game::KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    Game* pGame = static_cast<Game*>(glfwGetWindowUserPointer(pWindow));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, true);
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        pGame->cWorld.SetWorldOffset(glm::vec2(0.0f));
    }
}



void Game::ScrollCallback(GLFWwindow* pWindow, double xoffset, double yoffset)
{
    Game* pGame = static_cast<Game*>(glfwGetWindowUserPointer(pWindow));

    if (yoffset > 0)
        pGame->cWorld.ZoomAtScreenPos(2.0f, pGame->GetCursorPos());
    else
        pGame->cWorld.ZoomAtScreenPos(0.5f, pGame->GetCursorPos());
}



void Game::FramebufferSizeCallback(GLFWwindow* pWindow, int nWidth, int nHeight)
{
    glViewport(0, 0, nWidth, nHeight);
}
