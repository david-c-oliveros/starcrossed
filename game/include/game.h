#include <iostream>
#include <memory>

#include <glm/gtx/string_cast.hpp>


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
        bool Update();
        void RenderGame();
        void SetDeltaTime();
        void LoadResources();
        void InitDebugTri();
        void ProcessInput();
        void DebugAddVec2(const char* str, glm::vec2 vVec);
        void PrintDebug();

        glm::vec2 GetCursorPos();

        void GLConfig();

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
};
