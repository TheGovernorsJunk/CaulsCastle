#ifndef TE_PHYSICS_MANAGER_H
#define TE_PHYSICS_MANAGER_H

#include "types.h"
#include <boost/container/flat_map.hpp>
#include <memory>
#include <list>

class b2World;
class b2Body;
struct b2BodyDef;

namespace te {

struct Game_data;

class Physics_manager {
public:
	Physics_manager(Game_data& data);
	~Physics_manager();

	void step(float dt, int velocity_iterations = 8, int position_iterations = 3);
	b2Body& add_rigid_body(Entity_id entity_id, const b2BodyDef& body_def);
	auto find_rigid_body(Entity_id id) { return m_rigid_bodies.find(id); }
	auto find_rigid_body(Entity_id id) const { return m_rigid_bodies.find(id); }
	auto begin() noexcept { return m_rigid_bodies.begin(); }
	auto begin() const noexcept { return m_rigid_bodies.begin(); }
	auto end() noexcept { return m_rigid_bodies.end(); }
	auto end() const noexcept { return m_rigid_bodies.end(); }
private:
	struct Body_deleter {
		Body_deleter(b2World& world);
		void operator()(b2Body*) const;
		b2World* p_world;
	};
	class Listener;

	Game_data& m_game_data;
	std::unique_ptr<Listener> mp_listener;
	std::unique_ptr<b2World> mp_world;
	std::list<Entity_id> m_id_list;
	flat_map<Entity_id, std::unique_ptr<b2Body, Body_deleter>> m_rigid_bodies;
};

} // namespace te

#endif
