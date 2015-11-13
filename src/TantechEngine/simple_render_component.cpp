#include "simple_render_component.h"
#include "shader.h"
#include <glm/gtc/type_ptr.hpp>

namespace te
{
    void destroyGLMemory(SimpleRenderInstance& instance)
    {
        glDeleteVertexArrays(1, &instance.vao);
        glDeleteBuffers(1, &instance.vbo);
        instance.vao = 0;
        instance.vbo = 0;
    }

    SimpleRenderComponent::SimpleRenderComponent(const glm::mat4& projection)
        : mShader(loadProgram("shaders/simple_render_component.glvs", "shaders/simple_render_component.glfs"))
        , mProjectionLocation(glGetUniformLocation(mShader, "te_ProjectionMatrix"))
        , mViewLocation(glGetUniformLocation(mShader, "te_ViewMatrix"))
        , mModelLocation(glGetUniformLocation(mShader, "te_ModelMatrix"))
    {
        glUseProgram(mShader);
        glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
        glUseProgram(0);
    }

    SimpleRenderComponent::~SimpleRenderComponent()
    {
        forEach([](const Entity&, SimpleRenderInstance& instance)
        {
            destroyGLMemory(instance);
        });
    }

    void SimpleRenderComponent::setSprite(const Entity& entity, const glm::vec2& dimensions, const glm::vec2& offset)
    {
        bool exists = hasInstance(entity);
        SimpleRenderInstance& instance = exists ? at(entity) : createInstance(entity, { 0, 0 });

        if (!exists)
        {
            glGenVertexArrays(1, &instance.vao);
            glGenBuffers(1, &instance.vbo);
        }

        glBindVertexArray(instance.vao);

        glm::vec3 vertices[4] = {
            { -dimensions.x / 2, -dimensions.y / 2, 0 },
            { dimensions.x / 2, -dimensions.y / 2, 0 },
            { dimensions.x / 2, dimensions.y / 2, 0 },
            { -dimensions.x / 2, dimensions.y / 2, 0 }
        };

        glBindBuffer(GL_ARRAY_BUFFER, instance.vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void SimpleRenderComponent::destroyInstance(const Entity& entity)
    {
        if (hasInstance(entity))
        {
            SimpleRenderInstance& instance = at(entity);
            destroyGLMemory(instance);
        }
        Component::destroyInstance(entity);
    }
}
