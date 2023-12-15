#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
    public:
        uint32_t nID;

        Shader();
        ~Shader();

        void Compile(const char* sVertexSource, const char* sFragmentSource, const char* sGeometrySource);
        void Use();
        void PrintName();

        void SetBool(const std::string &name, bool value) const;
        void SetInt(const std::string &name, int value) const;
        void SetFloat(const std::string &name, float value) const;
        void SetVec3f(const std::string &name, glm::vec3 vec) const;
        void SetMat3(const std::string &name, glm::mat3 matrix) const;
        void SetMat4(const std::string &name, glm::mat4 matrix) const;


    private:
        void checkCompileErrors(unsigned int nObject, std::string sType);
};
