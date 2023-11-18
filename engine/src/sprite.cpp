#include "sprite.h"



Sprite::Sprite(const char* _sTexFilepath,
               bool _bAlphaTex,
               glm::vec2 _vSize,
               glm::vec3 _vColor,
               float _fRotation)

    : bAlphaTex(_bAlphaTex),
      vSize(_vSize),
      vColor(_vColor),
      fRotation(_fRotation)
{
    cTexture.Create(_sTexFilepath, bAlphaTex);
}



Sprite::~Sprite()
{
}



void Sprite::Draw(SpriteRenderer &cRenderer, glm::vec2 vPos)
{
    cRenderer.DrawSprite(cTexture, vPos, vSize, fRotation, vColor);
}
