#include "game_data.h"

#include <glm/gtx/transform.hpp>
#include <Box2D/Box2D.h>

namespace te {

Entity_manager::Entity_manager()
	: m_next_id{ 1 }
{}

Entity_id Entity_manager::get_free_id()
{
	return m_next_id++;
}

Game_data::Game_data()
	: physics_world{ std::make_unique<b2World>(b2Vec2{ 0, 0 }) }
{}
Game_data::~Game_data() = default;

Game_data::Body_deleter::Body_deleter(b2World& world)
	: p_world{ &world }
{}

void Game_data::Body_deleter::operator()(b2Body* p_body) const
{
	p_world->DestroyBody(p_body);
}

void set_animation(Game_data& data, Entity_id entity_id, Resource_id<Animation2> animation_id)
{
	Game_data::Animation_data<Mesh2> animation_data{ animation_id };
	auto found = std::find_if(data.entity_animations2.begin(), data.entity_animations2.end(), [entity_id](auto& pair) {
		return pair.first == entity_id;
	});
	if (found == data.entity_animations2.end()) {
		data.entity_animations2.insert(decltype(data.entity_animations2)::value_type{ entity_id, std::move(animation_data) });
	}
	else {
		found->second = std::move(animation_data);
	}

	auto mesh_id = data.animations2.get(animation_id).frames[0].mesh_id;
	auto mesh_found = std::find_if(data.entity_meshes2.begin(), data.entity_meshes2.end(), [entity_id](auto& mesh) {
		return mesh.first == entity_id;
	});
	if (mesh_found == data.entity_meshes2.end()) {
		data.entity_meshes2.push_back({ entity_id, { mesh_id } });
	}
	else {
		mesh_found->second.resource_id = mesh_id;
	}
}

} // namespace te
