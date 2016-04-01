#ifndef TE_ZELDA_GAME_H
#define TE_ZELDA_GAME_H

#include "game.h"
#include "player.h"

namespace te
{
	class TextureManager;
	class EntityManager;
	class MessageDispatcher;

	class ZeldaGame : public Game
	{
	public:
		ZeldaGame(const std::shared_ptr<TextureManager>& pTextureManager, const std::string& fileName);

		void processInput(const sf::Event& evt);
		void update(const sf::Time& dt);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void loadMap(const std::string& fileName);

		std::shared_ptr<TextureManager> mpTextureManager;

		std::shared_ptr<EntityManager> mpEntityManager;
		std::shared_ptr<MessageDispatcher> mpMessageDispatcher;

		std::shared_ptr<Player> mpPlayer;
	};
}

#endif
