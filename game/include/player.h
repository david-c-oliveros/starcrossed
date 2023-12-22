#pragma once



class Player : public Entity
{
    public:
        Player(glm::vec2 _vWorldPos);
        ~Player();


    public:
        std::vector<std::string> vecDebugInfo;
};
