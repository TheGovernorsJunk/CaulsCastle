#include "light_attack_state.h"
#include "game_data.h"

namespace te {

void Light_attack_state_table::step_entering(Record_type& record, Game_data& data, float dt)
{
	const auto& animation_group = data.entity_animation_groups[record.id];
	auto& animation = data.entity_animations2[record.id];
	auto heading = data.headings[record.id];

	animation_group.lookup_table.get(Entity_animation::Query(
		true,
		{},
		{},
		std::abs(heading.x) > std::abs(heading.y),
		heading.x > 0,
		heading.y > 0), animation.id);
	animation.frame_index = 0;
	animation.t = 0;

	auto duration_found = m_animation_durations.find(animation.id);
	if (duration_found == m_animation_durations.end()) {
		auto pair = m_animation_durations.insert(decltype(m_animation_durations)::value_type{
			animation.id,
			get_duration(data.animations2.get(animation.id))
		});
		duration_found = pair.first;
	}
	record.max_duration = duration_found->second;
	record.duration = 0.f;

	data.speeds[record.id] = 0;
}

void Light_attack_state_table::step_records(Record_type& record, Game_data& data, float dt)
{
	Resource_id<Mesh2> mesh_id{};
	for (const auto& mesh_pair : data.entity_meshes2) {
		if (record.id == mesh_pair.first) {
			mesh_id = mesh_pair.second.resource_id;
		}
	}
	for (const auto& collider : data.colliders) {
		if (collider.mesh_id == mesh_id) {
			b2AABB aabb{};
			auto position = data.positions[record.id];
			auto scale = data.pixel_to_world_scale;
			aabb.lowerBound = b2Vec2(position.x + collider.x / scale.x,
						 position.y + collider.y / scale.y);
			aabb.upperBound = b2Vec2(aabb.lowerBound.x + collider.w / scale.x,
						 aabb.lowerBound.y + collider.h / scale.y);
			data.attack_queries.push_back({ record.id, aabb, data.stats[record.id].strength });
		}
	}

	record.duration += dt;
	if (record.duration >= record.max_duration) {
		exit_state(record);
		data.normal_state_table.insert(record.id);
	}
}

} // namespace te
