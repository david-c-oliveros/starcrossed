#include <iostream>
#include <memory>


class Game
{
    public:
        Game(uint32_t _nCanvasWidth, uint32_t _nCanvasHeight);
        ~Game();

        bool Create();
        bool Update();


    public:
        uint32_t nCanvasWidth;
        uint32_t nCanvasHeight;
};
