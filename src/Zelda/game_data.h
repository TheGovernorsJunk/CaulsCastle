#ifndef TE_GAME_DATA_H
#define TE_GAME_DATA_H

#include "component_store.h"
#include "tile_map_layer.h"
#include "resource_manager.h"
#include "texture_atlas.h"
#include "tmx.h"
#include "entity_id_manager.h"

#include <SFML/Graphics.hpp>

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

		template <typename Resource>
		using ResourceHolder = te::ResourceManager<Resource>;

		ResourceHolder<sf::Texture> textureHolder;
		ResourceHolder<TextureAtlas> atlasHolder;
		ResourceHolder<sf::Sprite> spriteHolder;
		ResourceHolder<TMX> tmxHolder;

		EntityIDManager entityIDManager;

		struct {
			bool left;
			bool right;
			bool up;
			bool down;
		} directionInput;
		ComponentStore<sf::Vector2f> positions;
		ComponentStore<sf::Vector2f> velocities;
		ComponentStore<sf::Sprite> sprites;
		ComponentStore<int> sortingLayers;
		ComponentStore<te::TileMapLayer> mapLayers;
		std::vector<PendingDraw> pendingDraws;

		std::unique_ptr<lua_State, void(*)(lua_State*)> pL;
		std::unique_ptr<sf::RenderWindow> pWindow;
	};
}

#endif
