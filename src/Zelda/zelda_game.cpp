#include "zelda_game.h"
#include "tmx.h"
#include "tile_map.h"
#include "entity_manager.h"
#include "message_dispatcher.h"
#include "camera.h"
#include "texture_manager.h"
#include "animation.h"
#include "application.h"

namespace te
{
	std::unique_ptr<ZeldaGame> ZeldaGame::make(Application& state, const std::string& fileName, const sf::Transform& pixelToWorld)
	{
		return std::unique_ptr<ZeldaGame>(new ZeldaGame(state, fileName, pixelToWorld));
	}

	ZeldaGame::ZeldaGame(Application& app, const std::string& fileName, const sf::Transform& pixelToWorld)
		: Game(app)
		, mPlayerID(-1)
		, mpCamera(nullptr)
	{
		setPixelToWorldTransform(pixelToWorld);
		getTextureManager().loadSpritesheet("src/Zelda/textures/inigo_spritesheet.xml");
		getTextureManager().loadAnimations("src/Zelda/textures/inigo_animation.xml");
		getTextureManager().loadSpritesheet("assets/spritesheets/hero/hero.xml");
		loadMap(fileName);
	}

	void ZeldaGame::loadMap(const std::string& fileName)
	{
		TMX tmx(fileName);

		TMX::Object* pPlayer = nullptr;
		int playerSortOrder = 0;
		std::vector<TMX::ObjectGroup> objectGroups = tmx.getObjectGroups();
		for (auto& objectGroup : objectGroups)
		{
			for (auto& object : objectGroup.objects)
			{
				if (object.name == "Player")
				{
					if (pPlayer != nullptr) { throw std::runtime_error("Multiple Player objects found."); }
					pPlayer = &object;
					playerSortOrder = objectGroup.index;
				}
			}
		}
		if (pPlayer == nullptr) { throw std::runtime_error("No Player object found."); }

		auto upPlayer = Player::make(*this, *pPlayer);
		upPlayer->setDrawOrder(playerSortOrder);
		mPlayerID = upPlayer->getID();
		getSceneGraph().attachNode(std::move(upPlayer));

		mpCamera = std::make_unique<Camera>(getEntityManager(), mPlayerID, sf::Vector2f(16 * 24.f, 9 * 24.f));

		setTileMap(TileMap::make(*this, getTextureManager(), std::move(tmx)));
		getMap().setDrawColliderEnabled(true);
		getMap().setDrawNavGraphEnabled(true);

		// TODO: DELETE SECOND MAP
		auto upMap2 = TileMap::make(*this, getTextureManager(), TMX{fileName});
		upMap2->setDrawColliderEnabled(true);
		upMap2->setDrawNavGraphEnabled(true);
		getMap().stitch(sf::Vector2i{ 0, 0 }, *upMap2, sf::Vector2i{ 0, 20 });
		auto* pMap = upMap2.get();
		getMap().attachNode(std::move(upMap2));

		auto upMap3 = TileMap::make(*this, getTextureManager(), TMX{fileName});
		pMap->stitch({ 0, 0 }, *upMap3, { 30, 0 });
		getMap().attachNode(std::move(upMap3));

		//getMap().move({ -4, 0 });
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
		sf::View originalView = target.getView();
		target.setView(mpCamera->getView(states.transform));
		Game::draw(target, states);
		target.setView(originalView);
	}
}
