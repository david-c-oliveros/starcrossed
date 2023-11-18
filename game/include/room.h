#include <vector>

#include "sprite_renderer.h"

#include "tile.h"



class Room
{
    public:
        Room(uint32_t nrTiles, glm::ivec2 _vUpperLeft);
        ~Room();

        void Draw(SpriteRenderer &cRenderer);


    public:
        std::vector<std::unique_ptr<Tile>> vecTiles;
        glm::ivec2 vUpperLeft;
};
