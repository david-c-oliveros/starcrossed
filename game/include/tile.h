#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "sprite_renderer.h"
#include "sprite.h"


#define DEFAULT_TILE_SIZE glm::vec2(100.0f)
#define DEFAULT_TILE_COLOR glm::vec3(1.0f)
#define DEFAULT_TILE_ROT 0.0f



class Tile
{
    public:
        Tile(glm::ivec2 coords, const char* _sTexFilepath);
        ~Tile();

        void Create();
        void Draw(SpriteRenderer &cRenderer);


    public:
        std::unique_ptr<Sprite> cSprite;
        glm::vec2 vCoords;
};
