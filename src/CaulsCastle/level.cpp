#include "level.h"
#include "game_data.h"
#include "tmx.h"
#include "texture.h"
#include "tile_map_layer.h"
#include "entity.h"

#include <Box2D/Box2D.h>
#include <glm/gtx/transform.hpp>

#include <iterator>
#include <array>

namespace te {

void load_level(const std::string& tmx_filename, Game_data& data)
{
	Tmx tmx{ tmx_filename };

	std::vector<GLuint> tileset_texture_ids;
	load_tileset_textures(tmx, std::back_inserter(tileset_texture_ids));
	for (auto id : tileset_texture_ids) {
		data.textures.insert({ id });
	}

	auto map_id = data.entity_manager.get_free_id();
	iterate_layers_and_tilesets(tmx, [map_id, &data, &tmx, &tileset_texture_ids](size_t layer_i, size_t tileset_i) {
		Vertex_array<vec2, vec2> vertices{};
		get_tile_map_layer_vertices(tmx, layer_i, tileset_i, std::back_inserter(vertices));
		auto id = data.meshes2.insert({
			std::move(vertices),
			tileset_texture_ids[tileset_i],
			GL_QUADS
		});
		data.entity_meshes2.push_back({ map_id, { id, {}, static_cast<int>(layer_i) } });
	});

	for (auto& group : tmx.objectgroups) {
		if (group.name == "Collisions") {
			assert(data.physics_manager.find_rigid_body(map_id) == data.physics_manager.end());

			b2BodyDef body_def{};
			body_def.type = b2_staticBody;
			b2Body& p_body = data.physics_manager.add_rigid_body(map_id, body_def);

			for (auto& polygon : group.objects) {
				b2PolygonShape shape{};
				const std::array<b2Vec2, 4> points = {
					b2Vec2{ polygon.x / data.pixel_to_world_scale.x, polygon.y / data.pixel_to_world_scale.y },
					b2Vec2{ (polygon.x + polygon.width) / data.pixel_to_world_scale.x, polygon.y / data.pixel_to_world_scale.y },
					b2Vec2{ (polygon.x + polygon.width) / data.pixel_to_world_scale.x, (polygon.y + polygon.height) / data.pixel_to_world_scale.y },
					b2Vec2{ polygon.x / data.pixel_to_world_scale.x, (polygon.y + polygon.height) / data.pixel_to_world_scale.y }
				};
				shape.Set(points.data(), 4);
				p_body.CreateFixture(&shape, 0);
			}
		}

		if (group.name == "Entities") {
			for (auto& entity : group.objects) {
				const std::string type = entity.type.length() > 0 ? entity.type : "hero";
				auto entity_found = data.entity_table.find(type);
				assert(entity_found != data.entity_table.end());
				auto entity_id = make_entity(entity_found->second, data, {
					(entity.x + (entity.width * 0.5f)) / data.pixel_to_world_scale.x,
					(entity.y + (entity.height * 0.5f)) / data.pixel_to_world_scale.y
				});
				for (auto& mesh_pair : data.entity_meshes2) {
					if (mesh_pair.first == entity_id) {
						mesh_pair.second.draw_order = group.index;
					}
				}

				if (entity.name == "Player") {
					data.avatars[0] = entity_id;
				}
			}
		}
	}
}

} // namespace te
