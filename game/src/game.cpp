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
    vWorldOffset = glm::vec2(0.0f);
    vWorldScale  = glm::vec2(8.0f);
    vStartPan    = glm::vec2(0.0f);
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

    LoadResources();
    vCameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
    cWorld.Create();

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
    ProcessInput();
    PrintDebug();

    glfwSwapBuffers(pWindow);
    glfwPollEvents();

    return true;
}



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

    view = glm::lookAt(vCameraPos, glm::vec3(vWorldOffset, 0.0f), glm::vec3(0, 1, 0));

    cShader.SetMat4("projection", projection);
    cShader.SetMat4("view", view);

    cWorld.Draw(cRenderer);
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
void Game::StartPan()
{
    glm::vec2 vPos = GetCursorPos();
    bPanning = true;
    vStartPan = vPos;
}



void Game::UpdatePan()
{
    glm::vec2 vPos = GetCursorPos();

    vWorldOffset -= (vPos - vStartPan) / vWorldScale;
    vStartPan = vPos;
}



void Game::EndPan()
{
    UpdatePan();
    bPanning = false;
}



void Game::ZoomAtScreenPos(const float fDeltaZoom, const glm::ivec2& ivPos)
{
    glm::vec2 vOffsetBeforeZoom = ScreenToWorld(ivPos);
    vWorldScale *= fDeltaZoom;
    glm::vec2 vOffsetAfterZoom = ScreenToWorld(ivPos);
}

void Game::SetZoom(const float fZoom, const glm::ivec2& ivPos)
{
    glm::vec2 vOffsetBeforeZoom = ScreenToWorld(ivPos);
    vWorldScale = glm::vec2(fZoom);
    glm::vec2 vOffsetAfterZoom = ScreenToWorld(ivPos);
    vWorldOffset += vOffsetBeforeZoom - vOffsetAfterZoom;
}

glm::ivec2 Game::WorldToScreen(const glm::vec2& vWorldPos) const
{
    glm::vec2 vFloatScreenPos = ((vWorldPos - vWorldOffset) * vWorldScale);
    vFloatScreenPos = glm::vec2(std::floor(vFloatScreenPos.x), std::floor(vFloatScreenPos.y));

    return (glm::vec2)vFloatScreenPos;
}

glm::vec2 Game::ScreenToWorld(const glm::ivec2& ivScreenPos)
{
    return (GetCursorPos() / vWorldScale) + vWorldOffset;
}



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
    if (glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (bPanning)
            UpdatePan();
        else
            StartPan();

    }
    else if (bPanning)
    {
        EndPan();
    }
    vCameraPos = glm::vec3(vWorldOffset, vWorldScale.x);
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
    for (auto sLine : vecDebugMessage)
    {
        std::cout << sLine << "\n";
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



void Game::KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    Game* pGame = static_cast<Game*>(glfwGetWindowUserPointer(pWindow));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, true);
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        std::cout << "Camera position: " << glm::to_string(pGame->vCameraPos) << std::endl;
    }
}



void Game::ScrollCallback(GLFWwindow* pWindow, double xoffset, double yoffset)
{
    Game* pGame = static_cast<Game*>(glfwGetWindowUserPointer(pWindow));

    if (yoffset > 0)
        pGame->ZoomAtScreenPos(2.0f, pGame->GetCursorPos());
    else
        pGame->ZoomAtScreenPos(0.5f, pGame->GetCursorPos());
}



void Game::FramebufferSizeCallback(GLFWwindow* pWindow, int nWidth, int nHeight)
{
    glViewport(0, 0, nWidth, nHeight);
}
