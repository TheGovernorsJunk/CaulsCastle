#ifndef TE_ENTITY_H
#define TE_ENTITY_H

#include "types.h"
#include <string>
#include <vector>

namespace te {

struct Entity_xml {
	std::string name;
	std::string animation_group;
	float max_speed;
	std::string initial_state;

	std::string rigid_body_type;
	struct Rect_fixture {
		float half_width;
		float half_height;
		bool is_hitbox;
	};
	std::vector<Rect_fixture> rect_fixtures;

	Entity_xml(const std::string& filename);
};

struct Game_data;
void load_entity_xml(const std::string& filename, Game_data& data);
Entity_id make_entity(const Entity_xml& entity_xml, Game_data& data, vec2 position = {});

void load_entities_xml(const std::string& entity_listing_filename, Game_data& data);

} // namespace te

#endif
