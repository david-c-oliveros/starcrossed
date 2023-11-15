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

    GLFWConfig();
    LoadResources();

    return true;
}



/*******************************/
/*                             */
/*        L:GLFW Config        */
/*                             */
/*******************************/
void Game::GLFWConfig()
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

    RenderGame();

    glfwSwapBuffers(pWindow);
    glfwPollEvents();

    return true;
}



void Game::RenderGame()
{
    Renderer::Clear(glm::vec4(0.0f, 0.0f, 0.11f, 1.0f));

    cSpriteRenderer.DrawSprite(cSpriteTex, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
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
    cShader.Create("../shaders/sprite.vert", "../shaders/sprite.frag");
    cSpriteTex.Create("res/Texture/TX Player.png", false);
    cSpriteRenderer.Create(cShader);
}
