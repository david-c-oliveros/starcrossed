#include "player.h"



Player::Player(glm::vec2 _vWorldPos)
    : Character(_vWorldPos)
{
    vecDebugInfo.push_back("Player");
    for (int32_t i = 0; i < 8; i++)
        vecDebugInfo.push_back("");
}



Player::~Player()
{
}
