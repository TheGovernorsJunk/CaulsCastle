#ifndef TE_SCRIPTING_H
#define TE_SCRIPTING_H

#include "typedefs.h"

namespace te
{
	struct GameData;

	class ScriptInit
	{
	public:
		ScriptInit(GameData&);
	private:
		class ProxyEntity;

		ProxyEntity makeEntity();

		GameData& m_rData;
	};
}

#endif
