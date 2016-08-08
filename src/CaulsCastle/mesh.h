#ifndef TE_MESH_H
#define TE_MESH_H

#include "types.h"

#include <SDL_opengl.h>

namespace te
{
	template <typename Vec>
	struct Mesh {
		Vertex_array<Vec> vertices;
		GLuint texture_id;
		GLenum mode;
	};

	template <typename Vec>
	void draw(const Mesh<Vec>& mesh)
	{
		glBindTexture(GL_TEXTURE_2D, mesh.texture_id);
		glBegin(mesh.mode);
		for (auto vertex : mesh.vertices) {
			glTexCoord2f(vertex.tex_coords.x, vertex.tex_coords.y);
			glVertex2f(vertex.position.x, vertex.position.y);
		}
		glEnd();
	}
}

#endif
