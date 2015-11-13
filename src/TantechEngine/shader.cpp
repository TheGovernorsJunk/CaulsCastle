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

    Shader::Shader(const glm::mat4& projection, const glm::mat4& model)
        : mProgram(loadProgram("shaders/basic.glvs", "shaders/basic.glfs"))
        , mViewLocation(glGetUniformLocation(mProgram, "te_ViewMatrix"))
        , mLastView()
        , mModel(model)
    {
        glUseProgram(mProgram);

        GLint projectionMatrixLocation = glGetUniformLocation(mProgram, "te_ProjectionMatrix");
        if (projectionMatrixLocation == -1) { throw std::runtime_error("te_ProjectionMatrix: not a valid program variable."); }
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projection));

        if (mViewLocation == -1) { throw std::runtime_error{ "te_ViewMatrix: not a valid program variable." }; }
        glUniformMatrix4fv(mViewLocation, 1, GL_FALSE, glm::value_ptr(mLastView));

        GLint modelMatrixLocation = glGetUniformLocation(mProgram, "te_ModelMatrix");
        if (modelMatrixLocation == -1) { throw std::runtime_error("te_ModelMatrix: not a valid program variable."); }
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model));
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
        , mLastView(o.mLastView)
    {
        o.mProgram = 0;
    }

    Shader& Shader::operator=(Shader&& o)
    {
        destroy();

        mProgram = o.mProgram;
        mLastView = o.mLastView;
        o.mProgram = 0;

        return *this;
    }

    glm::mat4 Shader::getModel() const
    {
        return mModel;
    }

    void Shader::draw(const glm::mat4& view, const Mesh& mesh)
    {
        glUseProgram(mProgram);

        if (view != mLastView) {
            glUniformMatrix4fv(mViewLocation, 1, GL_FALSE, glm::value_ptr(view));
            mLastView = view;
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.getTexture(0)->getID());
        glBindVertexArray(mesh.getVAO());
        glDrawElements(GL_TRIANGLES, mesh.getElementCount(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(0);
    }
}
