#include "texture_atlas.h"
#include "texture_manager.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

namespace te
{

	std::unique_ptr<TextureAtlas> TextureAtlas::make(const std::string& dir, const std::string& filename, TextureManager* pTM)
	{
		return std::unique_ptr<TextureAtlas>(new TextureAtlas(dir, filename, pTM));
	}

	TextureAtlas::TextureAtlas(const std::string& dir, const std::string& filename, TextureManager* pTM)
		: mWidth(0)
		, mHeight(0)
		, mImagePathID(0)
		, mSprites()
	{
		rapidxml::file<> atlasFile((dir + "/" + filename).c_str());
		rapidxml::xml_document<> atlasXML;
		atlasXML.parse<0>(atlasFile.data());

		rapidxml::xml_node<char>* pAtlasNode = atlasXML.first_node("TextureAtlas");

		mWidth = std::stoi(pAtlasNode->first_attribute("width")->value());
		mHeight = std::stoi(pAtlasNode->first_attribute("height")->value());
		std::string imagePath = dir + "/" + pAtlasNode->first_attribute("imagePath")->value();
		if (pTM) pTM->load(imagePath);
		mImagePathID = TextureManager::getID(imagePath);

		for (auto* pSprite = pAtlasNode->first_node("sprite"); pSprite != 0; pSprite = pSprite->next_sibling("sprite"))
		{
			size_t filenameHash = TextureManager::getID(dir + "/" + std::string(pSprite->first_attribute("n")->value()));
			mSprites.insert(std::make_pair(filenameHash, Sprite{
				std::stof(pSprite->first_attribute("pX")->value()),
				std::stof(pSprite->first_attribute("pY")->value()),
				std::stoi(pSprite->first_attribute("w")->value()),
				std::stoi(pSprite->first_attribute("h")->value()),
				std::stoi(pSprite->first_attribute("x")->value()),
				std::stoi(pSprite->first_attribute("y")->value()),
				filenameHash
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
