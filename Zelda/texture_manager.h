#ifndef TE_TEXTURE_MANAGER_H
#define TE_TEXTURE_MANAGER_H

#include "typedefs.h"
#include "texture_atlas.h"

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
		TextureID loadSpritesheet(const std::string& dir, const std::string& xmlFile);

		sf::Texture& get(TextureID file) const;
		sf::Sprite getSprite(TextureID sprite) const;
	private:
		TextureManager();

		TextureManager(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;

		std::map<TextureID, std::unique_ptr<sf::Texture>> mTextures;

		struct SpriteData
		{
			TextureID textureID;
			TextureAtlas::Sprite sprite;
		};
		std::map<TextureID, SpriteData> mSpriteMap;
	};
}

#endif
