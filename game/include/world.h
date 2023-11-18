#include <vector>

#include "sprite_renderer.h"

#include "room.h"



class World
{
    public:
        World();
        ~World();

        bool Create();
        void Draw(SpriteRenderer &cRenderer);


    public:
        std::vector<std::unique_ptr<Room>> vecRooms;

        std::unique_ptr<Tile> pTestTile;
};
