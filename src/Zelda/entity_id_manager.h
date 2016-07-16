#ifndef TE_ENTITY_ID_MANAGER_H
#define TE_ENTITY_ID_MANAGER_H

#include "typedefs.h"

namespace te
{
	class EntityIDManager
	{
	public:
		EntityIDManager();

		EntityID getNextID();
	private:
		EntityID m_NextID;
	};
}

#endif
