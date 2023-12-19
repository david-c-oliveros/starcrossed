#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <game_defines.h>
#include <texture.h>
#include <shader.h>



class SpriteRenderer
{
    public:
        SpriteRenderer();
        ~SpriteRenderer();

        void Create(Shader &shader);
        void Destroy();
        void DrawSprite(Texture2D &texture,
                        glm::vec2 vPos,
                        glm::vec2 vTexSizeInSheet,
                        glm::vec2 vTexOffset,
                        glm::vec2 vSize = BASE_TILE_SIZE,
                        glm::vec3 vColor = glm::vec3(1.0f),
                        float fRotation = 0.0f);

        void DrawSprite(glm::vec2 vPos,
                        glm::vec2 vSize = BASE_TILE_SIZE,
                        glm::vec3 vColor = glm::vec3(1.0f),
                        float fRotation = 0.0f);


    public:
        std::vector<std::string> vecDebugInfo;


    private:
        Shader cShader;
        uint32_t nQuadVAO;

        void initRenderData();
};
