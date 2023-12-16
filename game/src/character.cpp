#include "character.h"



Character::Character(glm::vec2 _vWorldPos)
    : Entity(_vWorldPos)
{
}



Character::~Character()
{
}



void Character::Draw(SpriteRenderer &cRenderer, World &cWorld)
{
    glm::vec2 vScreenPos = cWorld.WorldToScreen(vWorldPos);
    mSprites.find("idle")->second.Draw(cRenderer, vScreenPos, cWorld.GetWorldScale(), glm::vec2(1.0f), glm::vec2(0.0f));
}



void Character::AddSpriteSheet(std::string sTexName, std::string sSpriteName)
{
    mSprites.emplace(sSpriteName, sTexName);
}
