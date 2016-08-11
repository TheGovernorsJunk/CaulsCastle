#include "level.h"
#include "game_data.h"
#include "tmx.h"
#include "texture.h"
#include "tile_map_layer.h"

#include <Box2D/Box2D.h>
#include <glm/gtx/transform.hpp>

#include <iterator>
#include <array>

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
			Vertex_array<vec3, vec2> vertices{};
			get_tile_map_layer_vertices(tmx, layer_i, tileset_i, std::back_inserter(vertices));
			auto id = data.meshes3.insert({
				std::move(vertices),
				tileset_texture_ids[tileset_i],
				GL_QUADS
			});
			data.entity_meshes3.push_back({ map_id, { id, glm::scale(vec3{ 1.f / data.pixel_to_world_scale.x, 1.f / data.pixel_to_world_scale.y, 1 }) } });
		});

		for (auto& group : tmx.objectgroups) {
			if (group.name == "Collisions") {
				assert(data.rigid_bodies.find(map_id) == data.rigid_bodies.end());

				b2BodyDef body_def{};
				body_def.type = b2_staticBody;
				decltype(Game_data::rigid_bodies)::mapped_type p_body{
					data.physics_world->CreateBody(&body_def),
					{ *data.physics_world }
				};

				for (auto& polygon : group.objects) {
					b2PolygonShape shape{};
					const std::array<b2Vec2, 4> points = {
						b2Vec2{ (float)polygon.x, (float)polygon.y },
						b2Vec2{ (float)polygon.x + polygon.width, (float)polygon.y },
						b2Vec2{ (float)polygon.x + polygon.width, (float)polygon.y + polygon.height },
						b2Vec2{ (float)polygon.x, (float)polygon.y + polygon.height }
					};
					shape.Set(points.data(), 4);
					p_body->CreateFixture(&shape, 0);
				}
			}
		}
	}
}
