#ifndef TE_ANIMATION_H
#define TE_ANIMATION_H

#include <string>
#include <vector>

namespace te {

struct Animation {
	struct Frame {
		int delay;
		std::string name;
	};

	std::vector<Frame> frames;

	Animation(const std::string& filename);
};

} // namespace te

#endif
