#include "texture_manager.h"

namespace te
{
	std::unique_ptr<TextureManager> TextureManager::make()
	{
		return std::unique_ptr<TextureManager>(new TextureManager);
	}

	TextureManager::TextureManager() {}

	static const std::hash<std::string> hashFn;
	TextureID TextureManager::getID(const std::string& filename)
	{
		return hashFn(filename);
	}

	TextureID TextureManager::load(const std::string& filename)
	{
		TextureID id = getID(filename);
		auto iter = mTextures.find(id);
		if (iter == mTextures.end())
		{
			auto texture = std::make_unique<sf::Texture>();
			if (!texture->loadFromFile(filename))
			{
				throw std::runtime_error("Texture file not found.");
			}
			mTextures.insert({ id, std::move(texture) });
			return id;
		}
		else
		{
			return iter->first;
		}
	}

	sf::Texture& TextureManager::get(TextureID file) const
	{
		auto iter = mTextures.find(file);
		if (iter != mTextures.end())
		{
			return *iter->second;
		}
		throw std::runtime_error("No texture for given ID.");
	}
}
