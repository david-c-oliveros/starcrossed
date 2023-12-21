#pragma once

#include <iostream>
#include <memory>

#include <glm/gtx/string_cast.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "engine_defines.h"

#include "resource_manager.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "sprite_renderer.h"
#include "timer.h"

#include "global_enums.h"
#include "world.h"
#include "character.h"



class Game
{
    public:
        Game(uint32_t _nCanvasWidth, uint32_t _nCanvasHeight);
        ~Game();

        bool Create();
        void Destroy();
        bool Update();
        void UpdateCursorTile();
        void RenderGame();
        void RenderUI();
        void SetDeltaTime();
        void LoadResources();

        void ProcessInput();
        void ProcessMouseInput();

        void PrintVar(int32_t var);

        glm::vec2 GetCursorScreenPos();
        glm::vec2 GetCursorWorldPos();

        bool GLInit();
        void GLConfig();
        void UIConfig();

        static void ScrollCallback(GLFWwindow* pWindow, double xoffset, double yoffset);
        static void KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
        static void FramebufferSizeCallback(GLFWwindow* pWindow, int nWidth, int nHeight);


    public:
        uint32_t nCanvasWidth;
        uint32_t nCanvasHeight;
        GLFWwindow* pWindow;

        SpriteRenderer cRenderer;
        World cWorld;

        std::unique_ptr<Character> pPlayer;

        glm::mat4 projection;
        glm::mat4 view;

        bool bPanning = false;
        bool bPlayerIdle = true;

    private:
        std::vector<std::string> vecDebugMessage;
        const char* glsl_version = "#version 330";

        ImGuiIO io;
        ImVec4 m_vUIClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        bool m_bShowUIWindow;
        bool m_bShowDebugInfo = true;

        bool m_bShowWorld = true;
        bool m_bShowCharacter = true;

        /****************************/
        /*        Map Editor        */
        /****************************/
        bool m_bTileSelectMod = false;
        bool m_bErase = false;
        uint32_t nCurFileNum = 0;

        glm::vec2 m_vCursorMoveDelta = glm::vec2(0.0f);
        glm::ivec2 m_vCursorTile;
        Sprite cCursorTileSprite;

        std::vector<std::string> m_vecDebugInfo;
};
