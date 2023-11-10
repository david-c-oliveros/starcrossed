#include "game.h"



Game::Game(uint32_t _nCanvasWidth, uint32_t _nCanvasHeight)
    : nCanvasWidth(_nCanvasWidth), nCanvasHeight(_nCanvasHeight)
{
}



Game::~Game()
{
}



bool Game::Create()
{
    std::cout << "Creating game\n";

    return true;
}



bool Game::Update()
{
    std::cout << "Updating\n";

    return true;
}
