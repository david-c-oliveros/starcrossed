#include "character.h"



Character::Character(glm::vec2 _vWorldPos)
    : Entity(_vWorldPos)
{
    vecDebugInfo.push_back("Character");
    for (int32_t i = 0; i < 8; i++)
        vecDebugInfo.push_back("");

    aDirs = { glm::ivec2(0, -1), glm::ivec2(0, 1), glm::ivec2(-1, 0), glm::ivec2(1, 0) };
    aDirStrings = { "forward", "backward", "left", "right" };
    aStateStrings = { "idle", "walk" };
}



Character::~Character()
{
}



void Character::Draw(SpriteRenderer &cRenderer, TileWorld &cTileWorld)
{
    vecDebugInfo[2] = "vWorldPos: " + glm::to_string(vWorldPos);
    glm::vec2 vAbsolutePos(vWorldPos * BASE_TILE_SIZE);
    glm::vec2 vScreenPos = cTileWorld.WorldToScreen(vAbsolutePos);
    mSprites.find(m_sCurSpriteName)->second.Draw(cRenderer, vScreenPos, cTileWorld.GetWorldScale());
}



void Character::AddAnimatedSprite(std::string sTexName, std::string sSpriteName)
{
    mSprites.emplace(sSpriteName, sTexName);
}



void Character::ConfigAnimatedSprite(std::string sSpriteName, uint32_t nNumFrames, uint32_t nTicksPerFrame, glm::vec2 vStart,
                                     glm::vec2 vFrameSize, glm::vec2 vFrameOffset, glm::vec2 vSpriteScalar)
{
    mSprites.find(sSpriteName)->second.CreateFrames(nNumFrames, nTicksPerFrame, vStart, vFrameSize, vFrameOffset, vSpriteScalar);
}



void Character::SetState(CharacterState _eState)
{
    // TODO (maybe) - stop old animation
    eCurState = _eState;
}



void Character::SetDir(Direction _eDir)
{
    eCurDir = _eDir;
}



void Character::StartSpriteAnim()
{
    for (std::map<std::string, AnimatedSprite>::iterator iter = mSprites.begin(); iter != mSprites.end(); ++iter)
    {
        iter->second.StartAnim();
    }
}



void Character::Update()
{
    setCurrentSpriteName();
    mSprites.find(m_sCurSpriteName)->second.Update();
}



void Character::Move(Direction _eDir)
{
    vWorldPos += aDirs[(int32_t)_eDir] * m_fMoveSpeedScalar;
    SetDir(_eDir);
    SetState(CharacterState::WALK);
}



void Character::SetMoveSpeedScalar(float _fSpeedScalar)
{
    m_fMoveSpeedScalar = _fSpeedScalar;
}



void Character::setCurrentSpriteName()
{
    m_sCurSpriteName = aStateStrings[(int32_t)eCurState] + "_" + aDirStrings[(int32_t)eCurDir];
}
