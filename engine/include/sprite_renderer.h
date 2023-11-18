#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <texture.h>
#include <shader.h>



class SpriteRenderer
{
    public:
        SpriteRenderer();
        ~SpriteRenderer();

        void Create(Shader &shader);
        void DrawSprite(Texture2D &texture,
                        glm::vec2 position,
                        glm::vec2 size = glm::vec2(10.0f, 10.0f),
                        float rotate = 0.0f,
                        glm::vec3 color = glm::vec3(1.0f));


    private:
        Shader cShader;
        uint32_t nQuadVAO;

        void initRenderData();
};
