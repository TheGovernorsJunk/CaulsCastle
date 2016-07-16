#ifndef TE_SCRIPTING_H
#define TE_SCRIPTING_H

#include "typedefs.h"

#include <memory>

namespace te
{
	struct GameData;

	class ScriptInit
	{
	public:
		ScriptInit(GameData&);
		ScriptInit(ScriptInit&&);
		ScriptInit& operator=(ScriptInit&&);
		~ScriptInit();
	private:
		struct Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}

#endif
