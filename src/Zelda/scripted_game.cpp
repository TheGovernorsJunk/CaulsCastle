#include "scripted_game.h"
#include "utilities.h"
#include "tmx.h"
#include "tile_map.h"
#include "scripted_entity.h"
#include "texture_manager.h"

namespace te
{
	ScriptedGame::ScriptedGame(Application& app, const std::string& initFilename)
		: Game{app}
		, mpL{luaL_newstate(), [](lua_State* L) { lua_close(L); }}
	{
		lua_State* L = mpL.get();

		luaL_openlibs(L);

		luabridge::getGlobalNamespace(L)
			.beginClass<ScriptedGame>("Game")
				.addFunction("loadMap", &ScriptedGame::loadMap)
				.addFunction("loadSpritesheet", &ScriptedGame::loadSpritesheet)
				.addFunction("makeEntity", &ScriptedGame::makeEntity)
			.endClass()
			.beginClass<ScriptedEntity>("Entity")
				.addFunction("initMachine", &ScriptedEntity::initMachine)
				.addFunction("setAnimation", &ScriptedEntity::setAnimation)
			.endClass();

		doLuaFile(*L, initFilename);
		luabridge::LuaRef init = luabridge::getGlobal(L, "init");

		if (!init.isFunction()) throw std::runtime_error{"Must supply init function."};
		init(this);
	}

	ScriptedGame::~ScriptedGame()
	{
		storeLuaState(std::move(mpL));
	}

	EntityID ScriptedGame::loadMap(const std::string& filename)
	{
		auto upTileMap = TileMap::make(*this, getTextureManager(), TMX{filename});
		EntityID id = upTileMap->getID();
		getSceneGraph().attachNode(std::move(upTileMap));
		return id;
	}

	TextureID ScriptedGame::loadSpritesheet(const std::string& filename)
	{
		return getTextureManager().loadSpritesheet(filename);
	}

	EntityID ScriptedGame::makeEntity(luabridge::LuaRef entityTable)
	{
		auto upEntity = ScriptedEntity::make(*this, entityTable, {0, 0});
		EntityID id = upEntity->getID();
		ScriptedEntity* pEntity = upEntity.get();
		getSceneGraph().attachNode(std::move(upEntity));
		return id;
	}

	std::unique_ptr<ScriptedGame> ScriptedGame::make(Application& app, const std::string& initFilename)
	{
		return std::unique_ptr<ScriptedGame>{new ScriptedGame{app, initFilename}};
	}

	void ScriptedGame::processInput(const sf::Event& evt)
	{
	}

	void ScriptedGame::update(const sf::Time& dt)
	{
		Game::update(dt);
	}

	void ScriptedGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.clear();
		Game::draw(target, states);
	}
}
