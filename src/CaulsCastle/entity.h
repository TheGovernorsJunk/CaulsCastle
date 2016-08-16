#ifndef TE_ENTITY_H
#define TE_ENTITY_H

#include <string>
#include <vector>

namespace te {

struct Entity_xml {
	std::string name;
	std::string animation_group;

	Entity_xml(const std::string& filename);
};

struct Game_data;
void load_entity_xml(const std::string& filename, Game_data& data);

} // namespace te

#endif
