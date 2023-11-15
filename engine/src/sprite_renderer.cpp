#include <sprite_renderer.h>



SpriteRenderer::SpriteRenderer()
{
}



SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &nQuadVAO);
}



void SpriteRenderer::Create(Shader &shader)
{
    cShader = shader;
    initRenderData();
}



void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 position,
                                glm::vec2 size, float rotate, glm::vec3 color)
{
    cShader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5 * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5 * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    cShader.SetMat4("model", model);

    cShader.SetVec3f("vColor", color);

    Renderer::Draw(texture.nID, nQuadVAO, cShader.nID, 4);
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
