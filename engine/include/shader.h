#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>

#ifndef __EMSCRIPTEN__
#include <glad/glad.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
    public:
        uint32_t nID;
        const char* vertexName;
        const char* fragmentName;

        Shader();
        ~Shader();

        void Create(const char* vertexPath, const char* fragmentPath);
        void Use();
        void PrintName();

        void SetBool(const std::string &name, bool value) const;
        void SetInt(const std::string &name, int value) const;
        void SetFloat(const std::string &name, float value) const;
        void SetVec3f(const std::string &name, glm::vec3 vec) const;
        void SetMat3(const std::string &name, glm::mat3 matrix) const;
        void SetMat4(const std::string &name, glm::mat4 matrix) const;
};

#endif
