#include "game_data.h"

namespace te
{
	Entity_manager::Entity_manager()
		: m_next_id{ 1 }
	{}

	Entity_id Entity_manager::get_free_id()
	{
		return m_next_id++;
	}
}
