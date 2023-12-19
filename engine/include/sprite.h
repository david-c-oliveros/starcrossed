#pragma once

#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "resource_manager.h"
#include "game_defines.h"
#include "texture.h"
#include "sprite_renderer.h"



class Sprite
{
    public:
        Sprite();
        Sprite(std::string _sTexName);
        ~Sprite();

        void SetColor(glm::vec3 _vColor);

        virtual void Draw(SpriteRenderer &cRenderer,
                          glm::vec2 vPos,
                          glm::vec2 vScale,
                          glm::vec2 vSizeInSheet = glm::vec2(1.0f),
                          glm::vec2 vOffset = glm::vec2(0.0f));

        virtual void DrawColored(SpriteRenderer &cRenderer,
                          glm::vec2 vPos,
                          glm::vec2 vScale);

    public:
        std::string sTexName;

        glm::vec3 vColor;
        float fRotation;
        bool m_bIsSpriteSheet;
};
