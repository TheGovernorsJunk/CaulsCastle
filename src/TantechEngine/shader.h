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
        Shader();
        Shader(const glm::mat4& projection);
        ~Shader();
        Shader(Shader&&);
        Shader& operator=(Shader&&);

        glm::mat4 getProjection() const;

        void setProjection(const glm::mat4& projection);
        void draw(const glm::mat4& view, const Mesh&) const;

        static void* operator new(std::size_t);
        static void operator delete(void*);
    private:
        GLuint mProgram;
        GLint mProjectionLocation;
        GLint mModelViewLocation;
        glm::mat4 mProjection;

        void destroy();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
    };
}

#endif
