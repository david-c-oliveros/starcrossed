#pragma once

#include <map>
#include <string>
#include <memory>

#include <stb_image.h>

#include "texture.h"
#include "shader.h"



class ResourceManager
{
    public:
        static std::map<std::string, Shader>    Shaders;
        static std::map<std::string, Texture2D> Textures;

        static std::map<std::string, int32_t> nums;
        static int32_t var;

        static Shader shader;

        static void SetVar(std::string s);
        static int32_t GetVar(std::string s);

        static Shader LoadShader(const char* vShaderFile,
                                 const char* fShaderFile,
                                 const char* gShaderFile,
                                 std::string sName);

        static Shader GetShader(std::string sName);

        static Texture2D LoadTexture(const char* cFile,
                                     bool bAlpha,
                                     std::string sName);

        static Texture2D GetTexture(std::string sName);

        static void Clear();


    private:
        ResourceManager() {}
        static Shader loadShaderFromFile(const char* vShaderFile,
                                         const char* fShaderFile,
                                         const char* gShaderFile = nullptr);

        static Texture2D loadTextureFromFile(const char* cFile, bool bAlpha);
};
