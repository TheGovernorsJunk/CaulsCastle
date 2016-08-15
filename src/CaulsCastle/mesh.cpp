#include "mesh.h"
#include "records.h"

namespace te {

Mesh2 make_mesh(const Sprite_record& record, GLuint gl_texture_id)
{
	using Vertex = decltype(Mesh2::vertices)::value_type;
	using Position_coord = decltype(Vertex::position_value_type::x);
	using Tex_coord = decltype(Vertex::tex_value_type::x);

	glBindTexture(GL_TEXTURE_2D, gl_texture_id);

	GLint tex_width, tex_height;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tex_width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &tex_height);

	glBindTexture(GL_TEXTURE_2D, NULL);

	decltype(Mesh2::vertices) quad(4);
	quad[0].tex_coords = { (Tex_coord)(record.x) / tex_width,
			       (Tex_coord)(record.y) / tex_height };
	quad[1].tex_coords = { (Tex_coord)(record.x + record.w) / tex_width,
			       (Tex_coord)(record.y) / tex_height };
	quad[2].tex_coords = { (Tex_coord)(record.x + record.w) / tex_width,
			       (Tex_coord)(record.y + record.h) / tex_height };
	quad[3].tex_coords = { (Tex_coord)(record.x) / tex_width,
			       (Tex_coord)(record.y + record.h) / tex_height };

	quad[0].position = { (Position_coord)0,
			     (Position_coord)0 };
	quad[1].position = { (Position_coord)record.w,
			     (Position_coord)0 };
	quad[2].position = { (Position_coord)record.w,
			     (Position_coord)record.h };
	quad[3].position = { (Position_coord)0,
			     (Position_coord)record.h };

	return { std::move(quad), gl_texture_id, GL_QUADS };
}

} // namespace te
