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
        Tile(const char* _sTexFilepath, glm::ivec2 _vCoords, glm::vec2 _vSize);
        ~Tile();

        void Create();
        void Draw(SpriteRenderer &cRenderer);


    public:
        std::unique_ptr<Sprite> cSprite;
        glm::vec2 vCoords;
        glm::vec2 vSize;
};
