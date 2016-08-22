#include "physics_manager.h"
#include <Box2D/Box2D.h>
#include <cassert>

namespace te {

class Physics_manager::Listener : public b2ContactListener {
	void BeginContact(b2Contact* p_contact) override
	{
	}
	void EndContact(b2Contact*) override
	{
	}
};

Physics_manager::Physics_manager(Game_data& data)
	: m_game_data{ data }
	, mp_listener{ std::make_unique<Physics_manager::Listener>() }
	, mp_world{ std::make_unique<b2World>(b2Vec2{ 0, 0 }) }
	, m_id_list{}
	, m_rigid_bodies{}
{
	mp_world->SetContactListener(mp_listener.get());
}

Physics_manager::~Physics_manager() = default;

void Physics_manager::step(float dt, int velocity_iterations, int position_iterations)
{
	mp_world->Step(dt, velocity_iterations, position_iterations);
}

b2Body& Physics_manager::add_rigid_body(Entity_id entity_id, const b2BodyDef& body_def)
{
	decltype(m_rigid_bodies)::mapped_type up_rigid_body{ mp_world->CreateBody(&body_def), { *mp_world } };
	auto inserted = m_rigid_bodies.insert(decltype(m_rigid_bodies)::value_type{
		entity_id,
		std::move(up_rigid_body)
	});
	assert(inserted.second);
	m_id_list.push_back(entity_id);
	auto& body = *inserted.first->second;
	body.SetUserData(&m_id_list.back());
	return body;
}

Physics_manager::Body_deleter::Body_deleter(b2World& world)
	: p_world{ &world }
{}

void Physics_manager::Body_deleter::operator()(b2Body* p_body) const
{
	p_world->DestroyBody(p_body);
}

} // namespace te
