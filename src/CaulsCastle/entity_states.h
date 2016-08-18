#ifndef TE_ENTITY_STATES_H
#define TE_ENTITY_STATES_H

#include "types.h"
#include <vector>
#include <cassert>

namespace te {

struct Game_data;

template <class Derived_table, class Record = Entity_id>
class State_table {
public:
	using Record_type = Record;

	virtual ~State_table() = default;

	template <typename R = Record_type>
	void insert(R&& record)
	{
		m_entering_records.push_back(std::forward<R>(record));
	}
	void step(Game_data& data, float dt)
	{
		for (auto& record : m_entering_records) {
			static_cast<Derived_table*>(this)->step_entering(record, data, dt);
		}
		m_records.insert(m_records.end(), m_entering_records.begin(), m_entering_records.end());
		m_entering_records.clear();

		for (auto& record : m_records) {
			static_cast<Derived_table*>(this)->step_records(record, data, dt);
		}

		if (m_pending_removals.size() > 0) {
			auto remove = std::remove_if(m_records.begin(),
						     m_records.end(),
						     [this](const auto& removal_record) {
				return std::find(m_pending_removals.begin(),
						 m_pending_removals.end(),
						 removal_record) != m_pending_removals.end();
			});
			assert(remove != m_records.end());
			m_records.erase(remove, m_records.end());
			m_pending_removals.clear();
		}

		for (auto& record : m_records) {
			static_cast<Derived_table*>(this)->step_exiting(record, data, dt);
		}
		m_exiting_records.clear();
	}
protected:
	void exit_state(const Record_type& record)
	{
		m_pending_removals.push_back(record);
		m_exiting_records.push_back(record);
	}
private:
	friend Derived_table;

	inline void step_entering(const Record_type&, Game_data&, float) {}
	inline void step_exiting(const Record_type&, Game_data&, float) {}

	std::vector<Record_type> m_entering_records;
	std::vector<Record_type> m_records;
	std::vector<Record_type> m_pending_removals;
	std::vector<Record_type> m_exiting_records;
};

} // namespace te

#endif
