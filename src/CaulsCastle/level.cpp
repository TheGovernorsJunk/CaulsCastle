#include "level.h"
#include "game_data.h"
#include "tmx.h"
#include "texture.h"
#include "tile_map_layer.h"

#include <iterator>

namespace te
{
	void load_level(const std::string& tmx_filename, Game_data& data)
	{
		Tmx tmx{ tmx_filename };

		std::vector<GLuint> tileset_texture_ids;
		load_tileset_textures(tmx, std::back_inserter(tileset_texture_ids));
		for (auto id : tileset_texture_ids) {
			data.textures.emplace_back(id);
		}

		auto map_id = data.entity_manager.get_free_id();
		iterate_layers_and_tilesets(tmx, [map_id, &data, &tmx, &tileset_texture_ids](size_t layer_i, size_t tileset_i) {
			Vertex_array<vec2, vec2> vertices{};
			get_tile_map_layer_vertices(tmx, layer_i, tileset_i, std::back_inserter(vertices));
			data.meshes.push_back({
				map_id,
				{
					std::move(vertices),
					tileset_texture_ids[tileset_i],
					GL_QUADS
				}
			});
		});
	}
}
