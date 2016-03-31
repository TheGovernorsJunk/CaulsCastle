#ifndef TE_ZELDA_APPLICATION_H
#define TE_ZELDA_APPLICATION_H

#include "application.h"
#include "texture_manager.h"
#include "zelda_game.h"

#include <string>

namespace te
{
	class ZeldaApplication : public Application
	{
	public:
		ZeldaApplication(const std::string& filename);
	private:
		std::unique_ptr<sf::RenderWindow> makeWindow() const;
		void processInput(const sf::Event& evt);
		void update(const sf::Time& dt);
		void render(sf::RenderTarget& target);

		std::shared_ptr<TextureManager> mTextureManager;
		ZeldaGame mGame;
	};
}

#endif
