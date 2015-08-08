#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <fstream>
#include <lua.hpp>
#include <LuaBridge.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "types.h"
#include "wrappers.h"
#include "auxiliary.h"
#include "player.h"
#include "entity_manager.h"
#include "transform_component.h"
#include "physics_component.h"
#include "physics_system.h"
#include "bounding_box_component.h"
#include "collision_system.h"
#include "simple_render_component.h"
#include "render_system.h"

using namespace te;

namespace te
{
    struct Sprite
    {
        TexturePtr pTexture;
        int w;
        int h;
        Sprite(TexturePtr pTexture, int w, int h)
            : pTexture(pTexture), w(w), h(h) {}
    };

    class CollisionHandler : public Observer<CollisionEvent>
    {
    public:
        typedef std::pair<Entity, Entity> EntityPair;

        std::map<EntityPair, std::function<void(Entity, Entity, float)>> map;

        virtual void onNotify(const CollisionEvent& evt)
        {
            EntityPair pair = { evt.a, evt.b };
            auto it = map.find(pair);
            if (it != map.end())
            {
                it->second(evt.a, evt.b, evt.dt);
            }
        }
    };

    class LuaGameState
    {
    public:
        LuaGameState(const TransformationData& tData, const std::string& filename = "init.lua")
            : mEntities()
            , mpTransformComponent(new TransformComponent())
            , mpPhysicsComponent(new PhysicsComponent())
            , mpBoundingBoxComponent(new BoundingBoxComponent(mpTransformComponent))
            , mpRenderComponent(new SimpleRenderComponent())
            , mEntityManager({mpTransformComponent, mpPhysicsComponent, mpBoundingBoxComponent, mpRenderComponent})
            , mPhysicsSystem(mpPhysicsComponent, mpTransformComponent)
            , mCollisionSystem(mpBoundingBoxComponent, {&mCollisionHandler})
            , mTransformationData(tData)
            , mRenderSystem(mpRenderComponent, mpTransformComponent, mTransformationData)
            , mKeyPressTable()
            , mKeyReleaseTable()
            , mCollisionHandler()
            , mFontCount(0)
            , mFontMap()
        {}

        typedef unsigned int EntityHandle;
        typedef std::pair<Entity, Entity> EntityPair;
        typedef luabridge::LuaRef LuaVector;
        typedef luabridge::LuaRef LuaFunction;
        typedef unsigned int FontHandle;
        typedef unsigned int TextHandle;

        Entity createEntity(glm::vec2 position, glm::vec2 velocity = glm::vec2(0, 0))
        {
            Entity entity = mEntityManager.create();
            mEntities.push_back(entity);
            mpTransformComponent->setLocalTransform(
                entity,
                glm::translate(glm::vec3(position.x, position.y, 0.f)));
            mpPhysicsComponent->setPhysics(entity, velocity);
            return entity;
        }

        void destroyEntity(Entity entity)
        {
            mEntityManager.destroy(entity);
        }

        void setPosition(Entity entity, glm::vec2 position)
        {
            if (!exists(entity)) return;

            mpTransformComponent->setLocalTransform(
                entity,
                glm::translate(glm::vec3(position.x, position.y, 0)));
        }

        glm::vec2 getPosition(Entity entity)
        {
            if (!exists(entity)) return glm::vec2(0.f, 0.f);

            glm::mat4 localTransform = mpTransformComponent->getLocalTransform(entity);
            glm::vec4 position = localTransform * glm::vec4(0.f, 0.f, 0.f, 1.f);

            return glm::vec2(position.x, position.y);
        }

        void setVelocity(Entity entity, glm::vec2 velocity)
        {
            if (!exists(entity)) return;

            mpPhysicsComponent->setPhysics(entity, velocity);
        }

        glm::vec2 getVelocity(Entity entity)
        {
            return mpPhysicsComponent->getVelocity(entity);
        }

        void setBoundingBox(Entity entity, glm::vec2 dimensions)
        {
            if (!exists(entity)) return;

            mpBoundingBoxComponent->setBoundingBox(entity, dimensions, { 0, 0 });
        }

        void setSprite(Entity entity, glm::vec2 dimensions)
        {
            mpRenderComponent->setSprite(entity, dimensions, { 0, 0 });
        }

        void scale(Entity entity, glm::vec3 scale)
        {
            mpTransformComponent->setLocalTransform(
                entity,
                glm::scale(mpTransformComponent->getLocalTransform(entity), scale));
        }

        void setParent(Entity child, Entity parent)
        {
            mpTransformComponent->setParent(child, parent);
        }

        void handleCollision(Entity e1, Entity e2, std::function<void(Entity,Entity,float)> handler)
        {
            if (!exists(e1) || !exists(e2)) return;

            auto key = std::make_pair(e1, e2);
            auto it = mCollisionHandler.map.find(key);
            if (it == mCollisionHandler.map.end())
            {
                mCollisionHandler.map.insert(std::make_pair(
                    key,
                    handler));
            }
            else
            {
                it->second = handler;
            }
        }

        void registerKeyPress(char character, std::function<void(void)> func)
        {
            auto it = mKeyPressTable.find(character);
            if (it == mKeyPressTable.end())
            {
                mKeyPressTable.insert(std::make_pair(character, func));
            }
            else
            {
                mKeyPressTable[character] = func;
            }
        }

        void registerKeyRelease(char character, std::function<void(void)> func)
        {
            auto it = mKeyReleaseTable.find(character);
            if (it == mKeyReleaseTable.end())
            {
                mKeyReleaseTable.insert(std::make_pair(character, func));
            }
            else
            {
                mKeyReleaseTable[character] = func;
            }
        }

        FontHandle loadFont(const std::string& filename, int ptSize)
        {
            FontPtr pFont = te::loadFont(filename, ptSize);

            mFontMap.insert(std::make_pair(
                mFontCount,
                pFont));

            return mFontCount++;
        }

        bool exists(Entity entity)
        {
            return mEntityManager.isAlive(entity);
        }

        void processInput(const SDL_Event& evt)
        {
            if (evt.type == SDL_KEYDOWN)
            {
                auto it = mKeyPressTable.find(evt.key.keysym.sym);
                if (it != mKeyPressTable.end())
                {
                    it->second();
                }
            }
            else if (evt.type == SDL_KEYUP)
            {
                auto it = mKeyReleaseTable.find(evt.key.keysym.sym);
                if (it != mKeyReleaseTable.end())
                {
                    it->second();
                }
            }
        }

        void update(float dt)
        {
            mPhysicsSystem.update(dt);
            mCollisionSystem.update(dt);
        }

        SDL_Rect getBoundingBox(Entity entity)
        {
            return mpBoundingBoxComponent->getBoundingBox(entity);
        }

        SDL_Rect getIntersection(Entity a, Entity b)
        {
            SDL_Rect aRect = getBoundingBox(a);
            SDL_Rect bRect = getBoundingBox(b);
            return te::getIntersection(aRect, bRect);
        }

        void draw()
        {
            mRenderSystem.draw();
        }

        void forEachEntity(const std::function<void(const Entity&)>& func)
        {
            std::for_each(mEntities.begin(), mEntities.end(), func);
        }

    private:
        std::vector<Entity> mEntities;

        TransformPtr mpTransformComponent;
        PhysicsPtr mpPhysicsComponent;
        BoundingBoxPtr mpBoundingBoxComponent;
        SimpleRenderPtr mpRenderComponent;

        EntityManager mEntityManager;

        PhysicsSystem mPhysicsSystem;
        CollisionSystem mCollisionSystem;

        TransformationData mTransformationData;

        RenderSystem mRenderSystem;

        std::map<char, std::function<void(void)>> mKeyPressTable;
        std::map<char, std::function<void(void)>> mKeyReleaseTable;

        CollisionHandler mCollisionHandler;

        FontHandle mFontCount;
        std::map<FontHandle, FontPtr> mFontMap;

        //std::map<Entity, GLuint> mVBOMap;
        //std::map<Entity, GLuint> mIBOMap;
    };
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

int main(int argc, char** argv)
{
    try
    {
        te::Initialization init;

        const int WIDTH = 640;
        const int HEIGHT = 480;

        te::WindowPtr pGLWindow = te::createWindowOpenGL("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        glClearColor(0.f, 0.5f, 1.f, 1.f);
        GLuint programID = loadProgram("vertex.glvs", "fragment.glfs");
        glUseProgram(programID);

        GLint projectionMatrixLocation = glGetUniformLocation(programID, "te_ProjectionMatrix");
        if (projectionMatrixLocation == -1) { throw std::runtime_error("te_ProjectionMatrix: not a valid program variable."); }
        glm::mat4 projectionMatrix = glm::ortho<GLfloat>(0.0, (GLfloat)WIDTH, (GLfloat)HEIGHT, 0.0, 1.0, -1.0);
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        GLint modelViewMatrixLocation = glGetUniformLocation(programID, "te_ModelViewMatrix");
        if (modelViewMatrixLocation == -1) { throw std::runtime_error("te_ModelViewMatrix: not a valid program variable."); }
        glm::mat4 modelViewMatrix;
        glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

        GLint vertex2DPositionLocation = glGetAttribLocation(programID, "te_VertexPos2D");
        if (vertex2DPositionLocation == -1) { throw std::runtime_error("te_Vertex2DPos2D: not a valid program variable."); }

        LuaGameState state({projectionMatrix, modelViewMatrix, projectionMatrixLocation, modelViewMatrixLocation, vertex2DPositionLocation});

        // State initialization

        Entity ball = state.createEntity({ 320, 240 }, { 350, 0 });
        state.setSprite(ball, { 25, 25 });
        state.setBoundingBox(ball, { 25, 25 });

        //Entity follower = state.createEntity({ 0, 30 }, { 0, 0 });
        //state.setSprite(follower, { 25, 25 });
        //state.setParent(follower, ball);
        //state.scale(ball, { 0.5, 2, 0 });

        Entity leftPaddle = state.createEntity({ 10, 240 });
        state.setSprite(leftPaddle, { 20, 100 });
        state.setBoundingBox(leftPaddle, { 20, 100 });

        Entity rightPaddle = state.createEntity({ 630, 240 });
        state.setSprite(rightPaddle, { 20, 100 });
        state.setBoundingBox(rightPaddle, { 20, 100 });

        auto handlePaddleCollision = [&state](Entity ball, Entity paddle, float dt)
        {
            glm::vec2 ballPos = state.getPosition(ball);
            glm::vec2 paddlePos = state.getPosition(paddle);
            glm::vec2 paddleToBall = ballPos - paddlePos;
            glm::vec2 ballVel = state.getVelocity(ball);
            glm::vec2 velocity = glm::length(ballVel) * glm::normalize(paddleToBall);
            state.setVelocity(ball, velocity);
        };
        state.handleCollision(ball, leftPaddle, handlePaddleCollision);
        state.handleCollision(ball, rightPaddle, handlePaddleCollision);

        state.registerKeyPress('w', [leftPaddle, &state]() { state.setVelocity(leftPaddle, glm::vec2(0, -100)); });
        state.registerKeyPress('s', [leftPaddle, &state]() { state.setVelocity(leftPaddle, glm::vec2(0, 100)); });
        state.registerKeyPress('p', [rightPaddle, &state]() { state.setVelocity(rightPaddle, glm::vec2(0, -100)); });
        state.registerKeyPress('l', [rightPaddle, &state]() { state.setVelocity(rightPaddle, glm::vec2(0, 100)); });

        state.registerKeyRelease('w', [leftPaddle, &state]() { state.setVelocity(leftPaddle, glm::vec2(0, 0)); });
        state.registerKeyRelease('s', [leftPaddle, &state]() { state.setVelocity(leftPaddle, glm::vec2(0, 0)); });
        state.registerKeyRelease('p', [rightPaddle, &state]() { state.setVelocity(rightPaddle, glm::vec2(0, 0)); });
        state.registerKeyRelease('l', [rightPaddle, &state]() { state.setVelocity(rightPaddle, glm::vec2(0, 0)); });

        Entity topWall = state.createEntity({ 320, -1 });
        state.setBoundingBox(topWall, { 640, 2 });
        Entity bottomWall = state.createEntity({ 320, 481 });
        state.setBoundingBox(bottomWall, { 640, 2 });

        auto handleWallCollision = [&state](Entity ball, Entity wall, float dt)
        {
            glm::vec2 velocity = state.getVelocity(ball);
            SDL_Rect intersection = state.getIntersection(ball, wall);
            if (intersection.w > intersection.h)
            {
                velocity.y = -velocity.y;
                state.setVelocity(ball, velocity);
            }
            else
            {
                velocity.x = -velocity.x;
                state.setVelocity(ball, velocity);
            }
        };
        state.handleCollision(ball, topWall, handleWallCollision);
        state.handleCollision(ball, bottomWall, handleWallCollision);

        // End state initialization

        SDL_Event e;
        bool running = true;

        Uint32 FPS = 60;
        Uint32 TIME_PER_FRAME = 1000 / FPS;

        Uint64 t0 = SDL_GetPerformanceCounter();

        while (running)
        {
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    running = false;
                }
                state.processInput(e);
            }

            Uint64 now = SDL_GetPerformanceCounter();
            float dt = (float)(now - t0) / SDL_GetPerformanceFrequency();

            state.update(dt);

            glClear(GL_COLOR_BUFFER_BIT);

            state.draw();

            SDL_GL_SwapWindow(pGLWindow.get());
            t0 = now;
        }
    }
    catch (std::exception e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
