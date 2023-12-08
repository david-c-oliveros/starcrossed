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

void FramebufferSizeCallback(GLFWwindow* pWindow, int nWidth, int nHeight)
{
    glViewport(0, 0, nWidth, nHeight);
}



void KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, true);
    }
}



Game::Game(uint32_t _nCanvasWidth, uint32_t _nCanvasHeight)
    : nCanvasWidth(_nCanvasWidth), nCanvasHeight(_nCanvasHeight)
{
}



Game::~Game()
{
//    vWorldOffset = glm::vec2(0.0f);
//    vWorldScale  = glm::vec2(1.0f);
//    vStartPan    = glm::vec2(0.0f);
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
    vCameraPos = glm::vec2(0, 0);
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
    glfwSetKeyCallback(pWindow, KeyCallback);
    glfwSetCursorPosCallback(pWindow, MouseCallback);
//    glfwSetScrollCallback(pWindow, ScrollCallback);
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

    glm::vec3 vTransform = glm::vec3(vCameraPos - vWorldOffset, 1.0f);
    DebugAddVec2("Camera position: ", vCameraPos);
    view = glm::lookAt(vTransform, glm::vec3(vTransform.x, vTransform.y, 0), glm::vec3(0, 1, 0));

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

void Game::StartPan(glm::ivec2& ivPos)
{
    bPanning = true;
    vStartPan = glm::vec2(ivPos);
}

void Game::UpdatePan(const glm::ivec2& ivPos)
{
    if (bPanning)
    {
        vWorldOffset -= (glm::vec2(ivPos) - vStartPan) / vWorldScale;
        vStartPan = glm::vec2(ivPos);
    }
}

void Game::EndPan(const glm::ivec2& ivPos)
{
    UpdatePan(ivPos);
    bPanning = false;
}

glm::ivec2 Game::WorldToScreen(const glm::vec2& vWorldPos) const
{
    glm::vec2 vFloat = ((vWorldPos - vWorldOffset) * vWorldScale);
    vFloat = glm::vec2(std::floor(vFloat.x), std::floor(vFloat.y));

    return vFloat;
}

glm::vec2 Game::ScreenToWorld(const glm::ivec2& ivScreenPos)
{
    return (glm::vec2(ivScreenPos) / vWorldScale);
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
        glm::dvec2 dvPos;

        glfwGetCursorPos(pWindow, &dvPos.x, &dvPos.y);
        glm::ivec2 ivPos = (glm::ivec2)dvPos;
        DebugAddVec2("Cursor position: ", glm::vec2(ivPos));
        StartPan(ivPos);
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
    for (auto sLine : vecDebugMessage)
    {
        std::cout << sLine << "\r";
        //std::cout << sLine << "\n";
    }
    std::cout << std::flush;
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
void MouseCallback(GLFWwindow* window, double fPosInX, double fPosInY)
{
    float fPosX = static_cast<float>(fPosInX);
    float fPosY = static_cast<float>(fPosInY);

    vOffset.x = fPosX - vLastCursorPos.x;
    vOffset.y = fPosY - vLastCursorPos.y;

    vLastCursorPos.x = fPosX;
    vLastCursorPos.y = fPosY;
}
