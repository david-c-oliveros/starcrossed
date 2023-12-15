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



void Sprite::Draw(std::shared_ptr<SpriteRenderer> pRenderer, glm::vec2 vPos, glm::vec2 vScale)
{
    Texture2D t = ResourceManager::GetTexture(sTexName);
    pRenderer->DrawSprite(t, vPos, vScale, fRotation, vColor);
}
