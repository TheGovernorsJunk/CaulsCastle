#include "texture_manager.h"

namespace te
{
	std::shared_ptr<sf::Texture> TextureManager::get(const std::string& file)
	{
		auto iter = mTextures.find(file);
		if (iter != mTextures.end())
		{
			return iter->second;
		}
		else
		{
			auto texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile(file))
			{
				throw std::runtime_error("File not found.");
			}
			mTextures.insert(std::make_pair(file, texture));
			return texture;
		}
	}
}
