#ifndef TE_ZELDA_GAME_H
#define TE_ZELDA_GAME_H

#include "game.h"
#include "player.h"

namespace te
{
	class TextureManager;
	class EntityManager;
	class MessageDispatcher;
	class Camera;

	class ZeldaGame : public Game
	{
	public:
		static std::unique_ptr<ZeldaGame> make(Application& app, TextureManager& textureManager, const std::string& fileName, const sf::Transform& pixelToWorldTransform);

		void processInput(const sf::Event& evt);

	private:
		ZeldaGame(Application& app, TextureManager& textureManager, const std::string& fileName, const sf::Transform& pixelToWorld);

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void loadMap(const std::string& fileName);

		TextureManager& mTextureManager;

		int mPlayerID;
		std::unique_ptr<Camera> mpCamera;
	};
}

#endif
