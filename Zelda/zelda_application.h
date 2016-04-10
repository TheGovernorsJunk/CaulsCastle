#ifndef TE_ZELDA_APPLICATION_H
#define TE_ZELDA_APPLICATION_H

#include "application.h"
#include "texture_manager.h"
#include "texture_atlas.h"

#include <string>

namespace te
{
	class ZeldaApplication : public Application
	{
	public:
		ZeldaApplication(const std::string& filename);
	private:
		std::unique_ptr<sf::RenderWindow> makeWindow() const;
		std::unique_ptr<Game> makeGame() const;

		std::unique_ptr<TextureManager> mpTextureManager;
		std::map<TextureID, TextureAtlas> mTextureAtlasMap;
		std::string mFilename;
	};
}

#endif
