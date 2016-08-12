#ifndef TE_TEXTURE_H
#define TE_TEXTURE_H

#include "utilities.h"

#include <SDL_opengl.h>
#include <IL/il.h>
#include <IL/ilu.h>

#include <string>
#include <cassert>
#include <vector>

namespace te {

std::vector<GLuint> load_image32(const std::string& path,
				 decltype(ilGetInteger(IL_IMAGE_WIDTH))& tex_width,
				 decltype(ilGetInteger(IL_IMAGE_WIDTH))& tex_height);

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

} // namespace te

#endif
