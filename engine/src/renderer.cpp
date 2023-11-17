#include "renderer.h"



void Renderer::Clear(glm::vec4 vClearColor)
{
    glClearColor(vClearColor.x, vClearColor.y, vClearColor.z, vClearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



void Renderer::Draw(GLuint &vao, Shader shader, GLuint nNumVert)
{
    shader.Use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, nNumVert);
    glBindVertexArray(0);
}



bool Renderer::Init_GLFW(GLFWwindow** pWindow, uint32_t nWidth, uint32_t nHeight)
{
    std::cout << "Init GLFW\n";

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Temporary

    *pWindow = glfwCreateWindow(nWidth, nHeight, "Star Crossed", NULL, NULL);

    if (pWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(*pWindow);

    std::cout << "Initializing window at: " << *pWindow << std::endl;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    return true;
}
