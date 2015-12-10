#include "shader.h"
#include "mesh.h"
#include "texture.h"

#include <glm/gtc/type_ptr.hpp>

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

    Shader::Shader()
        : Shader(glm::mat4())
    {}

    Shader::Shader(const glm::mat4& projection)
        : mProgram(loadProgram("assets/shaders/basic.glvs", "assets/shaders/basic.glfs"))
        , mProjectionLocation(glGetUniformLocation(mProgram, "te_ProjectionMatrix"))
        , mModelViewLocation(glGetUniformLocation(mProgram, "te_ModelViewMatrix"))
        , mProjection(projection)
    {
        glUseProgram(mProgram);

        if (mProjectionLocation == -1) { throw std::runtime_error("te_ProjectionMatrix: not a valid program variable."); }
        glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        if (mModelViewLocation == -1) { throw std::runtime_error{ "te_ModelViewMatrix: not a valid program variable." }; }
        glUniformMatrix4fv(mModelViewLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4()));
    }

    void Shader::destroy()
    {
        glDeleteProgram(mProgram);
    }

    Shader::~Shader()
    {
        destroy();
    }

    Shader::Shader(Shader&& o)
        : mProgram(o.mProgram)
    {
        o.mProgram = 0;
    }

    Shader& Shader::operator=(Shader&& o)
    {
        destroy();

        mProgram = o.mProgram;
        o.mProgram = 0;

        return *this;
    }

    glm::mat4 Shader::getProjection() const
    {
        return mProjection;
    }

    void Shader::setProjection(const glm::mat4& projection)
    {
        glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
        mProjection = projection;
    }

    void Shader::draw(const glm::mat4& modelview, const Mesh& mesh) const
    {
        glUseProgram(mProgram);

        glUniformMatrix4fv(mModelViewLocation, 1, GL_FALSE, glm::value_ptr(modelview));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.getTexture(0)->getID());
        glBindVertexArray(mesh.getVAO());
        glDrawElements(GL_TRIANGLES, mesh.getElementCount(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(0);
    }

    void* Shader::operator new(std::size_t sz)
    {
        return _aligned_malloc(sz, 16);
    }

    void Shader::operator delete(void* ptr)
    {
        _aligned_free(ptr);
    }
}
