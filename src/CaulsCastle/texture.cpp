#include "texture.h"

#include <vector>
#include <iterator>

namespace te {

std::vector<GLuint> load_image32(const std::string& path,
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
	ilDeleteImages(1, &image_id);
	return pixels;
}

GLuint load_texture32(GLuint* pixels, GLuint width, GLuint height)
{
	GLuint texture_id{};
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, NULL);

	return texture_id;
}

GLuint load_texture32(const std::string& path)
{
	ILint width{}, height{};
	auto pixels = load_image32(path, width, height);
	return load_texture32(pixels.data(), width, height);
}

Texture::Texture(GLuint texture_id)
	: m_texture_id{ texture_id }
{}

Texture::~Texture()
{
	destroy_texture();
}

Texture::Texture(Texture&& rhs)
	: m_texture_id{ rhs.m_texture_id }
{
	rhs.m_texture_id = 0;
}

Texture& Texture::operator=(Texture&& rhs)
{
	destroy_texture();
	m_texture_id = rhs.m_texture_id;
	rhs.m_texture_id = 0;
	return *this;
}

GLuint Texture::get_texture_id() const
{
	return m_texture_id;
}

void Texture::destroy_texture()
{
	if (m_texture_id > 0) glDeleteTextures(1, &m_texture_id);
}

} // namespace te
