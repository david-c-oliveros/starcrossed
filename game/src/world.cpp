#include "world.h"



World::World()
{
}



bool World::Create()
{
//    for (int i = 0; i < 2; i++)
//    {
//        vecRooms.push_back(std::make_unique<Room>(4, glm::ivec2(i)));
//    }

    pTestTile = std::make_unique<Tile>(glm::ivec2(200), "../../res/Texture/awesomeface.png");

    return true;
}



World::~World()
{
}



void World::Draw(SpriteRenderer &cRenderer)
{
    pTestTile->Draw(cRenderer);
//    for (auto & r : vecRooms)
//    {
//        r->Draw(cRenderer);
//    }
}
