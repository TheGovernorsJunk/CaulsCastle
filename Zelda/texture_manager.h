#ifndef TE_TEXTURE_MANAGER_H
#define TE_TEXTURE_MANAGER_H

#include <SFML/Graphics.hpp>

#include <memory>
#include <map>

namespace te
{
	class TextureManager
	{
	public:
		TextureManager();

		std::shared_ptr<sf::Texture> get(const std::string& file);
	private:
		TextureManager(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;

		std::map<const std::string, std::shared_ptr<sf::Texture>> mTextures;
	};
}

#endif
