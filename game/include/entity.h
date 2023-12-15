#include <memory>

#include <glm/glm.hpp>

#include "sprite_renderer.h"
#include "sprite.h"



class Entity
{
    public:
        Entity(std::string _sTexName, glm::ivec2 _vCoords);
        ~Entity();

        void Create();
        void Draw(SpriteRenderer &cRenderer);


    public:
        std::unique_ptr<Sprite> pSprite;
        glm::vec2 vCoords;
};
