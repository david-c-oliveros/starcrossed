#include "sprite_renderer.h"



SpriteRenderer::SpriteRenderer()
{
}



SpriteRenderer::~SpriteRenderer()
{
}



void SpriteRenderer::Create(Shader &shader)
{
    vecDebugInfo = { "SpriteRenderer", "" };
    cShader = shader;
    initRenderData();
}



void SpriteRenderer::Destroy()
{
    glDeleteVertexArrays(1, &nQuadVAO);
}



void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 vPos,
                glm::vec2 vTexSizeInSheet, glm::vec2 vTexOffset,
                glm::vec2 vSize, glm::vec3 vColor, float fRotation)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(vPos, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * vSize.x, 0.5 * vSize.y, 0.0f));
    model = glm::rotate(model, glm::radians(fRotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * vSize.x, -0.5 * vSize.y, 0.0f));

    model = glm::scale(model, glm::vec3(vSize, 1.0f));

    cShader.SetBool("bTextured", true);
    cShader.SetVec2f("vTexSizeInSheet", vTexSizeInSheet);
    cShader.SetVec2f("vTexOffset", vTexOffset);
    cShader.SetMat4("model", model);
    cShader.SetVec3f("vSpriteColor", vColor);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    Renderer::Draw(nQuadVAO, cShader, 6);
}



void SpriteRenderer::DrawSprite(glm::vec2 vPos, glm::vec2 vSize, glm::vec3 vColor, float fRotation)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(vPos, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * vSize.x, 0.5 * vSize.y, 0.0f));
    model = glm::rotate(model, glm::radians(fRotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * vSize.x, -0.5 * vSize.y, 0.0f));

    model = glm::scale(model, glm::vec3(vSize, 1.0f));

    cShader.SetBool("bTextured", false);
    cShader.SetMat4("model", model);
    cShader.SetVec3f("vSpriteColor", vColor);

    Renderer::Draw(nQuadVAO, cShader, 6);
}



void SpriteRenderer::initRenderData()
{
    uint32_t nVBO;
    float vertices[] =
    {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
    };


    glGenVertexArrays(1, &nQuadVAO);
    glGenBuffers(1, &nVBO);

    glBindBuffer(GL_ARRAY_BUFFER, nVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(nQuadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
