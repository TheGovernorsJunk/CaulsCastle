#ifndef TE_ENTITY_STATES_H
#define TE_ENTITY_STATES_H

#include "types.h"
#include <vector>
#include <cassert>

namespace te {

struct Game_data;

template <class Derived_table>
class State_table {
public:
	void insert(Entity_id id)
	{
		m_entering_records.push_back(id);
	}
	void step(Game_data& data, float dt)
	{
		for (auto entity_id : m_entering_records) {
			static_cast<Derived_table*>(this)->step_entering(entity_id, data, dt);
		}
		m_records.insert(m_records.end(), m_entering_records.begin(), m_entering_records.end());
		m_entering_records.clear();

		for (auto entity_id : m_records) {
			static_cast<Derived_table*>(this)->step_records(entity_id, data, dt);
		}

		if (m_pending_removals.size() > 0) {
			auto remove = std::remove_if(m_records.begin(),
						     m_records.end(),
						     [this](auto removal_id) {
				return std::find(m_pending_removals.begin(),
						 m_pending_removals.end(),
						 removal_id) != m_pending_removals.end();
			});
			assert(remove != m_records.end());
			m_records.erase(remove, m_records.end());
			m_pending_removals.clear();
		}

		for (auto entity_id : m_records) {
			static_cast<Derived_table*>(this)->step_exiting(entity_id, data, dt);
		}
		m_exiting_records.clear();
	}
protected:
	void exit_state(Entity_id id)
	{
		m_pending_removals.push_back(id);
		m_exiting_records.push_back(id);
	}
private:
	friend Derived_table;

	inline void step_entering(Entity_id, Game_data&, float) {}
	inline void step_exiting(Entity_id, Game_data&, float) {}

	std::vector<Entity_id> m_entering_records;
	std::vector<Entity_id> m_records;
	std::vector<Entity_id> m_pending_removals;
	std::vector<Entity_id> m_exiting_records;
};

} // namespace te

#endif
