#pragma once

#include <stb_image.h>
#include "renderer.h"



class Texture2D
{
    public:
        Texture2D();
        void Bind() const;
        void Generate(uint8_t* pData, uint32_t _nWidth, uint32_t _nHeight);


    public:
        GLuint nID;
        uint32_t nWidth, nHeight;

        uint32_t nInternal_Format;
        uint32_t nImage_Format;

        uint32_t nWrap_S;
        uint32_t nWrap_T;
        uint32_t nFilter_Min;
        uint32_t nFilter_Max;
};
