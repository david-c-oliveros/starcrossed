#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "tile_world.h"



class Interactable
{
    public:
        Interactable(glm::vec2 _vPos = glm::vec2(0.0f), glm::vec2 _vSize = glm::vec2(1.0f));
        ~Interactable();

        void Draw(SpriteRenderer &cRenderer, TileWorld &cTileWorld);

        bool IsHovered(TileWorld &cTileWorld, glm::vec2 vCursorScreenPos);


    public:
        std::vector<std::string> vecDebugInfo;

        std::unique_ptr<Sprite> cSprite;
        std::unique_ptr<Sprite> cSpriteSolid;

        glm::vec2 vPos;
        glm::vec2 vSize;
};
