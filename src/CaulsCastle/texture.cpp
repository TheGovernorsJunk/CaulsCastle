#include "texture.h"

namespace te
{
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
}
