#include "animated_sprite.h"



AnimatedSprite::AnimatedSprite(std::string _sTexName)
    : Sprite(_sTexName)
{
}



AnimatedSprite::~AnimatedSprite()
{
}



void AnimatedSprite::CreateFrames(uint32_t _nNumFrames, uint32_t nTicksPerFrame,
                                  glm::vec2 _vStart, glm::vec2 _vFrameSize, glm::vec2 _vFrameOffset, glm::vec2 _vScalar)
{
    SetAnimRate(nTicksPerFrame);
    nNumFrames = _nNumFrames;

    m_vStart = _vStart;
    m_vFrameSize = _vFrameSize;
    m_vFrameOffset = _vFrameOffset;
    m_vScalar = _vScalar;
}



void AnimatedSprite::SetAnimRate(uint32_t nTicksPerFrame)
{
    cAnimTimer.SetTime(nTicksPerFrame);
}



void AnimatedSprite::StartAnim()
{
    cAnimTimer.Start();
}



void AnimatedSprite::Update()
{
    if (cAnimTimer.Check())
    {
        if (++nCurFrame >= nNumFrames)
            nCurFrame = 0;

        cAnimTimer.Reset();
        cAnimTimer.Start();
    }
    else
    {
        cAnimTimer.Update();
    }
}




void AnimatedSprite::Draw(SpriteRenderer &cRenderer,
                          glm::vec2 vPos, glm::vec2 vScale)
{
    vScale *= BASE_TILE_SIZE;
    glm::vec2 vCurOffset = m_vStart + glm::vec2(m_vFrameOffset.x * nCurFrame, 0.0f);

    Texture2D t = ResourceManager::GetTexture(sTexName);
    cRenderer.DrawSprite(t, vPos, m_vFrameSize, vCurOffset, vScale * m_vScalar, fRotation, vColor);
}
