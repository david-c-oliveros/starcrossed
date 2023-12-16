#include "sprite.h"



Sprite::Sprite(std::string _sTexName,
               glm::vec3 _vColor,
               float _fRotation)

    : sTexName(_sTexName),
      vColor(_vColor),
      fRotation(_fRotation)
{
}



Sprite::~Sprite()
{
}



void Sprite::Draw(SpriteRenderer &cRenderer,
                  glm::vec2 vPos, glm::vec2 vScale,
                  glm::vec2 vSizeInSheet, glm::vec2 vOffset)
{
    vScale *= BASE_TILE_SIZE;
    Texture2D t = ResourceManager::GetTexture(sTexName);
    cRenderer.DrawSprite(t, vPos, vSizeInSheet, vOffset, vScale, fRotation, vColor);
}
