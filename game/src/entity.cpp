#include "entity.h"



Entity::Entity(const char* _sTexFilepath, glm::ivec2 _vCoords, glm::vec2 _vSizeScalar)
    : vCoords(_vCoords), vSizeScalar(_vSizeScalar)
{
    cSprite = std::make_unique<Sprite>(_sTexFilepath, true, vSizeScalar, glm::vec3(1.0f), 0.0f);
}
