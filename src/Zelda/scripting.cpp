#include "scripting.h"
#include "game_data.h"
#include "utilities.h"

#include <lua.hpp>
#include <LuaBridge.h>

#include <regex>

namespace te
{
	const static std::regex packageExpr{"(?:.*/)*([a-zA-Z_0-9]+)\\.lua"};
	static std::string getPackageName(const std::string& scriptFilename)
	{
		std::smatch match;
		if (std::regex_match(scriptFilename, match, packageExpr))
		{
			return match[1];
		}
		throw std::runtime_error{"Could not match Lua script name."};
	}

	class ScriptInit::ProxyEntity
	{
	public:
		ProxyEntity(EntityID id)
			: m_ID{ id }
		{}
	private:
		EntityID m_ID;
	};

	ScriptInit::ScriptInit(GameData& data)
		: m_rData{data}
	{
		lua_State* L = m_rData.pL.get();

		luabridge::getGlobalNamespace(L)
			.beginClass<ScriptInit>("Game")
				.addFunction("makeEntity", &ScriptInit::makeEntity)
			.endClass()
			.beginClass<ProxyEntity>("Entity")
			.endClass();

		doLuaFile(*L, m_rData.config.initialScript);

		std::string packageName = getPackageName(m_rData.config.initialScript);

		luabridge::LuaRef pkg = luabridge::getGlobal(L, packageName.c_str());
		if (!pkg.isTable()) throw std::runtime_error{"Package `" + packageName + "' not found. Package name must match filename."};

		luabridge::LuaRef init = pkg["init"];
		if (!init.isFunction()) throw std::runtime_error{"Must supply init function."};

		init(this);
	}

	ScriptInit::ProxyEntity ScriptInit::makeEntity()
	{
		return ProxyEntity{ m_rData.entityIDManager.getNextID() };
	}
}
