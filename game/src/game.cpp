#include "game.h"



/************************************/
/*                                  */
/*        L:Global Variables        */
/*                                  */
/************************************/
float fDeltaTime = 0.0f;
float fLastFrame = 0.0f;



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
//    glfwSetCursorPosCallback(pWindow, MouseCallback);
//    glfwSetScrollCallback(pWindow, ScrollCallback);

    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetKeyCallback(pWindow, KeyCallback);
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

    glfwSwapBuffers(pWindow);
    glfwPollEvents();

    return true;
}



void Game::RenderGame()
{
    Renderer::Clear(glm::vec4(0.0f, 0.0f, 0.21f, 1.0f));

    projection = glm::ortho<float>(0.0f, static_cast<float>(nCanvasWidth),
                                         static_cast<float>(nCanvasHeight), 0.0f, -1.0f, 1.0f);

    cShader.SetMat4("projection", projection);


    cRenderer.DrawSprite(cSpriteTex,
                         glm::vec2(50.0f, 50.0f),
                         glm::vec2(100.0f),
                         0.0f,
                         glm::vec3(1.0f, 1.0f, 1.0f));

    cWorld.Draw(cRenderer);
}



void Game::SetDeltaTime()
{
    float fCurFrame = static_cast<float>(glfwGetTime());
    fDeltaTime = fCurFrame - fLastFrame;
    fLastFrame = fCurFrame;
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
