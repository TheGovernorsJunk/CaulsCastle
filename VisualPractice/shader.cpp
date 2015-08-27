#include "shader.h"

#include <stdexcept>
#include <fstream>

namespace te
{
    std::string getShaderLog(GLuint shader)
    {
        if (!glIsShader(shader))
        {
            throw std::runtime_error("Passed ID is not a shader.");
        }

        int infoLogLength = 0;
        int maxLength = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        char* infoLog = new char[maxLength];

        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0)
        {
            std::string log{ infoLog };
            delete[] infoLog;
            return log;
        }
        else
        {
            return{ "" };
        }
    }

    std::string getProgramLog(GLuint program)
    {
        if (!glIsProgram(program))
        {
            throw std::runtime_error("Passed ID is not a program.");
        }

        int infoLogLength = 0;
        int maxLength = 0;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        char* infoLog = new char[maxLength];

        glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0)
        {
            std::string log{ infoLog };
            delete[] infoLog;
            return log;
        }
        else
        {
            return{ "" };
        }
    }

    GLuint loadShader(const std::string& path, GLenum shaderType)
    {
        GLuint shaderID = 0;
        std::string shaderString;
        std::ifstream srcFile(path.c_str());

        if (!srcFile) { throw std::runtime_error("Unable to open file."); }

        shaderString.assign(std::istreambuf_iterator<char>(srcFile), std::istreambuf_iterator<char>());
        shaderID = glCreateShader(shaderType);

        const GLchar* shaderSrc = shaderString.c_str();
        glShaderSource(shaderID, 1, (const GLchar**)&shaderSrc, NULL);

        glCompileShader(shaderID);

        GLint shaderCompiled = GL_FALSE;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);

        if (shaderCompiled != GL_TRUE)
        {
            std::string msg{ getShaderLog(shaderID) };
            glDeleteShader(shaderID);
            throw std::runtime_error(msg);
        }

        return shaderID;
    }

    GLuint loadProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        GLuint program = glCreateProgram();
        GLuint vertexShader = 0;
        GLuint fragmentShader = 0;

        try
        {
            vertexShader = loadShader(vertexShaderPath, GL_VERTEX_SHADER);

            glAttachShader(program, vertexShader);

            fragmentShader = loadShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

            glAttachShader(program, fragmentShader);

            glLinkProgram(program);

            GLint programSuccess = GL_TRUE;
            glGetProgramiv(program, GL_LINK_STATUS, &programSuccess);
            if (programSuccess != GL_TRUE)
            {
                throw std::runtime_error(getProgramLog(program));
            }

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }
        catch (std::exception e)
        {
            glDeleteProgram(program);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            throw std::runtime_error(e.what());
        }

        return program;
    }
}
