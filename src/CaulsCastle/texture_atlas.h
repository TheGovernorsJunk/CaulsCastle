#ifndef TE_TEXTURE_ATLAS_H
#define TE_TEXTURE_ATLAS_H

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
	using Mesh = Container::value_type;
	using Vertex = decltype(Mesh::vertices)::value_type;
	using PositionCoord = decltype(Vertex::position_value_type::x);
	using TexCoord = decltype(Vertex::tex_value_type::x);

	glBindTexture(GL_TEXTURE_2D, texture_id);

	GLint tex_width, tex_height;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tex_width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &tex_height);

	glBindTexture(GL_TEXTURE_2D, NULL);

	for (auto& sprite : atlas.sprites) {
		decltype(Mesh::vertices) quad(4);
		quad[0].tex_coords = { (TexCoord)(sprite.x) / tex_width,
				       (TexCoord)(sprite.y) / tex_height };
		quad[1].tex_coords = { (TexCoord)(sprite.x + sprite.w) / tex_width,
				       (TexCoord)(sprite.y) / tex_height };
		quad[2].tex_coords = { (TexCoord)(sprite.x + sprite.w) / tex_width,
				       (TexCoord)(sprite.y + sprite.h) / tex_height };
		quad[3].tex_coords = { (TexCoord)(sprite.x) / tex_width,
				       (TexCoord)(sprite.y + sprite.h) / tex_height };

		quad[0].position = { (PositionCoord)0,
				     (PositionCoord)0 };
		quad[1].position = { (PositionCoord)sprite.w,
				     (PositionCoord)0 };
		quad[2].position = { (PositionCoord)sprite.w,
				     (PositionCoord)sprite.h };
		quad[3].position = { (PositionCoord)0,
				     (PositionCoord)sprite.h };

		out++ = { std::move(quad), texture_id, GL_QUADS };
	}
}

} // namespace te

#endif
