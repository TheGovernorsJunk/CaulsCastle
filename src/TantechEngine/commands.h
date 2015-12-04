#ifndef TE_COMMANDS_H
#define TE_COMMANDS_H

#include <lua.hpp>
#include <LuaBridge.h>

#include <functional>

namespace te
{
    class Entity;
    struct ECS;

    static enum class FuncID
    { MOVE };

    std::function<void(const Entity&, const ECS&, float)> (*getFunction(FuncID))(luabridge::LuaRef);
}

#endif
