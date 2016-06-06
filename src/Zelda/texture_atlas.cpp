#include "texture_atlas.h"
#include "texture_manager.h"
#include "utilities.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#include <algorithm>
#include <regex>
#include <iostream>

namespace te
{

	std::unique_ptr<TextureAtlas> TextureAtlas::make(const std::string& filename, TextureManager* pTM)
	{
		return std::unique_ptr<TextureAtlas>(new TextureAtlas(filename, pTM));
	}

	const static std::regex animationEx{"([0-9]+)-([a-z]+)\\.png", std::regex::icase};

	TextureAtlas::TextureAtlas(const std::string& filename, TextureManager* pTM)
		: mWidth(0)
		, mHeight(0)
		, mImagePathID(0)
		, mSprites()
		, mAnimations()
	{
		rapidxml::file<> atlasFile(filename.c_str());
		rapidxml::xml_document<> atlasXML;
		atlasXML.parse<0>(atlasFile.data());

		rapidxml::xml_node<char>* pAtlasNode = atlasXML.first_node("TextureAtlas");

		std::string dir = getDir(filename);

		mWidth = std::stoi(pAtlasNode->first_attribute("width")->value());
		mHeight = std::stoi(pAtlasNode->first_attribute("height")->value());
		std::string imagePath = dir + pAtlasNode->first_attribute("imagePath")->value();
		if (pTM) pTM->load(imagePath);
		mImagePathID = TextureManager::getID(imagePath);

		for (auto* pSprite = pAtlasNode->first_node("sprite"); pSprite != 0; pSprite = pSprite->next_sibling("sprite"))
		{
			std::string filename(pSprite->first_attribute("n")->value());
			TextureID spriteID = TextureManager::getID(filename);

			std::smatch match;
			if (std::regex_match(filename, match, animationEx))
			{
				int index = std::stoi(match[1]);
				TextureID animationID = TextureManager::getID(match[2]);
				Animation& animation = mAnimations[animationID];
				animation.id = animationID;
				animation.sprites.insert(std::make_pair(index, spriteID));
			}

			mSprites.insert(std::make_pair(spriteID, Sprite{
				std::stof(pSprite->first_attribute("pX")->value()),
				std::stof(pSprite->first_attribute("pY")->value()),
				std::stoi(pSprite->first_attribute("w")->value()),
				std::stoi(pSprite->first_attribute("h")->value()),
				std::stoi(pSprite->first_attribute("x")->value()),
				std::stoi(pSprite->first_attribute("y")->value()),
				spriteID
			}));
		}
	}

	TextureID TextureAtlas::getTextureID() const
	{
		return mImagePathID;
	}

	TextureAtlas::Sprite TextureAtlas::getSprite(TextureID hash) const
	{
		return mSprites.at(hash);
	}

	size_t TextureAtlas::getSpriteCount() const
	{
		return mSprites.size();
	}
}
