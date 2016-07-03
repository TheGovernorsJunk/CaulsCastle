#include "texture_atlas.h"
#include "texture_manager.h"
#include "utilities.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#include <algorithm>
#include <regex>

namespace te
{
	const static std::regex animationEx{"(?:.*/)*([0-9]+)-([a-z]+)\\.png", std::regex::icase};

	TextureAtlas::TextureAtlas()
		: mWidth(0)
		, mHeight(0)
		, mImagePath("")
		, mSprites()
	{}

	bool TextureAtlas::loadFromFile(const std::string& filename)
	{
		rapidxml::file<> atlasFile(filename.c_str());
		rapidxml::xml_document<> atlasXML;
		atlasXML.parse<0>(atlasFile.data());

		rapidxml::xml_node<char>* pAtlasNode = atlasXML.first_node("TextureAtlas");

		std::string dir = getDir(filename);

		mWidth = std::stoi(pAtlasNode->first_attribute("width")->value());
		mHeight = std::stoi(pAtlasNode->first_attribute("height")->value());
		std::string imagePath = dir + pAtlasNode->first_attribute("imagePath")->value();
		//if (pTM) pTM->load(imagePath);
		mImagePath = imagePath;

		for (auto* pSprite = pAtlasNode->first_node("sprite"); pSprite != 0; pSprite = pSprite->next_sibling("sprite"))
		{
			std::string filename(pSprite->first_attribute("n")->value());

			bool r = false;
			auto* pR = pSprite->first_attribute("r");
			if (pR && std::string(pR->value()) == "y")
			{
				r = true;
			}

			mSprites.insert(std::make_pair(filename, Sprite{
				std::stof(pSprite->first_attribute("pX")->value()),
				std::stof(pSprite->first_attribute("pY")->value()),
				std::stoi(pSprite->first_attribute("w")->value()),
				std::stoi(pSprite->first_attribute("h")->value()),
				std::stoi(pSprite->first_attribute("x")->value()),
				std::stoi(pSprite->first_attribute("y")->value()),
				filename,
				r
			}));
		}

		return true;
	}

	TextureAtlas::Sprite TextureAtlas::getSprite(const std::string& name) const
	{
		return mSprites.at(name);
	}
}
