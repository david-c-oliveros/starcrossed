#include "crew_member.h"



CrewMember::CrewMember(glm::vec2 _vWorldPos)
    : Character(_vWorldPos)
{
    vecDebugInfo[0] = "CrewMember";
    for (int32_t i = 0; i < 8; i++)
        vecDebugInfo.push_back("");
}



CrewMember::~CrewMember()
{
}



void CrewMember::Update()
{
    if (glm::abs(glm::length(vWorldPos - (glm::vec2)m_vMoveGoal)) > 0.1f)
        MoveTowardGoal();
    else
        SetMoveGoal((glm::ivec2)glm::diskRand(2.0));

    Character::Update();
}



glm::ivec2 CrewMember::GetMoveGoal()
{
    return m_vMoveGoal;
}



void CrewMember::SetMoveGoal(glm::ivec2 vGoal)
{
    vecDebugInfo[1] = "m_vMoveGoal: " + glm::to_string(m_vMoveGoal);
    m_vMoveGoal = vGoal;
}



void CrewMember::MoveTowardGoal()
{
    glm::vec2 vMoveVec = calcMoveVec();
    vWorldPos += vMoveVec * m_fMoveSpeedScalar;
    SetDir(calcSpriteDir(vMoveVec));
}



glm::vec2 CrewMember::calcMoveVec()
{
    return glm::normalize((glm::vec2)m_vMoveGoal - vWorldPos);
}



Direction CrewMember::calcSpriteDir(glm::vec2 _vMoveVec)
{
    // TODO - Update for all four directions

    if (_vMoveVec.x < 0)
        return Direction::LEFT;

        return Direction::RIGHT;
}
