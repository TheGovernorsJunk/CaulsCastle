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

	template <typename P, typename T>
	struct vertex {
		using position_value_type = P;
		using tex_value_type = T;
		P position;
		T tex_coords;
	};

	template <typename K, typename V>
	using component = boost::container::flat_map<K, V>;
	template <typename K, typename V>
	using flat_map = boost::container::flat_map<K, V>;

	template <typename K, typename V>
	using Multi_component = std::vector<std::pair<K, V>>;

	template <typename PositionVec, typename TexVec>
	using Vertex_array = std::vector<vertex<PositionVec, TexVec>>;
}

#endif
