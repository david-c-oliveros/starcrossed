#include "shader.h"


Shader::Shader()
{
}


Shader::~Shader()
{
}


void Shader::Compile(const char* sVertexSource, const char* sFragmentSource, const char* sGeometrySource)
{

    unsigned int sVertex;
    unsigned int sFragment;
    unsigned int sGeometry;
    int success;
    char infoLog[512];

    /***************************************/
    /*        Compile Vertex Shader        */
    /***************************************/
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &sVertexSource, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");



    /*****************************************/
    /*        Compile Fragment Shader        */
    /*****************************************/
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &sFragmentSource, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");



    /*****************************************/
    /*        Compile Geometry Shader        */
    /*****************************************/
    if (sGeometrySource != nullptr)
    {
        sGeometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(sGeometry, 1, &sGeometrySource, NULL);
        glCompileShader(sGeometry);
        checkCompileErrors(sGeometry, "GEOMETRY");
    }



    /***************************************/
    /*        Create Shader Program        */
    /***************************************/
    nID = glCreateProgram();
    glAttachShader(nID, sVertex);
    glAttachShader(nID, sFragment);
    if (sGeometrySource != nullptr)
        glAttachShader(nID, sGeometry);
    glLinkProgram(nID);

    checkCompileErrors(nID, "PROGRAM");

    glDeleteShader(sVertex);
    glDeleteShader(sFragment);

    if (sGeometrySource != nullptr)
        glDeleteShader(sGeometry);
}


void Shader::Use()
{
    glUseProgram(nID);
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



void Shader::checkCompileErrors(unsigned int nObject, std::string sType)
{
    int iSuccess;
    char cInfoLog[1024];
    if (sType != "PROGRAM")
    {
        glGetShaderiv(nObject, GL_COMPILE_STATUS, &iSuccess);
        if (!iSuccess)
        {
            glGetShaderInfoLog(nObject, 1024, NULL, cInfoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << sType << "\n"
                << cInfoLog << "\n-- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(nObject, GL_COMPILE_STATUS, &iSuccess);
        if (!iSuccess)
        {
            glGetProgramInfoLog(nObject, 1024, NULL, cInfoLog);
            std::cout << "| ERROR::SHADER: Link-time error: Type: " << sType << "\n"
                << cInfoLog << "\n-- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}
