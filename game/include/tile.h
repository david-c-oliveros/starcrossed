#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "game_defines.h"
#include "sprite_renderer.h"
#include "sprite.h"



class Tile
{
    public:
        Tile(const char* _sTexFilepath, glm::ivec2 _vCoords, glm::vec2 _vSizeScalar);
        ~Tile();

        void Create();
        void Draw(SpriteRenderer &cRenderer);


    public:
        std::unique_ptr<Sprite> cSprite;
        glm::vec2 vCoords;
        glm::vec2 vSizeScalar;
};
