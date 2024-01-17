#pragma once

#include <string>
#include <map>
#include <array>

#include "global_enums.h"
#include "world.h"
#include "entity.h"
#include "animated_sprite.h"



enum class CharacterState
{
    IDLE,
    WALK
};



class Character : public Entity
{
    public:
        Character(glm::vec2 _vWorldPos);
        ~Character();

        void Draw(SpriteRenderer &cRenderer, World &cWorld);
        void AddAnimatedSprite(std::string sTexName, std::string sSpriteName);
        void ConfigAnimatedSprite(std::string sSpriteName, uint32_t nNumFrames, uint32_t nTicksPerFrame,
                                  glm::vec2 vStart, glm::vec2 vFrameSize, glm::vec2 vFrameOffset,
                                  glm::vec2 vSpriteScalar = glm::vec2(1.0f));
        void SetState(CharacterState _eState);
        void SetDir(Direction _eDir);
        void StartSpriteAnim();
        void Update();
        void Move(Direction _eDir);
        void SetMoveSpeedScalar(float _fSpeedScalar);


    public:
        std::map<std::string, AnimatedSprite> mSprites;
        std::vector<std::string> vecDebugInfo;

        Direction eCurDir = Direction::FORWARD;
        CharacterState eCurState = CharacterState::IDLE;
        std::array<glm::vec2, 4> aDirs;
        std::array<std::string, 4> aDirStrings;
        std::array<std::string, 4> aStateStrings;


    private:
        void setCurrentSpriteName();


    private:
        std::string m_sCurSpriteName;
        float m_fMoveSpeedScalar;
};
