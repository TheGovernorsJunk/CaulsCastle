#include "game_data.h"

#include <Box2D/Box2D.h>

namespace te
{
	Entity_manager::Entity_manager()
		: m_next_id{ 1 }
	{}

	Entity_id Entity_manager::get_free_id()
	{
		return m_next_id++;
	}

	Game_data::Game_data()
		: physics_world{ std::make_unique<b2World>(b2Vec2{ 0, 0 }) }
	{}
	Game_data::~Game_data() = default;
}
