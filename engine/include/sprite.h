#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "game_defines.h"
#include "texture.h"
#include "sprite_renderer.h"



class Sprite
{
    public:
        Sprite(const char* _sTexFilepath, bool _bAlphaTex,
               glm::vec2 _vSizeScalar = glm::vec2(50.0f),
               glm::vec3 _vColor = glm::vec3(1.0f),
               float _fRotation = 0.0f);

        ~Sprite();

        void Draw(SpriteRenderer &cRenderer, glm::vec2 vPos);


    public:
        Texture2D cTexture;
        bool bAlphaTex;
        glm::vec2 vSizeScalar;
        glm::vec3 vColor;
        float fRotation;
};
