#ifndef TE_TEXTURE_ATLAS_H
#define TE_TEXTURE_ATLAS_H

#include <string>
#include <vector>

namespace te {

struct Texture_atlas {
	struct Sprite {
		std::string n;
		int x;
		int y;
		int w;
		int h;
		float pX;
		float pY;
	};

	std::string imagePath;
	int width;
	int height;
	std::vector<Sprite> sprites;

	Texture_atlas(const char* path);
};

} // namespace te

#endif
