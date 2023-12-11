#include "sprite.h"



Sprite::Sprite(const char* _sTexFilepath,
               bool _bAlphaTex,
               glm::vec2 _vSizeScalar,
               glm::vec3 _vColor,
               float _fRotation)

    : bAlphaTex(_bAlphaTex),
      vSizeScalar(_vSizeScalar),
      vColor(_vColor),
      fRotation(_fRotation)
{
    cTexture.Create(_sTexFilepath, bAlphaTex);
}



Sprite::~Sprite()
{
}



void Sprite::Draw(SpriteRenderer &cRenderer, glm::vec2 vPos, glm::vec2 vScale)
{
    cRenderer.DrawSprite(cTexture, vPos, vScale * BASE_TILE_SIZE, fRotation, vColor);
}
