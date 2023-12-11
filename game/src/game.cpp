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
}



/**************************/
/*                        */
/*        L:Create        */
/*                        */
/**************************/
bool Game::Create()
{
    if (!Renderer::Init_GLFW(&pWindow, nCanvasWidth, nCanvasHeight))
    {
        std::cout << "ERROR::GLFW" << std::endl;
        return false;
    }

    GLConfig();
    UIConfig();

    LoadResources();
    vCameraPos = glm::vec3(0.0f, 0.0f, 1.0f);

    pWorld = std::make_unique<World>();
    pWorld->Create(glm::ivec2(32, 32), glm::vec2(1.0f));

    return true;
}



/*******************************/
/*                             */
/*        L:GLFW Config        */
/*                             */
/*******************************/
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

    glViewport(0, 0, 1920, 1080);
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
    ImGuiIO& io = ImGui::GetIO(); (void)io;
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
    //RenderUI();
    RenderGame();
    ProcessInput();
    PrintDebug();

    glfwSwapBuffers(pWindow);
    glfwPollEvents();

    return true;
}



void Game::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(pWindow);
    glfwTerminate();
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

    projection = glm::ortho<float>(-(fWidth  / 2.0f),  fWidth  / 2.0f,
                                     fHeight / 2.0f, -(fHeight / 2.0f), -1000.0f, 1000.0f);

    view = glm::lookAt(vCameraPos, glm::vec3(pWorld->GetWorldOffset(), 0.0f), glm::vec3(0, 1, 0));

    cShader.SetMat4("projection", projection);
    cShader.SetMat4("view", view);

    pWorld->Draw(cRenderer);
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

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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
    cShader.Create("../../shaders/sprite.vert", "../../shaders/sprite.frag");

    cSpriteTex.Create("../../res/Texture/awesomeface.png", true);
    cRenderer.Create(cShader);

    cShader.SetInt("sImage", 0);
}



/*********************************/
/*                               */
/*        L:Process Input        */
/*                               */
/*********************************/
void Game::ProcessInput()
{
    glm::vec2 vPos = GetCursorPos();
    if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (bPanning)
        {
            pWorld->UpdatePan(vPos);
        }
        else
        {
            pWorld->StartPan(vPos);
            bPanning = true;
        }

    }
    else if (bPanning)
    {
        pWorld->EndPan(vPos);
        bPanning = false;
    }
    vCameraPos = glm::vec3(pWorld->GetWorldOffset(), pWorld->GetWorldScale().x);
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
//              << glm::to_string(pWorld->ScreenToWorld(vCursorPos)) << "\r" << std::flush;
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
        pGame->Shutdown();
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        pGame->pWorld->SetWorldOffset(glm::vec2(0.0f));
    }
}



void Game::ScrollCallback(GLFWwindow* pWindow, double xoffset, double yoffset)
{
    Game* pGame = static_cast<Game*>(glfwGetWindowUserPointer(pWindow));

    if (yoffset > 0)
        pGame->pWorld->ZoomAtScreenPos(2.0f, pGame->GetCursorPos());
    else
        pGame->pWorld->ZoomAtScreenPos(0.5f, pGame->GetCursorPos());
}



void Game::FramebufferSizeCallback(GLFWwindow* pWindow, int nWidth, int nHeight)
{
    glViewport(0, 0, nWidth, nHeight);
}
