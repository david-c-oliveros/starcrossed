#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>



class Renderer
{
    public:
        static void Clear(glm::vec4 vClearColor);
        static void Draw(GLuint &vao, GLuint &vbo, GLuint shader, GLuint nNumVert);

        static void Init_GLFW(GLFWwindow** pWindow, uint32_t nWidth, uint32_t nHeight);
};
