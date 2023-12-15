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
        Sprite(std::string _sTexName,
               glm::vec3 _vColor = glm::vec3(1.0f),
               float _fRotation = 0.0f);

        ~Sprite();

        void Draw(std::shared_ptr<SpriteRenderer> pRenderer, glm::vec2 vPos, glm::vec2 vScale);


    public:
        std::string sTexName;
        glm::vec3 vColor;
        float fRotation;
};
