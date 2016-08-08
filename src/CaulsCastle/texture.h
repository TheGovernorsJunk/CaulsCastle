#ifndef TE_TEXTURE_H
#define TE_TEXTURE_H

#include "utilities.h"

#include <SDL_opengl.h>
#include <IL/il.h>
#include <IL/ilu.h>

#include <string>
#include <cassert>

namespace te
{
	template <typename Iter>
	void load_image32(const std::string& path,
			  Iter out,
			  decltype(ilGetInteger(IL_IMAGE_WIDTH))& tex_width,
			  decltype(ilGetInteger(IL_IMAGE_WIDTH))& tex_height)
	{
		ILuint image_id{};
		ilGenImages(1, &image_id);
		ilBindImage(image_id);

		assert(ilLoadImage(path.c_str()) == IL_TRUE);
		assert(ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE) == IL_TRUE);

		auto image_width = ilGetInteger(IL_IMAGE_WIDTH);
		auto image_height = ilGetInteger(IL_IMAGE_HEIGHT);
		tex_width = pow2up(image_width);
		tex_height = pow2up(image_height);

		if (image_width != tex_width || image_height != tex_height) {
			iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);
			iluEnlargeCanvas(tex_width, tex_height, 1);
		}

		auto size = tex_width * tex_height;
		std::vector<GLuint> pixels(size);
		std::memcpy(pixels.data(), ilGetData(), size * 4);
		for (auto pixel : pixels) {
			out++ = pixel;
		}

		ilDeleteImages(1, &image_id);
	}

	GLuint load_texture32(GLuint* pixels, GLuint width, GLuint height);
	GLuint load_texture32(const std::string& path);

	class Texture {
	public:
		Texture(GLuint texture_id);
		~Texture();
		Texture(Texture&& rhs);
		Texture& operator=(Texture&& rhs);

		GLuint get_texture_id() const;
	private:
		void destroy_texture();

		GLuint m_texture_id;
	};
}

#endif
