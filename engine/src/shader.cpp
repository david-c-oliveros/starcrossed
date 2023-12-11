#include "shader.h"


Shader::Shader()
{
}


Shader::~Shader()
{
}


void Shader::Create(const char* vertexPath, const char* fragmentPath)
{
    vertexName = vertexPath;
    fragmentName = fragmentPath;
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex;
    unsigned int fragment;
    int success;
    char infoLog[512];

    /***************************************/
    /*        Compile Vertex Shader        */
    /***************************************/
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << vertexName << std::endl;
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    /*****************************************/
    /*        Compile Fragment Shader        */
    /*****************************************/
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << fragmentName << std::endl;
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    /***************************************/
    /*        Create Shader Program        */
    /***************************************/
    nID = glCreateProgram();
    glAttachShader(nID, vertex);
    glAttachShader(nID, fragment);
    glLinkProgram(nID);

    glGetProgramiv(nID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(nID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


void Shader::Destroy()
{
    glDeleteProgram(nID);
}


void Shader::Use()
{
    glUseProgram(nID);
}


void Shader::PrintName()
{
    std::cout << vertexName << std::endl;
    std::cout << fragmentName << std::endl;
}


void Shader::SetBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(nID, name.c_str()), value);
}


void Shader::SetInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(nID, name.c_str()), value);
}


void Shader::SetFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(nID, name.c_str()), value);
}


void Shader::SetVec3f(const std::string &name, glm::vec3 vec) const
{
    glUniform3fv(glGetUniformLocation(nID, name.c_str()), 1, glm::value_ptr(vec));
}


void Shader::SetMat3(const std::string &name, glm::mat3 matrix) const
{
    glUniformMatrix3fv(glGetUniformLocation(nID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}


void Shader::SetMat4(const std::string &name, glm::mat4 matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(nID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
