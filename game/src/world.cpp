#include "world.h"



World::World()
{
}



bool World::Create()
{
    for (int i = 0; i < 4; i++)
    {
        vecRooms.push_back(std::make_unique<Room>(4, glm::ivec2(0, i)));
    }

    return true;
}



World::~World()
{
}



void World::Draw(SpriteRenderer &cRenderer)
{
    for (auto & r : vecRooms)
    {
        r->Draw(cRenderer);
    }
}
