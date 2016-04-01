#include "zelda_game.h"
#include "tmx.h"
#include "tile_map.h"
#include "entity_manager.h"
#include "message_dispatcher.h"

namespace te
{
	ZeldaGame::ZeldaGame(const std::shared_ptr<TextureManager>& pTextureManager, const std::string& fileName)
		: mpTextureManager(pTextureManager)
		, mpEntityManager(std::make_shared<EntityManager>())
		, mpMessageDispatcher(std::make_shared<MessageDispatcher>(mpEntityManager))
		, mpPlayer(nullptr)
	{
		if (!pTextureManager)
		{
			throw std::runtime_error("Must supply TextureManager in ZeldaGame ctor.");
		}

		loadMap(fileName);
	}

	void ZeldaGame::loadMap(const std::string& fileName)
	{
		TMX tmx(fileName);
		setTileMap(std::make_shared<TileMap>(*mpTextureManager, tmx));

		TMX::Object* pPlayer = nullptr;
		std::vector<TMX::ObjectGroup> objectGroups = tmx.getObjectGroups();
		for (auto& objectGroup : objectGroups)
		{
			for (auto& object : objectGroup.objects)
			{
				if (object.name == "Player")
				{
					if (pPlayer != nullptr) { throw std::runtime_error("Multiple Player objects found."); }
					pPlayer = &object;
				}
			}
		}
		if (pPlayer == nullptr) { throw std::runtime_error("No Player object found."); }
		mpPlayer = std::make_shared<Player>(*this, *pPlayer);
		mpEntityManager->registerEntity(mpPlayer);
	}

	void ZeldaGame::processInput(const sf::Event& evt)
	{
		if (evt.type == sf::Event::KeyPressed || evt.type == sf::Event::KeyReleased)
		{
			int flip = evt.type == sf::Event::KeyPressed ? Player::On : Player::Off;
			switch (evt.key.code)
			{
			case sf::Keyboard::W:
				mpMessageDispatcher->dispatchMessage(0.0, -1, mpPlayer->getID(), Player::Up | flip, NULL);
				break;
			case sf::Keyboard::A:
				mpMessageDispatcher->dispatchMessage(0.0, -1, mpPlayer->getID(), Player::Left | flip, NULL);
				break;
			case sf::Keyboard::S:
				mpMessageDispatcher->dispatchMessage(0.0, -1, mpPlayer->getID(), Player::Down | flip, NULL);
				break;
			case sf::Keyboard::D:
				mpMessageDispatcher->dispatchMessage(0.0, -1, mpPlayer->getID(), Player::Right | flip, NULL);
				break;
			}
		}
	}

	void ZeldaGame::update(const sf::Time& dt)
	{
		mpMessageDispatcher->dispatchDelayedMessages(dt);
		mpPlayer->update(dt);
	}

	void ZeldaGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		Game::draw(target, states);
		target.draw(*mpPlayer);
	}
}
