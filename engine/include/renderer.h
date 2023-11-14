#pragma once

#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <texture.h>



class Renderer
{
    public:
        static void Clear(glm::vec4 vClearColor);
        static void Draw(GLuint &texID, GLuint &vao, GLuint shader, GLuint nNumVert);

        static bool Init_GLFW(GLFWwindow** pWindow, uint32_t nWidth, uint32_t nHeight);
};
