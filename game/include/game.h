#include <iostream>
#include <memory>

#include <glm/gtx/string_cast.hpp>

#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "sprite_renderer.h"

#include "world.h"



void FramebufferSizeCallback(GLFWwindow* pWindow, int nWidth, int nHeight);
void KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods);


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

        void GLConfig();


    public:
        uint32_t nCanvasWidth;
        uint32_t nCanvasHeight;
        GLFWwindow* pWindow;

        Shader cShader;
        Shader cSimpleShader;
        Texture2D cSpriteTex;
        SpriteRenderer cRenderer;

        glm::mat4 projection;

        World cWorld;
};
