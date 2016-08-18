#ifndef TE_ANIMATION_H
#define TE_ANIMATION_H

#include "resource_holder.h"
#include "types.h"
#include "mesh.h"

#include <string>
#include <vector>
#include <numeric>

namespace te {

struct Animation_csv {
	struct Frame {
		int delay;
		std::string name;
	};

	std::vector<Frame> frames;
	int delay_unit;

	Animation_csv(const std::string& filename);
};

template <typename Mesh>
struct Animation {
	struct Frame {
		int delay;
		Resource_id<Mesh> mesh_id;
	};

	std::vector<Frame> frames;
	int delay_unit;
};

template <typename Animation>
float get_duration(const Animation& animation)
{
	return std::accumulate(std::begin(animation.frames), std::end(animation.frames), 0.f, [](float curr_total, const auto& frame) {
		return curr_total + frame.delay;
	}) / animation.delay_unit;
}

using Animation2 = Animation<Mesh2>;
using Animation3 = Animation<Mesh3>;

} // namespace te

#endif
