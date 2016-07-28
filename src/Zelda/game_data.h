#ifndef TE_GAME_DATA_H
#define TE_GAME_DATA_H

#include "component_store.h"
#include "tile_map_layer.h"
#include "resource_manager.h"
#include "texture_atlas.h"
#include "tmx.h"
#include "entity_id_manager.h"
#include "input.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <memory>

struct lua_State;

namespace te
{
	struct Config
	{
		int fps;
		unsigned screenWidth;
		unsigned screenHeight;
		std::string windowTitle;
		std::string initialScript;
	};

	struct PendingDraw
	{
		sf::RenderStates renderStates;
		int drawOrder;
		const sf::Drawable* pDrawable;
	};

	struct GameData
	{
		GameData();
		GameData(GameData&&) = default;
		GameData& operator=(GameData&&) = default;

		Config config;
		sf::Vector2f pixelToWorldScale;
		sf::View mainView;
		b2World physicsWorld;

		template <typename Resource>
		using ResourceHolder = te::ResourceManager<Resource>;

		ResourceHolder<sf::Texture> textureHolder;
		ResourceHolder<TextureAtlas> atlasHolder;
		ResourceHolder<sf::Sprite> spriteHolder;
		ResourceHolder<TMX> tmxHolder;
		ResourceHolder<TileMapLayer> mapLayerHolder;

		EntityIDManager entityIDManager;

		boost::container::flat_map<unsigned, Keymap<XBoxInput, sf::Joystick::Axis>> keymaps;
		boost::container::flat_map<unsigned, Input> playerInputs;
		ComponentStore<sf::Vector2f> positions;
		ComponentStore<std::unique_ptr<b2Body, std::function<void(b2Body*)>>> rigidBodies;
		ComponentStore<sf::Vector2f> velocities;
		RenderableStore<sf::Sprite> sprites;
		RenderableStore<te::TileMapLayer> mapLayers;
		std::vector<PendingDraw> pendingDraws;

		std::unique_ptr<lua_State, void(*)(lua_State*)> pL;
		std::unique_ptr<sf::RenderWindow> pWindow;
	};
}

#endif
