#include "scripted_game.h"
#include "utilities.h"
#include "tmx.h"
#include "tile_map.h"
#include "scripted_entity.h"
#include "texture_manager.h"
#include "entity_manager.h"
#include "camera_entity.h"

#include <SFML/Window.hpp>

#include <regex>
#include <iterator>

namespace te
{
	// LuaBridge does not support enum
	static int KeyPressed = sf::Event::KeyPressed;
	static int KeyReleased = sf::Event::KeyReleased;

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
	static int Space = sf::Keyboard::Space;

	const static std::regex packageExpr{"(?:.*/)*([a-zA-Z_0-9]+)\\.lua"};

	static sf::Vector2f addVec(sf::Vector2f a, sf::Vector2f b) { return a + b; }
	static sf::Vector2f mulVec(float scalar, sf::Vector2f v) { return scalar * v; }
	static sf::Vector2f normalizeVec(sf::Vector2f v)
	{
		if (v.x == 0 && v.y == 0) return v;
		return v / std::sqrt(v.x * v.x + v.y * v.y);
	}

	ScriptedGame::ScriptedGame(Application& app, const std::string& initFilename)
		: Game{app}
		, mpL{luaL_newstate(), [](lua_State* L) { lua_close(L); }}
		, mInputFn{mpL.get()}
		, mUpdateFn{mpL.get()}
		, mpCamera{nullptr}
	{
		auto pCamera = CameraEntity::make(*this);
		mpCamera = pCamera.get();
		getSceneGraph().attachNode(std::move(pCamera));

		lua_State* L = mpL.get();

		luaL_openlibs(L);

		luabridge::getGlobalNamespace(L)
			.beginNamespace("Event")
				.addVariable("KeyPressed", &KeyPressed, false)
				.addVariable("KeyReleased", &KeyReleased, false)
			.endNamespace()
			.beginNamespace("Key")
				.addVariable("W", &W, false)
				.addVariable("A", &A, false)
				.addVariable("S", &S, false)
				.addVariable("D", &D, false)
				.addVariable("Space", &Space, false)
			.endNamespace()
			.beginClass<sf::Vector2f>("Vec")
				.addConstructor<void(*)(float,float)>()
				.addData("x", &sf::Vector2f::x)
				.addData("y", &sf::Vector2f::y)
			.endClass()
			.addFunction("addVec", &addVec)
			.addFunction("mulVec", &mulVec)
			.addFunction("normalizeVec", &normalizeVec)
			.beginClass<ScriptedTelegram>("Telegram")
				.addData("dispatchTime", &ScriptedTelegram::dispatchTime)
				.addData("sender", &ScriptedTelegram::sender)
				.addData("receiver", &ScriptedTelegram::receiver)
				.addData("info", &ScriptedTelegram::info)
			.endClass()
			.beginClass<ScriptedGame>("Game")
				.addFunction("loadMap", &ScriptedGame::loadMap)
				.addFunction("loadSpritesheet", &ScriptedGame::loadSpritesheet)
				.addFunction("makeEntity", &ScriptedGame::makeEntity)
				.addFunction("getScriptedEntity", &ScriptedGame::getScriptedEntity)
				.addProperty("camera", &ScriptedGame::getCamera)
				.addFunction("dispatchMessage", &ScriptedGame::dispatchMessage)
				.addFunction("getMap", &ScriptedGame::getMap)
				.addFunction("getObjects", &ScriptedGame::getObjects)
				.addFunction("getLayerNames", &ScriptedGame::getLayerNames)
			.endClass()
			.beginClass<SceneNode>("SceneNode")
				.addProperty("position", &SceneNode::getPosition, &SceneNode::setPosition)
				.addFunction<void(SceneNode::*)(float,float)>("move", &SceneNode::move)
				.addProperty("drawOrder", &SceneNode::getDrawOrder, &SceneNode::setDrawOrder)
			.endClass()
			.deriveClass<BaseGameEntity, SceneNode>("BaseGameEntity")
				.addFunction("attachNode", &BaseGameEntity::attachNodeByID)
			.endClass()
			.deriveClass<CameraEntity, BaseGameEntity>("Camera")
				.addFunction("setViewSize", &CameraEntity::setViewSize)
			.endClass()
			.deriveClass<TileMap, BaseGameEntity>("TileMap")
			.endClass()
			.deriveClass<ScriptedEntity, BaseGameEntity>("Entity")
				.addProperty("world", &ScriptedEntity::getWorld)
				.addProperty("data", &ScriptedEntity::getUserData)
				.addFunction("initMachine", &ScriptedEntity::initMachine)
				.addProperty("animation", &ScriptedEntity::getAnimation, &ScriptedEntity::setAnimation)
				.addFunction("setPositionByTile", &ScriptedEntity::setPositionByTile)
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
		if (!pkg.isTable()) throw std::runtime_error{"Package `" + packageName + "' not found. Package name must match filename."};

		mInputFn = pkg["processInput"];
		if (!mInputFn.isNil() && !mInputFn.isFunction()) throw std::runtime_error{"processInput must be a function."};

		if (pkg["update"].isFunction()) mUpdateFn = pkg["update"];
		else if (!pkg["update"].isNil()) throw std::runtime_error{"update must be a function."};

		luabridge::LuaRef init = pkg["init"];
		if (!init.isFunction()) throw std::runtime_error{"Must supply init function."};
		try
		{
			init(this);
		}
		catch (luabridge::LuaException& ex)
		{
			storeLuaState(std::move(mpL));
			throw ex;
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

	ScriptedEntity* ScriptedGame::getScriptedEntity(EntityID id) const
	{
		EntityManager& em = getEntityManager();
		if (em.hasEntity(id))
		{
			auto& entity = getEntityManager().getEntityFromID(id);
			assert(dynamic_cast<ScriptedEntity*>(&entity) != nullptr);
			return static_cast<ScriptedEntity*>(&entity);
		}
		return nullptr;
	}

	TileMap* ScriptedGame::getMap(EntityID id) const
	{
		EntityManager& em = getEntityManager();
		if (em.hasEntity(id))
		{
			auto& entity = getEntityManager().getEntityFromID(id);
			assert(dynamic_cast<TileMap*>(&entity) != nullptr);
			return static_cast<TileMap*>(&entity);
		}
		return nullptr;
	}

	luabridge::LuaRef ScriptedGame::getObjects(EntityID mapID, const std::string& groupName) const
	{
		luabridge::LuaRef table = luabridge::newTable(mpL.get());

		TileMap* map = getMap(mapID);
		if (map)
		{
			std::vector<TileMap::Area> areas;
			map->getAreasInGroup(groupName, std::back_inserter(areas));

			std::for_each(areas.begin(), areas.end(), [this, &table](TileMap::Area& area) {
				luabridge::LuaRef obj = luabridge::newTable(mpL.get());
				obj["id"] = area.id;
				if (area.name != "") obj["name"] = area.name;
				if (area.type != "") obj["type"] = area.type;
				obj["x"] = area.rect.left;
				obj["y"] = area.rect.top;
				obj["w"] = area.rect.width;
				obj["h"] = area.rect.height;
				table[area.name] = obj;
			});
		}

		return table;
	}

	luabridge::LuaRef ScriptedGame::getLayerNames(EntityID mapID) const
	{
		luabridge::LuaRef table = luabridge::newTable(mpL.get());

		TileMap* map = getMap(mapID);
		if (map)
		{
			std::vector<std::string> layerNames;
			map->getLayerNames(std::back_inserter(layerNames));
			size_t index = 1;
			std::for_each(layerNames.begin(), layerNames.end(), [&index, &table](const std::string& name) {
				table[index++] = name;
			});
		}

		return table;
	}

	CameraEntity& ScriptedGame::getCamera() const
	{
		return *mpCamera;
	}

	void ScriptedGame::dispatchMessage(double delay, EntityID sender, EntityID receiver, luabridge::LuaRef info)
	{
		getMessageDispatcher().dispatchMessage(delay, sender, receiver, 0, std::make_unique<ScriptedInfo>(info));
	}

	std::unique_ptr<ScriptedGame> ScriptedGame::make(Application& app, const std::string& initFilename)
	{
		return std::unique_ptr<ScriptedGame>{new ScriptedGame{app, initFilename}};
	}

	void ScriptedGame::processInput(const sf::Event& evt)
	{
		if (mInputFn.isFunction())
			mInputFn(this, (int)evt.key.code, (int)evt.type);
	}

	void ScriptedGame::update(const sf::Time& dt)
	{
		Game::update(dt);
		if (mUpdateFn.isFunction())
			mUpdateFn(this, dt.asSeconds());
	}

	void ScriptedGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.clear();
		sf::View cameraView = mpCamera->getView();
		sf::View originalView = target.getView();
		target.setView(cameraView);
		Game::draw(target, states);
		target.setView(originalView);
	}
}
