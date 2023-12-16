#include <memory>

#include <glm/glm.hpp>

#include "sprite_renderer.h"
#include "sprite.h"



class Entity
{
    public:
        Entity(glm::vec2 _vWorldPos);
        ~Entity();

        void Create();
        void Draw(SpriteRenderer &cRenderer);


    public:
        glm::vec2 vWorldPos;
};
