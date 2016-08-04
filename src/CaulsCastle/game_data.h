#ifndef TE_GAME_DATA_H
#define TE_GAME_DATA_H

#include <SDL.h>
#include <boost/container/flat_map.hpp>
#include <glm/glm.hpp>

namespace te
{
	using entity_id = int;
	using vec2 = glm::vec2;

	template <typename K, typename V>
	using component = boost::container::flat_map<K, V>;

	struct game_data {
		entity_id next_id;
		component<entity_id, vec2> positions;

		game_data();
	};
}

#endif
