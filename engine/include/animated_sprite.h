#pragma once

#include "game_defines.h"
#include "sprite.h"
#include "timer.h"



class AnimatedSprite : public Sprite
{
    public:
        AnimatedSprite(std::string _sTexName);
        ~AnimatedSprite();

        void CreateFrames(uint32_t _nNumFrames, uint32_t nTicksPerFrame,
                          glm::vec2 _vStart, glm::vec2 _vFrameSizeInSheet, glm::vec2 _vFrameOffset,
                          glm::vec2 _vScalar = glm::vec2(1.0f));

        glm::vec2 GetPosOffset();
        void SetPosOffset(glm::vec2 vPosOffset);
        void SetAnimRate(uint32_t nTicksPerFrame);
        void StartAnim();
        void Update();

        void Draw(SpriteRenderer &cRenderer,
                  glm::vec2 vPos, glm::vec2 vScale);


    public:
        Timer cAnimTimer;


    private:
        glm::vec2 m_vStart;
        glm::vec2 m_vFrameSizeInSheet;
        glm::vec2 m_vFrameOffset;
        glm::vec2 m_vScalar;
        glm::vec2 m_vPosOffset;

        uint32_t nNumFrames;
        uint32_t nCurFrame = 1;
};
