#include "resource_manager.h"



std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader>    ResourceManager::Shaders;



Shader ResourceManager::LoadShader(const char* vShaderFile,
                                   const char* fShaderFile,
                                   const char* gShaderFile,
                                   std::string sName)
{
    Shaders[sName] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[sName];
}



Shader ResourceManager::GetShader(std::string sName)
{
    return Shaders[sName];
}



Texture2D ResourceManager::LoadTexture(const char* cFile, bool bAlpha, std::string sName)
{
    return Textures[sName] = loadTextureFromFile(cFile, bAlpha);
}



Texture2D ResourceManager::GetTexture(std::string sName)
{
    return Textures[sName];
}



void ResourceManager::Clear()
{
    for (auto iter : Shaders)
    {
        glDeleteProgram(iter.second.nID);
    }

    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.nID);
}



Texture2D ResourceManager::loadTextureFromFile(const char* cFile, bool bAlpha)
{
    Texture2D cTexture;

    if (bAlpha)
    {
        cTexture.nInternal_Format = GL_RGBA;
        cTexture.nImage_Format = GL_RGBA;
    }

    int32_t nWidth = 0;
    int32_t nHeight = 0;
    int32_t nrChannels = 0;
    uint8_t* pData = stbi_load(cFile, &nWidth, &nHeight, &nrChannels, 0);

    if (pData == NULL)
        std::cout << "WARNING::File " << cFile << " not found" << std::endl;

    cTexture.Generate(pData, nWidth, nHeight);
    
    stbi_image_free(pData);

    return cTexture;
}



Shader ResourceManager::loadShaderFromFile(const char* vShaderFile,
                                           const char* fShaderFile,
                                           const char* gShaderFile)
{
    std::string sVertexCode;
    std::string sFragmentCode;
    std::string sGeometryCode;

    try
    {
        std::ifstream vShaderFileIn(vShaderFile);
        std::ifstream fShaderFileIn(fShaderFile);

        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        vShaderStream << vShaderFileIn.rdbuf();
        fShaderStream << fShaderFileIn.rdbuf();

        vShaderFileIn.close();
        fShaderFileIn.close();

        sVertexCode = vShaderStream.str();
        sFragmentCode = fShaderStream.str();

        if (gShaderFile != nullptr)
        {
            std::ifstream gShaderFileIn(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFileIn.rdbuf();
            gShaderFileIn.close();
            sGeometryCode = gShaderStream.str();
        }
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char* vShaderCode = sVertexCode.c_str();
    const char* fShaderCode = sFragmentCode.c_str();
    const char* gShaderCode = sGeometryCode.c_str();

    Shader cShader;
    cShader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);

    return cShader;
}
