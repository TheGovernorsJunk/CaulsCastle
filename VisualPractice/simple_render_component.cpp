#include "simple_render_component.h"

namespace te
{
    void destroyGLMemory(SimpleRenderInstance& instance)
    {
        glDeleteBuffers(1, &instance.vbo);
        glDeleteBuffers(1, &instance.ibo);
        instance.vbo = 0;
        instance.ibo = 0;
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
            glGenBuffers(1, &instance.vbo);
            glGenBuffers(1, &instance.ibo);
        }

        glm::vec2 vertices[4] = {
            { -dimensions.x / 2, -dimensions.y / 2 },
            { dimensions.x / 2, -dimensions.y / 2 },
            { dimensions.x / 2, dimensions.y / 2 },
            { -dimensions.x / 2, dimensions.y / 2 }
        };

        glBindBuffer(GL_ARRAY_BUFFER, instance.vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), vertices, GL_STATIC_DRAW);

        GLuint indices[4]{0, 1, 2, 3};

        glBindBuffer(GL_ARRAY_BUFFER, instance.ibo);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLuint), indices, GL_STATIC_DRAW);
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
