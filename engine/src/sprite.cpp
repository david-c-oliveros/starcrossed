#include "sprite.h"



Sprite::Sprite(std::string _sTexName)
    : sTexName(_sTexName), vColor(glm::vec3(1.0f)), fRotation(0.0f)
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
