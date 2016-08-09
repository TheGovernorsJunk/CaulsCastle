#ifndef TE_TYPES_H
#define TE_TYPES_H

#include <SDL.h>
#include <boost/container/flat_map.hpp>
#include <glm/glm.hpp>

#include <vector>

namespace te
{
	using Entity_id = int;
	using Player_id = int;
	using vec2 = glm::vec2;

	template <typename T>
	struct vertex {
		using value_type = T;
		T position;
		T tex_coords;
	};

	template <typename K, typename V>
	using component = boost::container::flat_map<K, V>;
	template <typename K, typename V>
	using flat_map = boost::container::flat_map<K, V>;

	template <typename K, typename V>
	using Multi_component = std::vector<std::pair<K, V>>;

	template <typename Vec>
	using Vertex_array = std::vector<vertex<Vec>>;
}

#endif
