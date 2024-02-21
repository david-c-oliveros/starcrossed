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
    if ((glm::ivec2)vWorldPos != m_vMoveGoal)
        MoveTowardGoal();
    else
        SetMoveGoal((glm::ivec2)glm::diskRand(20.0));

    Character::Update();
}



glm::ivec2 CrewMember::GetMoveGoal()
{
    return m_vMoveGoal;
}



void CrewMember::SetMoveGoal(glm::ivec2 vGoal)
{
    m_vMoveGoal = vGoal;
}



void CrewMember::MoveTowardGoal()
{
    vWorldPos += calcMoveVec() * m_fMoveSpeedScalar;
}



glm::vec2 CrewMember::calcMoveVec()
{
    return glm::normalize((glm::vec2)m_vMoveGoal - vWorldPos);
}
