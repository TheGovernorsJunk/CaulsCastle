#include "texture_atlas.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

namespace te {

Texture_atlas::Texture_atlas(const char* path)
{
	rapidxml::file<> file(path);
	rapidxml::xml_document<> atlas_xml;
	atlas_xml.parse<0>(file.data());
	auto atlas_node = atlas_xml.first_node("TextureAtlas");

	imagePath = atlas_node->first_attribute("imagePath")->value();
	width = std::stoi(atlas_node->first_attribute("width")->value());
	height = std::stoi(atlas_node->first_attribute("height")->value());

	for (auto* sprite_node = atlas_node->first_node("sprite"); sprite_node != 0; sprite_node = sprite_node->next_sibling("sprite")) {
		sprites.push_back({
			sprite_node->first_attribute("n")->value(),
			std::stoi(sprite_node->first_attribute("x")->value()),
			std::stoi(sprite_node->first_attribute("y")->value()),
			std::stoi(sprite_node->first_attribute("w")->value()),
			std::stoi(sprite_node->first_attribute("h")->value()),
			std::stof(sprite_node->first_attribute("pX")->value()),
			std::stof(sprite_node->first_attribute("pY")->value())
		});
	}
}

} // namespace te
