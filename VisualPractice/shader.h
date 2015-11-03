#ifndef TE_SHADER_H
#define TE_SHADER_H

#include "gl.h"
#include <glm/glm.hpp>
#include <string>

namespace te
{
    class Mesh;

    std::string getShaderLog(GLuint shader);
    std::string getProgramLog(GLuint program);
    GLuint loadShader(const std::string& path, GLenum shaderType);
    GLuint loadProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

    class Shader {
    public:
        Shader(const glm::mat4& projection, const glm::mat4& model);
        ~Shader();
        Shader(Shader&&);
        Shader& operator=(Shader&&);

        void draw(const glm::mat4& modelview, const Mesh&);
    private:
        GLuint mProgram;
        glm::mat4 mLastView;

        void destroy();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
    };
}

#endif
