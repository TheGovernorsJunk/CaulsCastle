#ifndef TE_GAME_DATA_H
#define TE_GAME_DATA_H

#include "types.h"
#include "input.h"
#include "mesh.h"
#include "texture.h"
#include "resource_holder.h"
#include "animation.h"
#include "records.h"
#include "entity.h"
#include "entity_animation.h"
#include "normal_state.h"
#include "light_attack_state.h"
#include "mappings.h"
#include "physics_manager.h"
#include "collider.h"

#include <Box2D/Box2D.h>
#include <boost/container/flat_map.hpp>

#include <memory>
#include <vector>
#include <type_traits>
#include <iterator>

namespace te {

class Entity_manager {
public:
	Entity_manager();
	Entity_id get_free_id();
private:
	Entity_id m_next_id;
};

struct Game_data {
	std::string image_root;
	flat_map<std::string, Image_record> image_table;
	flat_map<std::string, Sprite_record> sprite_table;
	flat_map<std::string, Animation_record> animation_table;
	std::vector<Animation_frame_record> animation_frame_table;
	flat_map<std::string, Animation_group_record> animation_group_table;
	std::vector<Animation_group_type_record> animation_group_type_table;
	std::vector<Collider_record> collider_table;
	flat_map<std::string, Entity_xml> entity_table;

	flat_map<std::string, Resource_record<Texture>> texture_table;
	flat_map<std::string, Resource_record<Mesh2>> mesh2_table;
	flat_map<std::string, Resource_record<Animation2>> animation2_table;

	std::vector<Collider> colliders;

	vec2 pixel_to_world_scale;
	vec2 resolution;

	Physics_manager physics_manager;

	Resource_holder<Texture> textures;
	Resource_holder<Mesh2> meshes2;
	Resource_holder<Mesh3> meshes3;
	Resource_holder<Animation2> animations2;

	flat_map<int, Team_mask> team_masks;

	Entity_manager entity_manager;

	flat_map<Player_id, std::unique_ptr<SDL_GameController, decltype(&SDL_GameControllerClose)>> controllers;
	flat_map<Player_id, Controllermap> controllermaps;
	flat_map<Player_id, Keymap> keymaps;
	flat_map<Player_id, Player_input> inputs;
	flat_map<Player_id, Entity_id> avatars;

	component<Entity_id, float> max_speeds;
	component<Entity_id, float> speeds;
	component<Entity_id, vec2> headings;
	component<Entity_id, vec2> positions;
	component<Entity_id, Team_mask> entity_team_masks;

	struct Attack_data {
		struct Attack {
			int damage;
		};
		Attack light_attack;
	};
	component<Entity_id, Attack_data> entity_attacks;

	component<Entity_id, Entity_animation> entity_animation_groups;

	Normal_state_table normal_state_table;
	Light_attack_state_table light_attack_state_table;

	struct Attack_query {
		Entity_id entity_id;
		b2AABB aabb;
		int damage;
	};
	std::vector<Attack_query> attack_queries;
	struct Pending_hits {
		Entity_id entity_id;
		int damage;
	};
	std::vector<Pending_hits> pending_hits;

	template <typename Mesh>
	struct Animation_data {
		using Id = Resource_id<Animation<Mesh>>;

		Resource_id<Animation<Mesh>> id;
		size_t frame_index;
		float t;

		Animation_data(Id id)
			: id{ id }
			, frame_index{ 0 }
			, t{ 0.f }
		{}
		Animation_data() = default;
	};
	component<Entity_id, Animation_data<Mesh2>> entity_animations2;

	template <typename Resource>
	struct Render_data {
		Resource_id<Resource> resource_id;
		glm::mat4 transform;
		int draw_order;
	};
	Multi_component<Entity_id, Render_data<Mesh2>> entity_meshes2;
	Multi_component<Entity_id, Render_data<Mesh3>> entity_meshes3;

	glm::mat4 view_matrix;

	Game_data();
	~Game_data();
};

void set_animation(Game_data& data, Entity_id entity_id, Resource_id<Animation2>);

template <typename Resource>
Resource_id<Resource> get_or_create(const std::string& name, Game_data& data)
{
	auto& resource_table = detail::get_resource_table<Resource>(data);
	auto resource_found = resource_table.find(name);
	if (resource_found != resource_table.end()) {
		return resource_found->second.id;
	}

	auto data_table = detail::get_data_table<Resource>(data);
	auto data_found = data_table.find(name);
	assert(data_found != data_table.end());
	auto resource_id = detail::create_resource<Resource>(data_found->second, data);
	resource_table.insert(std::remove_reference_t<decltype(resource_table)>::value_type{
		name, {
			name,
			resource_id
		}
	});
	return resource_id;
}

namespace detail {

template <typename Resource>
inline auto& get_resource_table(Game_data& data);
template <>
inline auto& get_resource_table<Texture>(Game_data& data)
{
	return data.texture_table;
}
template <>
inline auto& get_resource_table<Mesh2>(Game_data& data)
{
	return data.mesh2_table;
}
template <>
inline auto& get_resource_table<Animation2>(Game_data& data)
{
	return data.animation2_table;
}

template <typename Resource>
inline auto& get_data_table(Game_data& data);
template <>
inline auto& get_data_table<Texture>(Game_data& data)
{
	return data.image_table;
}
template <>
inline auto& get_data_table<Mesh2>(Game_data& data)
{
	return data.sprite_table;
}
template <>
inline auto& get_data_table<Animation2>(Game_data& data)
{
	return data.animation_table;
}

template <typename Resource, typename Record>
inline Resource_id<Resource> create_resource(const Record& record, Game_data& data);
template <>
inline Resource_id<Texture> create_resource(const Image_record& record, Game_data& data)
{
	return data.textures.insert({ load_texture32(data.image_root + record.filename) });
}
template <>
inline Resource_id<Mesh2> create_resource(const Sprite_record& record, Game_data& data)
{
	auto texture_id = get_or_create<Texture>(record.image_filename, data);
	auto mesh_id = data.meshes2.insert(make_mesh(record, data.textures.get(texture_id).get_texture_id()));

	std::vector<decltype(data.collider_table)::value_type> colliders{};
	std::for_each(data.collider_table.begin(), data.collider_table.end(), [record, mesh_id, &data](const auto& collider_record) {
		if (collider_record.image_name == record.filename) {
			vec2 origin{ record.w * record.px, record.h * record.py };
			data.colliders.push_back({ mesh_id, collider_record.x - (int)origin.x, collider_record.y - (int)origin.y, collider_record.w, collider_record.h });
		}
	});

	return mesh_id;
}
template <>
inline Resource_id<Animation2> create_resource(const Animation_record& record, Game_data& data)
{
	std::vector<decltype(Game_data::animation_frame_table)::value_type> frames;
	std::copy_if(data.animation_frame_table.begin(), data.animation_frame_table.end(), std::back_inserter(frames), [&record](auto& frame_record) {
		return frame_record.animation_filename == record.filename;
	});
	assert(frames.size() > 0);
	Animation<Mesh2> animation{ {}, frames[0].delay_unit };
	for (auto& frame : frames) {
		auto mesh_id = get_or_create<Mesh2>(frame.sprite_filename, data);
		animation.frames.push_back({ frame.delay, mesh_id });
	}
	return data.animations2.insert(std::move(animation));
}

} // namespace detail

} // namespace te

#endif
