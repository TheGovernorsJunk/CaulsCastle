#ifndef TE_TEXTURE_ATLAS_H
#define TE_TEXTURE_ATLAS_H

#include "mesh.h"
#include "records.h"

#include <SDL_opengl.h>

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

template <typename Container, template <typename> typename Iter>
void make_meshes(const Texture_atlas& atlas, GLuint texture_id, Iter<Container> out)
{
	for (auto& sprite : atlas.sprites) {
		Sprite_record record{
			sprite.n,
			atlas.imagePath,
			sprite.x,
			sprite.y,
			sprite.w,
			sprite.h,
			sprite.pX,
			sprite.pY
		};
		out++ = make_mesh(record, texture_id);
	}
}

} // namespace te

#endif
