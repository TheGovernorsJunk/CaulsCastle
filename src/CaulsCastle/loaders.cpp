#include "loaders.h"
#include "records.h"
#include <iterator>

namespace te {

void load_image_data(const std::string& data_filename, Game_data& data)
{
	auto dir = get_directory(data_filename);

	rapidxml::file<> file{ data_filename.c_str() };
	rapidxml::xml_document<> xml;
	xml.parse<0>(file.data());
	auto* p_root = xml.first_node("image_data");

	for (auto* p_atlas = p_root->first_node("atlases")->first_node("file"); p_atlas != NULL; p_atlas = p_atlas->next_sibling("file")) {
		load_atlas(dir + p_atlas->first_attribute("name")->value(),
			   std::inserter(data.image_table, data.image_table.end()),
			   std::inserter(data.sprite_table, data.sprite_table.end()));
	}
	for (auto* p_animation = p_root->first_node("animations")->first_node("file"); p_animation != NULL; p_animation = p_animation->next_sibling("file")) {
		load_animation(dir + p_animation->first_attribute("name")->value(),
			       std::inserter(data.animation_table, data.animation_table.begin()),
			       std::back_inserter(data.animation_sprite_table));
	}
}

} // namespace te
