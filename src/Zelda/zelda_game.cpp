#include "zelda_game.h"
#include "tmx.h"
#include "tile_map.h"
#include "entity_manager.h"
#include "message_dispatcher.h"
#include "camera.h"
#include "texture_manager.h"
#include "animation.h"

namespace te
{
	std::unique_ptr<ZeldaGame> ZeldaGame::make(Application& app, TextureManager& textureManager, const std::string& fileName, const sf::Transform& pixelToWorld)
	{
		return std::unique_ptr<ZeldaGame>(new ZeldaGame(app, textureManager, fileName, pixelToWorld));
	}

	ZeldaGame::ZeldaGame(Application& app, TextureManager& textureManager, const std::string& fileName, const sf::Transform& pixelToWorld)
		: Game(app, pixelToWorld)
		, mTextureManager(textureManager)
		, mPlayerID(-1)
		, mpCamera(nullptr)
	{
		mTextureManager.loadSpritesheet("textures/inigo_spritesheet.xml");
		mTextureManager.loadAnimations("textures/inigo_animation.xml");
		loadMap(fileName);
	}

	void ZeldaGame::loadMap(const std::string& fileName)
	{
		TMX tmx(fileName);
		setTileMap(std::make_unique<TileMap>(*this, mTextureManager, tmx));
		getMap().setDrawColliderEnabled(true);
		getMap().setDrawNavGraphEnabled(true);

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

		auto upPlayer = Player::make(*this, *pPlayer);
		mPlayerID = upPlayer->getID();
		getSceneGraph().attachNode(std::move(upPlayer));

		mpCamera = std::make_unique<Camera>(getEntityManager(), mPlayerID, sf::Vector2f(16 * 24.f, 9 * 24.f));
	}

	void ZeldaGame::processInput(const sf::Event& evt)
	{
		if (!sf::Joystick::isConnected(0))
		{
			bool w = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
			bool a = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
			bool s = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
			bool d = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

			float yAxis = (w && s || !(w || s)) ? 0 : (w ? -1.f : 1.f);
			getMessageDispatcher().dispatchMessage(0.0, -1, mPlayerID, Player::Y, &yAxis);
			float xAxis = (a && d || !(a || d)) ? 0 : (a ? -1.f : 1.f);
			getMessageDispatcher().dispatchMessage(0.0, -1, mPlayerID, Player::X, &xAxis);

		}
		else
		{
			float xAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::X) / 100.f;
			getMessageDispatcher().dispatchMessage(0.0, -1, mPlayerID, Player::X, (void*)&xAxis);
			float yAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Y) / 100.f;
			getMessageDispatcher().dispatchMessage(0.0, -1, mPlayerID, Player::Y, (void*)&yAxis);
		}
	}

	void ZeldaGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform.scale(0.5f, 0.5f) *= getWorldToPixelTransform();
		target.setView(mpCamera->getView(states.transform));
		Game::draw(target, states);
	}
}
