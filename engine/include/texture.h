#pragma once

#include <stb_image.h>
#include "renderer.h"



class Texture2D
{
    public:
        Texture2D();
        void Create(const char* file, bool alpha);
        void Bind() const;


    public:
        GLuint nID;
        uint32_t nWidth, nHeight;

        uint32_t nInternal_Format;
        uint32_t nImage_Format;

        uint32_t nWrap_S;
        uint32_t nWrap_T;
        uint32_t nFilter_Min;
        uint32_t nFilter_Max;


    private:
        void generate(uint8_t* data);
        void loadTextureFromFile(const char* file, bool alpha);
};
