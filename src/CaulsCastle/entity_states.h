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
		static_cast<Derived_table*>(this)->step_entering(data, dt);
		m_records.insert(m_records.end(), m_entering_records.begin(), m_entering_records.end());
		m_entering_records.clear();

		for (auto entity_id : m_records) {
			static_cast<Derived_table*>(this)->step_records(entity_id, data, dt);
		}

		static_cast<Derived_table*>(this)->step_exiting(data, dt);
		m_exiting_records.clear();
	}
protected:
	void exit(Entity_id id)
	{
		auto remove = std::remove(m_records.begin(),
					  m_records.end(),
					  id);
		assert(remove != m_records.end());
		m_records.erase(remove, m_records.end());
		m_exiting_records.push_back(id);
	}
private:
	friend Derived_table;

	inline void step_entering(Game_data&, float) {}
	inline void step_exiting(Game_data&, float) {}

	std::vector<Entity_id> m_entering_records;
	std::vector<Entity_id> m_records;
	std::vector<Entity_id> m_exiting_records;
};

} // namespace te

#endif
