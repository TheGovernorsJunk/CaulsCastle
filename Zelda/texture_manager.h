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
		static std::unique_ptr<TextureManager> make();

		std::shared_ptr<sf::Texture> get(const std::string& file);
	private:
		TextureManager();

		TextureManager(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;

		std::map<const std::string, std::shared_ptr<sf::Texture>> mTextures;
	};
}

#endif
