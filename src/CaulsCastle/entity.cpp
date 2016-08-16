#include "entity.h"
#include "game_data.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

namespace te {

Entity_xml::Entity_xml(const std::string& filename)
{
	rapidxml::file<> file{ filename.c_str() };
	rapidxml::xml_document<> xml;
	xml.parse<0>(file.data());
	auto* p_root = xml.first_node("entity");

	name = p_root->first_attribute("name")->value();
	animation_group = p_root->first_node("animationgroup")->first_attribute("name")->value();
}

void load_entity_xml(const std::string& filename, Game_data& data)
{
	Entity_xml entity_xml{ filename };
	std::string entity_name{ entity_xml.name };
	data.entity_table.insert(decltype(data.entity_table)::value_type({
		entity_name,
		std::move(entity_xml)
	}));
}

Entity_id make_entity(const Entity_xml& entity_xml, Game_data& data)
{
	auto entity_id = data.entity_manager.get_free_id();

	data.entity_animation_groups.insert(decltype(data.entity_animation_groups)::value_type{
		entity_id,
		Entity_animation{ entity_xml.animation_group, data }
	});
	auto group_found = data.entity_animation_groups.find(entity_id);
	assert(group_found != data.entity_animation_groups.end());
	set_animation(data,
		      entity_id,
		      group_found->second.idle_down);

	return entity_id;
}

} // namespace te
