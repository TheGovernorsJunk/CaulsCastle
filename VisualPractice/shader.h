#ifndef TE_SHADER_H
#define TE_SHADER_H

#include "gl.h"
#include <string>

namespace te
{
    std::string getShaderLog(GLuint shader);
    std::string getProgramLog(GLuint program);
    GLuint loadShader(const std::string& path, GLenum shaderType);
    GLuint loadProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
}

#endif
