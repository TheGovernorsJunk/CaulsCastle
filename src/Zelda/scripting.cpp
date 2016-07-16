#include "scripting.h"
#include "game_data.h"
#include "utilities.h"

#include <lua.hpp>

namespace te
{
	void registerInterface(GameData& data)
	{
		lua_State* L = data.pL.get();
		doLuaFile(*L, data.config.initialScript);
	}
}
