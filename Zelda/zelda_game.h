#ifndef TE_ZELDA_GAME_H
#define TE_ZELDA_GAME_H

#include "game.h"

namespace te
{
	class TextureManager;

	class ZeldaGame : public Game
	{
	public:
		ZeldaGame(const std::shared_ptr<TextureManager>& pTextureManager);

		void loadMap(const std::string& fileName);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		std::shared_ptr<TextureManager> mpTextureManager;
	};
}

#endif
