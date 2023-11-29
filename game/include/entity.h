#include <memory>

#include <glm/glm.hpp>

#include "sprite_renderer.h"
#include "sprite.h"



class Entity
{
    public:
        Entity(const char* _sTexFilepath, glm::ivec2 _vCoords, glm::vec2 _vSizeScalar);
        ~Entity();

        void Create();
        void Draw(SpriteRenderer &cRenderer);


    public:
        std::unique_ptr<Sprite> cSprite;
        glm::vec2 vCoords;
        glm::vec2 vSizeScalar;
};
