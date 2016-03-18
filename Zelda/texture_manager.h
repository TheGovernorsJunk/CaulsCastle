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
		std::shared_ptr<sf::Texture> get(const std::string& file);
	private:
		std::map<const std::string, std::shared_ptr<sf::Texture>> mTextures;
	};
}

#endif
