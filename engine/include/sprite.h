#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"



class Sprite
{
    public:
        Sprite(const char* _sTexFilepath, bool _bAlphaTex,
               glm::vec2 _vSize = glm::vec2(50.0f),
               glm::vec3 _vColor = glm::vec3(1.0f),
               float _fRotation = 0.0f);

        ~Sprite();

        void Draw(SpriteRenderer &cRenderer, glm::vec2 vPos);


    public:
        Texture2D cTexture;
        glm::vec2 vSize;
        glm::vec3 vColor;
        float fRotation;
        bool bAlphaTex;
};
