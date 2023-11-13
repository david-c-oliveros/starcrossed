#include <iostream>
#include <memory>

#include <renderer.h>



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

        void GLFWConfig();


    public:
        uint32_t nCanvasWidth;
        uint32_t nCanvasHeight;
        GLFWwindow* pWindow;
};
