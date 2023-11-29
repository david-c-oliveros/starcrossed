#include <vector>

#include "sprite_renderer.h"

#include "tile.h"



class Room
{
    public:
        Room(uint32_t nrTiles = 1,
             glm::ivec2 _vUpperLeft = glm::ivec2(0),
             glm::vec2 _vTileSizeScalar = glm::vec2(1.0f));

        ~Room();

        void Draw(SpriteRenderer &cRenderer);


    public:
        std::vector<std::unique_ptr<Tile>> vecTiles;
        glm::ivec2 vUpperLeft;
        glm::vec2 vTileSizeScalar;
};
