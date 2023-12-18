#include "character.h"



Character::Character(glm::vec2 _vWorldPos)
    : Entity(_vWorldPos)
{
    vecDebugInfo = { "Character", "" };
}



Character::~Character()
{
}



void Character::Draw(SpriteRenderer &cRenderer, World &cWorld)
{
    // TODO - Debug sprite rendering issue
    glm::vec2 vScreenPos = cWorld.WorldToScreen(vWorldPos);
    mSprites.find(m_sCurSprite)->second.Draw(cRenderer, vScreenPos, cWorld.GetWorldScale());
}



void Character::AddAnimatedSprite(std::string sTexName, std::string sSpriteName)
{
    m_sCurSprite = sSpriteName;
    mSprites.emplace(sSpriteName, sTexName);
}



void Character::ConfigAnimatedSprite(std::string sSpriteName, uint32_t nNumFrames, uint32_t nTicksPerFrame, glm::vec2 vStart,
                                     glm::vec2 vFrameSize, glm::vec2 vFrameOffset, glm::vec2 vSpriteScalar)
{
    mSprites.find(sSpriteName)->second.CreateFrames(nNumFrames, nTicksPerFrame, vStart, vFrameSize, vFrameOffset, vSpriteScalar);
}



void Character::SetCurrentSprite(std::string sSpriteName)
{
    // TODO (maybe) - stop old animation
    m_sCurSprite = sSpriteName;
}



void Character::StartSpriteAnim()
{
    mSprites.find(m_sCurSprite)->second.StartAnim();
}



void Character::Update()
{
    mSprites.find(m_sCurSprite)->second.Update();
}
