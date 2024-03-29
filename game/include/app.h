#pragma once

#include <iostream>
#include <memory>

#include <glm/gtx/string_cast.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "engine_defines.h"

#include "resource_manager.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "sprite_renderer.h"
#include "timer.h"

#include "global_enums.h"
#include "tile_world.h"
#include "ship.h"
#include "character.h"
#include "player.h"
#include "crew_member.h"
#include "event.h"

#include "ui.h"
#include "interactable.h"



int32_t randInRange(int32_t nMin, int32_t nMax);



class App
{
    public:
        App(uint32_t _nCanvasWidth, uint32_t _nCanvasHeight);
        ~App();

        bool Create(const char* str);
        void Destroy();
        bool Update();
        void UpdateCursorTile();

        void RenderApp();
        void RenderUI();

        void ActivateEvent();

        void SetDeltaTime();
        void SetGameState(GameState _eState);
        void LoadResources();
        bool ConfigEntities();

        void ProcessInput();
        void ProcessMouseInput();

        glm::vec2 GetCursorScreenPos();
        glm::vec2 GetCursorWorldPos();

        bool GLInit();
        void GLConfig();
        void UIConfig();

        static void MouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods);
        static void KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
        static void ScrollCallback(GLFWwindow* pWindow, double xoffset, double yoffset);
        static void FramebufferSizeCallback(GLFWwindow* pWindow, int nWidth, int nHeight);


    public:
        uint32_t nCanvasWidth;
        uint32_t nCanvasHeight;
        GLFWwindow* pWindow;

        SpriteRenderer cRenderer;
        TileWorld cTileWorld;

        glm::mat4 projection;
        glm::mat4 view;

        bool bPanning = false;
        bool bPlayerIdle = true;
        int32_t pTmpSize;

        GameState eGameState = GameState::PLAY;

        bool bShowDemoWindow = false;
        bool bShowDebugPanel = false;
        bool bShowInfoPanel  = true;
        bool bFullscreen = false;

        std::unique_ptr<Player> pPlayer;
        std::unique_ptr<CrewMember> pNPC;
        Ship cShip;

    private:
        Interactable cI;
        const char* glsl_version = "#version 330";

        std::vector<Event> m_vecAllEvents;

        ImGuiIO io;
        ImGuiStyle style;
        ImVec4 m_vUIClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        UI m_cUI;

        /****************************/
        /*        Map Editor        */
        /****************************/
        bool m_bCtrlHeld = false;
        bool m_bShiftHeld = false;
        bool m_bErase = false;
        uint32_t nCurFileNum = 0;

        std::shared_ptr<Room> m_pSelectedRoom;
        glm::ivec2 m_vNextRoomSize = glm::ivec2(8);

        glm::vec2 m_vCursorMoveDelta = glm::vec2(0.0f);
        glm::ivec2 m_vCursorTile;
        Sprite cCursorTileSprite;


        std::unique_ptr<Sprite> pBGSprite;

        std::vector<std::string> m_vecThisDebugInfo;
        std::vector<std::vector<std::string>*> m_vecAllDebugInfo;
};
