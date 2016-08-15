#ifndef TE_GAME_DATA_H
#define TE_GAME_DATA_H

#include "types.h"
#include "input.h"
#include "mesh.h"
#include "texture.h"
#include "resource_holder.h"
#include "animation.h"
#include "records.h"

#include <boost/container/flat_map.hpp>

#include <memory>
#include <vector>
#include <type_traits>

class b2World;
class b2Body;

namespace te {

struct Keymap {
	SDL_GameControllerAxis x_movement;
	SDL_GameControllerAxis y_movement;

	SDL_GameControllerButton dodge;

	Keymap();
};

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
	flat_map<std::string, Resource_record<Texture>> texture_table;
	flat_map<std::string, Resource_record<Mesh2>> mesh2_table;

	vec2 pixel_to_world_scale;

	std::unique_ptr<b2World> physics_world;

	Resource_holder<Texture> textures;
	Resource_holder<Mesh2> meshes2;
	Resource_holder<Mesh3> meshes3;
	Resource_holder<Animation2> animations2;

	Entity_manager entity_manager;

	flat_map<Player_id, std::unique_ptr<SDL_GameController, decltype(&SDL_GameControllerClose)>> controllers;
	flat_map<Player_id, Keymap> keymaps;
	flat_map<Player_id, Player_input> inputs;
	flat_map<Player_id, Entity_id> avatars;

	struct Body_deleter {
		Body_deleter(b2World& world);
		void operator()(b2Body*) const;
		b2World* p_world;
	};
	component<Entity_id, std::unique_ptr<b2Body, Body_deleter>> rigid_bodies;
	component<Entity_id, float> max_speeds;
	component<Entity_id, vec2> velocities;
	component<Entity_id, vec2> positions;

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
	};
	component<Entity_id, Animation_data<Mesh2>> entity_animations2;

	template <typename Resource>
	struct Render_data {
		Resource_id<Resource> resource_id;
		glm::mat4 transform;
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
	return data.meshes2.insert(make_mesh(record, data.textures.get(texture_id).get_texture_id()));
}

} // namespace detail

} // namespace te

#endif
