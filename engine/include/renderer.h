#pragma once

#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <texture.h>
#include <shader.h>



class Renderer
{
    public:
        static void Clear(glm::vec4 vClearColor);
        static void Draw(GLuint &vao, Shader shader, GLuint nNumVert);
        static void InitTri();
        static void DrawTri(Shader shader, uint32_t vao);

        static bool Init_GLFW(GLFWwindow** pWindow, uint32_t nWidth, uint32_t nHeight);
};
