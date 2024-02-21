#pragma once

#include <glm/gtc/random.hpp>

#include "character.h"



class CrewMember : public Character
{
    public:
        CrewMember(glm::vec2 _vWorldPos);
        ~CrewMember();

        void Update();
        glm::ivec2 GetMoveGoal();
        void SetMoveGoal(glm::ivec2 vGoal);
        void MoveTowardGoal();


    private:
        glm::vec2 calcMoveVec();


    private:
        glm::ivec2 m_vMoveGoal;
};
