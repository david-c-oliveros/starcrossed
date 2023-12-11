#pragma once

#include <iostream>
#include <memory>

#include <glm/gtx/string_cast.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "engine_defines.h"

#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "sprite_renderer.h"

#include "world.h"



class Game
{
    public:
        Game(uint32_t _nCanvasWidth, uint32_t _nCanvasHeight);
        ~Game();

        bool Create();
        void Destroy();
        bool Update();
        void Shutdown();
        void RenderGame();
        void RenderUI();
        void SetDeltaTime();
        void LoadResources();
        void InitDebugTri();
        void ProcessInput();
        void DebugAddVec2(const char* str, glm::vec2 vVec);
        void PrintDebug();

        glm::vec2 GetCursorPos();

        void GLConfig();
        void UIConfig();

        static void ScrollCallback(GLFWwindow* pWindow, double xoffset, double yoffset);
        static void KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
        static void FramebufferSizeCallback(GLFWwindow* pWindow, int nWidth, int nHeight);


    public:
        uint32_t nCanvasWidth;
        uint32_t nCanvasHeight;
        GLFWwindow* pWindow;

        Shader cShader;
        Shader cSimpleShader;
        Texture2D cSpriteTex;
        SpriteRenderer cRenderer;

        glm::mat4 projection;
        glm::mat4 view;

        std::unique_ptr<World> pWorld;

        glm::vec3 vCameraPos;

        bool bPanning = false;


    private:
        std::vector<std::string> vecDebugMessage;
        const char* glsl_version = "#version 330";
        ImVec4 m_vUIClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        bool m_bShowUIWindow;
};
