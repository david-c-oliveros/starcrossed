#include "texture.h"



Texture2D::Texture2D()
    : nWidth(0), nHeight(0),
      nInternal_Format(GL_RGB), nImage_Format(GL_RGB),
      nWrap_S(GL_REPEAT), nWrap_T(GL_REPEAT),
      nFilter_Min(GL_LINEAR), nFilter_Max(GL_NEAREST)
{
    glGenTextures(1, &nID);
}



void Texture2D::Bind() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, nID);
}



void Texture2D::Generate(uint8_t* pData, uint32_t _nWidth, uint32_t _nHeight)
{
    nWidth = _nWidth;
    nHeight = _nHeight;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, nID);
    glTexImage2D(GL_TEXTURE_2D, 0, nInternal_Format, nWidth, nHeight, 0, nImage_Format, GL_UNSIGNED_BYTE, pData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, nWrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, nWrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, nFilter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, nFilter_Max);

    glBindTexture(GL_TEXTURE_2D, 0);
}
