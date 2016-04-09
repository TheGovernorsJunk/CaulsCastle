#ifndef TE_TEXTURE_MANAGER_H
#define TE_TEXTURE_MANAGER_H

#include "typedefs.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <map>

namespace te
{
	class TextureManager
	{
	public:
		static std::unique_ptr<TextureManager> make();

		static TextureID getID(const std::string& filename);

		TextureID load(const std::string& filename);
		std::shared_ptr<sf::Texture> get(TextureID file) const;
	private:
		TextureManager();

		TextureManager(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;

		std::map<TextureID, std::shared_ptr<sf::Texture>> mTextures;
	};
}

#endif
