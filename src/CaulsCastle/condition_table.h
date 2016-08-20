#ifndef TE_CONDITION_TABLE_H
#define TE_CONDITION_TABLE_H

#include <vector>

namespace te {

template <typename Query, typename Out>
class Condition_table {
public:
	Condition_table() = default;
	Condition_table(std::initializer_list<std::pair<Query, Out>> pair_list)
	{
		assign_records(std::move(pair_list));
	}

	void assign_records(std::initializer_list<std::pair<Query, Out>> pair_list)
	{
		for (auto& pair : pair_list) {
			m_query_records.push_back(std::move(pair.first));
			m_output_records.push_back(std::move(pair.second));
		}
	}

	template <typename Iter>
	void get(const Query& query, Iter out)
	{
		size_t i = 0;
		for (auto& record : m_query_records) {
			if (record == query) {
				out++ = m_output_records[i];
			}
			++i;
		}
	}

	const Out& get(const Query& query) const
	{
		size_t i = 0;
		for (auto& record : m_query_records) {
			if (record == query) {
				return m_output_records[i];
			}
			++i;
		}
		assert(false && "No record found for query.");
	}

private:
	std::vector<Query> m_query_records;
	std::vector<Out> m_output_records;
};

} // namespace te

#endif
