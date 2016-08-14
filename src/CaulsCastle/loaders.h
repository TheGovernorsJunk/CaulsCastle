#ifndef TE_LOADERS_H
#define TE_LOADERS_H

#include "resource_holder.h"
#include "animation.h"
#include "game_data.h"
#include "utilities.h"
#include "texture_atlas.h"
#include "texture.h"
#include "animation.h"
#include "mesh.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#include <vector>
#include <map>
#include <string>

namespace te {

template <template <typename> typename Iter, typename Map_container>
void load_animations(const std::string& animations_filename, Game_data& data, Iter<Map_container> out)
{
	auto dir = get_directory(animations_filename);

	rapidxml::file<> file{ animations_filename.c_str() };
	rapidxml::xml_document<> xml;
	xml.parse<0>(file.data());

	auto* p_root = xml.first_node("animations");
	Texture_atlas atlas((dir + std::string{ p_root->first_node("texture")->first_attribute("atlas")->value() }).c_str());
	auto gl_texture_id = load_texture32(dir + atlas.imagePath);
	data.textures.insert({ gl_texture_id });

	std::vector<Mesh2> meshes{};
	make_meshes(atlas, gl_texture_id, std::back_inserter(meshes));

	std::map<std::string, Resource_id<Mesh2>> mesh_map{};
	size_t i = 0;
	std::for_each(meshes.begin(), meshes.end(), [i, &atlas, &mesh_map, &data](auto& mesh) mutable {
		using Pair = std::remove_reference_t<decltype(mesh_map)>::value_type;
		mesh_map.insert(Pair{ atlas.sprites[i].n, data.meshes2.insert(std::move(mesh)) });
		++i;
	});

	for (auto* p_animation = p_root->first_node("animation"); p_animation != NULL; p_animation = p_animation->next_sibling("animation")) {
		Animation<Mesh2> animation{};
		Animation_csv csv{ dir + p_animation->first_attribute("filename")->value() };
		animation.delay_unit = csv.delay_unit;
		for (auto& frame : csv.frames) {
			auto id = mesh_map.find(frame.name)->second;
			animation.frames.push_back({ frame.delay, id });
		}
		auto animation_id = data.animations2.insert(std::move(animation));

		std::string animation_name = p_animation->first_attribute("name")->value();
		out++ = Map_container::value_type{ animation_name, animation_id };
	}
}

void load_image_data(const std::string& data_filename, Game_data& data);

}

#endif
