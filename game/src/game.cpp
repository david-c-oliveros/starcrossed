#include "game.h"



void FramebufferSizeCallback(GLFWwindow* pWindow, int nWidth, int nHeight)
{
}



void KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
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



bool Game::Update()
{
    return true;
}
