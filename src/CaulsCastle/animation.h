#ifndef TE_ANIMATION_H
#define TE_ANIMATION_H

#include "resource_holder.h"
#include "types.h"

#include <string>
#include <vector>

namespace te {

struct Animation_csv {
	struct Frame {
		int delay;
		std::string name;
	};

	std::vector<Frame> frames;

	Animation_csv(const std::string& filename);
};

template <typename Mesh>
struct Animation {
	struct Frame {
		int delay;
		Resource_id<Mesh> mesh_id;
	};

	std::vector<Frame> frames;
};

} // namespace te

#endif
