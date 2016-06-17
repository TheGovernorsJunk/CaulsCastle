#include "scripted_game.h"
#include "utilities.h"
#include "tmx.h"
#include "tile_map.h"
#include "scripted_entity.h"
#include "texture_manager.h"
#include "entity_manager.h"

#include <regex>

namespace te
{
	// LuaBridge does not support enum
	static int A = 0;
	static int B = 1;
	static int C = 2;
	static int D = 3;
	static int E = 4;
	static int F = 5;
	static int G = 6;
	static int H = 7;
	static int I = 8;
	static int J = 9;
	static int K = 10;
	static int L = 11;
	static int M = 12;
	static int N = 13;
	static int O = 14;
	static int P = 15;
	static int Q = 16;
	static int R = 17;
	static int S = 18;
	static int T = 19;
	static int U = 20;
	static int V = 21;
	static int W = 22;
	static int X = 23;
	static int Y = 24;
	static int Z = 25;

	const static std::regex packageExpr{"(?:.*/)*([a-zA-Z_0-9]+)\\.lua"};

	ScriptedGame::ScriptedGame(Application& app, const std::string& initFilename)
		: Game{app}
		, mpL{luaL_newstate(), [](lua_State* L) { lua_close(L); }}
		, mInputFn{mpL.get()}
	{
		lua_State* L = mpL.get();

		luaL_openlibs(L);

		luabridge::getGlobalNamespace(L)
			.beginNamespace("Key")
				.addVariable("W", &W, false)
				.addVariable("A", &A, false)
				.addVariable("S", &S, false)
				.addVariable("D", &D, false)
			.endNamespace()
			.beginClass<ScriptedTelegram>("Telegram")
				.addData("dispatchTime", &ScriptedTelegram::dispatchTime)
				.addData("sender", &ScriptedTelegram::sender)
				.addData("receiver", &ScriptedTelegram::receiver)
				.addData("msg", &ScriptedTelegram::msg)
				.addData("info", &ScriptedTelegram::info)
			.endClass()
			.beginClass<ScriptedGame>("Game")
				.addFunction("loadMap", &ScriptedGame::loadMap)
				.addFunction("loadSpritesheet", &ScriptedGame::loadSpritesheet)
				.addFunction("makeEntity", &ScriptedGame::makeEntity)
				.addFunction("getScriptedEntity", &ScriptedGame::getScriptedEntity)
				.addFunction("dispatchMessage", &ScriptedGame::dispatchMessage)
			.endClass()
			.beginClass<SceneNode>("SceneNode")
				.addFunction<void(SceneNode::*)(float,float)>("setPosition", &SceneNode::setPosition)
			.endClass()
			.deriveClass<BaseGameEntity, SceneNode>("BaseGameEntity")
			.endClass()
			.deriveClass<ScriptedEntity, BaseGameEntity>("Entity")
				.addFunction("initMachine", &ScriptedEntity::initMachine)
				.addFunction("setAnimation", &ScriptedEntity::setAnimation)
			.endClass();

		doLuaFile(*L, initFilename);

		std::string packageName{};
		std::smatch match;
		if (std::regex_match(initFilename, match, packageExpr))
		{
			packageName = match[1];
		}
		else
		{
			throw std::runtime_error{"Could not match Lua script name."};
		}

		luabridge::LuaRef pkg = luabridge::getGlobal(L, packageName.c_str());
		if (!pkg.isTable()) throw std::runtime_error{"Package not found. Package name must match filename."};

		mInputFn = pkg["processInput"];
		if (!mInputFn.isNil() && !mInputFn.isFunction()) throw std::runtime_error{"processInput must be a function."};

		luabridge::LuaRef init = pkg["init"];
		if (!init.isFunction()) throw std::runtime_error{"Must supply init function."};
		try
		{
			init(this);
		}
		catch (std::exception& ex)
		{
			storeLuaState(std::move(mpL));
			throw ex;
		}
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

	ScriptedEntity& ScriptedGame::getScriptedEntity(EntityID id) const
	{
		auto& entity = getEntityManager().getEntityFromID(id);
		assert(dynamic_cast<ScriptedEntity*>(&entity) != nullptr);
		return (ScriptedEntity&)entity;
	}

	void ScriptedGame::dispatchMessage(double delay, EntityID sender, EntityID receiver, int msg, luabridge::LuaRef info)
	{
		getMessageDispatcher().dispatchMessage(delay, sender, receiver, msg, std::make_unique<ScriptedInfo>(info));
	}

	std::unique_ptr<ScriptedGame> ScriptedGame::make(Application& app, const std::string& initFilename)
	{
		return std::unique_ptr<ScriptedGame>{new ScriptedGame{app, initFilename}};
	}

	void ScriptedGame::processInput(const sf::Event& evt)
	{
		if (evt.type == sf::Event::KeyPressed && mInputFn.isFunction())
			mInputFn(this, (int)evt.key.code);
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
