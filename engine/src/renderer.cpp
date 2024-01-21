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



void Renderer::EnterFullscreen(GLFWwindow* pWindow)
{
    GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* pMode = glfwGetVideoMode(pMonitor);

    glfwSetWindowMonitor(pWindow, glfwGetPrimaryMonitor(), 0, 0, pMode->width, pMode->height, pMode->refreshRate);
    std::cout << "Monitor width: " << pMode->width << std::endl;
    std::cout << "Monitor height: " << pMode->height << std::endl;
}



void Renderer::ExitFullscreen(GLFWwindow* pWindow, glm::ivec2 vPos, glm::ivec2 vSize)
{
    glfwSetWindowMonitor(pWindow, NULL, vPos.x, vPos.y, vSize.x, vSize.y, 0);
}



glm::vec2 Renderer::GetScreenSize()
{
    GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* pMode = glfwGetVideoMode(pMonitor);

    return glm::vec2(pMode->width, pMode->height);
}



glm::vec2 Renderer::GetMonitorContentScale()
{
    GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();

    float fScaleX, fScaleY;
    glfwGetMonitorContentScale(pMonitor, &fScaleX, &fScaleY);

    return glm::vec2(fScaleX, fScaleY);
}



void Renderer::CloseWindow(GLFWwindow* pWindow)
{
    glfwSetWindowShouldClose(pWindow, true);
}



bool Renderer::Init_GLFW(GLFWwindow** pWindow, uint32_t nWidth, uint32_t nHeight, bool bFullscreen)
{
    std::cout << "Init GLFW\n";

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Temporary

    if (bFullscreen)
    {
        GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();

        int32_t nWidth_mm, nHeight_mm;
        glfwGetMonitorPhysicalSize(pMonitor, &nWidth_mm, &nHeight_mm);

        float fScaleX, fScaleY;
        glfwGetMonitorContentScale(pMonitor, &fScaleX, &fScaleY);

        std::cout << "Monitor width: " << nWidth_mm << std::endl;
        std::cout << "Monitor height: " << nHeight_mm << std::endl;

        std::cout << "Monitor scaleX: " << fScaleX << std::endl;
        std::cout << "Monitor scaleY: " << fScaleY << std::endl;

        *pWindow = glfwCreateWindow(nWidth * fScaleX, nHeight * fScaleY, "Star Crossed", pMonitor, NULL);
    }
    else
    {
        *pWindow = glfwCreateWindow(nWidth, nHeight, "Star Crossed", NULL, NULL);
    }

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
