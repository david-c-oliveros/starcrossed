#include <texture.h>



Texture2D::Texture2D()
    : nWidth(0), nHeight(0),
      nInternal_Format(GL_RGB), nImage_Format(GL_RGB),
      nWrap_S(GL_REPEAT), nWrap_T(GL_REPEAT),
      nFilter_Min(GL_LINEAR), nFilter_Max(GL_LINEAR)
{
}



void Texture2D::Create(const char* file, bool alpha)
{
    glGenTextures(1, &nID);

    loadTextureFromFile(file, alpha);
}



void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, nID);
}



void Texture2D::generate(uint8_t* data)
{
    glBindTexture(GL_TEXTURE_2D, nID);
    glTexImage2D(GL_TEXTURE_2D, 0, nInternal_Format, nWidth, nHeight, 0, nImage_Format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, nWrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, nWrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, nFilter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, nFilter_Max);

    glBindTexture(GL_TEXTURE_2D, 0);
}



void Texture2D::loadTextureFromFile(const char* file, bool alpha)
{
    if (alpha)
    {
        nInternal_Format = GL_RGBA;
        nImage_Format = GL_RGBA;
    }

    int32_t width = 0;
    int32_t height = 0;
    int32_t nrChannels = 0;
    uint8_t* data = stbi_load(file, &width, &height, &nrChannels, 0);

    if (data == NULL)
        std::cout << "WARNING::File " << file << " not found" << std::endl;

    std::cout << "Image width: " << width << std::endl;
    std::cout << "Image height: " << height << std::endl;
    std::cout << "Number of channels: " << nrChannels << std::endl;
    nWidth = width;
    nHeight = height;

    generate(data);
    
    stbi_image_free(data);
}
