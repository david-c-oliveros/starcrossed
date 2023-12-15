#include "entity.h"



Entity::Entity(std::string _sTexName, glm::ivec2 _vCoords)
    : vCoords(_vCoords)
{
    pSprite = std::make_unique<Sprite>(_sTexName, glm::vec3(1.0f), 0.0f);
}
