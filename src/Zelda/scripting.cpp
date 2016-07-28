#include "scripting.h"
#include "game_data.h"
#include "utilities.h"
#include "tmx.h"
#include "tile_map_layer.h"
#include "texture_atlas.h"
#include "input.h"

#include <lua.hpp>
#include <LuaBridge.h>

#include <regex>
#include <cassert>
#include <array>

namespace te
{
	static unsigned A = static_cast<unsigned>(XBoxInput::A);
	static unsigned B = static_cast<unsigned>(XBoxInput::B);
	static unsigned X = static_cast<unsigned>(XBoxInput::X);
	static unsigned Y = static_cast<unsigned>(XBoxInput::Y);
	static unsigned LB = static_cast<unsigned>(XBoxInput::LB);
	static unsigned RB = static_cast<unsigned>(XBoxInput::RB);

	static unsigned LT = static_cast<unsigned>(XBoxInput::LT);
	static unsigned RT = static_cast<unsigned>(XBoxInput::RT);

	const static std::regex packageExpr{"(?:.*/)*([a-zA-Z_0-9]+)\\.lua"};
	static std::string getPackageName(const std::string& scriptFilename)
	{
		std::smatch match;
		if (std::regex_match(scriptFilename, match, packageExpr))
		{
			return match[1];
		}
		throw std::runtime_error{"Could not match Lua script name."};
	}

	struct ScriptInit::Impl
	{
		using KeymapImpl = Keymap<unsigned, unsigned>;

		struct ProxyEntity
		{
			ProxyEntity(GameData& data)
				: m_rData{data}
				, m_ID{m_rData.entityIDManager.getNextID()}
			{}

			void addTileLayer(ResourceID<TileMapLayer> id, int sortingLayer)
			{
				m_rData.mapLayers.insert(m_ID, { m_rData.mapLayerHolder.get(id), sortingLayer });
			}

			void addRigidBody(int type)
			{
				assert(type >= 0 && type <= 2);
				b2BodyDef bodyDef;
				bodyDef.type = static_cast<b2BodyType>(type);
				auto* pPhysicsWorld = &m_rData.physicsWorld;
				m_rData.rigidBodies[m_ID] = {
					m_rData.physicsWorld.CreateBody(&bodyDef),
					[pPhysicsWorld](b2Body* pBody) { pPhysicsWorld->DestroyBody(pBody); }
				};
			}

			void addFixtureRect(luabridge::LuaRef rect)
			{
				assert(rect.isTable());
				auto& body = *m_rData.rigidBodies.at(m_ID);

				b2PolygonShape shape;
				float x = rect["x"], y = rect["y"], w = rect["w"], h = rect["h"];
				std::array<b2Vec2, 4> points = {
					b2Vec2{ x, y },
					b2Vec2{ x + w, y },
					b2Vec2{ x + w, y + h },
					b2Vec2{ x, y + h }
				};
				shape.Set(points.data(), 4);

				body.CreateFixture(&shape, 0);
			}

			GameData& m_rData;
			EntityID m_ID;
		};

		GameData& m_rData;

		Impl::Impl(GameData& data)
			: m_rData{ data }
		{
			lua_State* L = m_rData.pL.get();

			luabridge::getGlobalNamespace(L)
				.beginNamespace("Button")
					.addVariable("A", &A, false)
					.addVariable("B", &B, false)
					.addVariable("X", &X, false)
					.addVariable("Y", &Y, false)
					.addVariable("LB", &LB, false)
					.addVariable("RB", &RB, false)
				.endNamespace()
				.beginNamespace("Axis")
					.addVariable("LT", &LT, false)
					.addVariable("RT", &RT, false)
				.endNamespace()
				.beginClass<KeymapImpl>("Keymap")
					.addConstructor<void(*)(void)>()
					.addData("lightAttack", &KeymapImpl::lightAttack)
					.addData("parry", &KeymapImpl::parry)
					.addData("heavyAttack", &KeymapImpl::heavyAttack)
					.addData("lock", &KeymapImpl::lock)
				.endClass()
				.beginClass<ResourceID<TMX>>("TMXID").endClass()
				.beginClass<ResourceID<TileMapLayer>>("LayerID").endClass()
				.beginClass<ResourceID<TextureAtlas>>("AtlasID").endClass()
				.beginClass<sf::Vector2f>("Vec")
					.addConstructor<void(*)(float, float)>()
					.addData("x", &sf::Vector2f::x)
					.addData("y", &sf::Vector2f::y)
				.endClass()
				.beginClass<Impl>("Game")
					.addProperty("pixelToUnitScale", &Impl::getPixelToWorldScale, &Impl::setPixelToWorldScale)
					.addProperty("viewSize", &Impl::getViewSize, &Impl::setViewSize)
					.addProperty("viewCenter", &Impl::getViewCenter, &Impl::setViewCenter)
					.addFunction("makeEntity", &Impl::makeEntity)
					.addFunction("loadTMX", &Impl::loadTMX)
					.addFunction("makeTileLayers", &Impl::makeTileLayers)
					.addFunction("getTileLayerIndex", &Impl::getTileLayerIndex)
					.addFunction("getObjectsInLayer", &Impl::getObjectsInLayer)
					.addFunction("loadAtlas", &Impl::loadAtlas)
					.addFunction("setKeymap", &Impl::setKeymap)
				.endClass()
				.beginClass<ProxyEntity>("Entity")
					.addData("id", &ProxyEntity::m_ID, false)
					.addFunction("addTileLayer", &ProxyEntity::addTileLayer)
					.addFunction("addRigidBody", &ProxyEntity::addRigidBody)
					.addFunction("addFixtureRect", &ProxyEntity::addFixtureRect)
				.endClass();

			doLuaFile(*L, m_rData.config.initialScript);

			std::string packageName = getPackageName(m_rData.config.initialScript);

			luabridge::LuaRef pkg = luabridge::getGlobal(L, packageName.c_str());
			if (!pkg.isTable()) throw std::runtime_error{ "Package `" + packageName + "' not found. Package name must match filename." };

			luabridge::LuaRef init = pkg["init"];
			if (!init.isFunction()) throw std::runtime_error{ "Must supply init function." };

			init(this);
		}

		sf::Vector2f getPixelToWorldScale() const
		{
			return m_rData.pixelToWorldScale;
		}
		void setPixelToWorldScale(sf::Vector2f scale)
		{
			m_rData.pixelToWorldScale = scale;
		}
		sf::Vector2f getViewSize() const
		{
			return m_rData.mainView.getSize();
		}
		void setViewSize(sf::Vector2f size)
		{
			m_rData.mainView.setSize(size);
		}
		sf::Vector2f getViewCenter() const
		{
			return m_rData.mainView.getCenter();
		}
		void setViewCenter(sf::Vector2f size)
		{
			m_rData.mainView.setCenter(size);
		}

		ProxyEntity makeEntity()
		{
			return ProxyEntity{ m_rData };
		}

		ResourceID<TMX> loadTMX(const std::string& filename)
		{
			return m_rData.tmxHolder.load(filename);
		}

		luabridge::LuaRef makeTileLayers(ResourceID<TMX> id)
		{
			luabridge::LuaRef table = luabridge::newTable(m_rData.pL.get());

			TMX& tmx = m_rData.tmxHolder.get(id);
			std::vector<std::string> tilesetFilenames{};
			getTilesetFilenames(tmx, std::back_inserter(tilesetFilenames));
			std::vector<const sf::Texture*> textures{};
			//ResourceManager<sf::Texture>& textureManager = getTextureManager();
			std::transform(tilesetFilenames.begin(), tilesetFilenames.end(), std::back_inserter(textures), [this](const std::string& filename) {
				return &m_rData.textureHolder.get(m_rData.textureHolder.load(filename));
			});
			std::vector<TileMapLayer> layers{};
			TileMapLayer::make(tmx, textures.begin(), textures.end(), std::back_inserter(layers));
			for (auto& layer : layers) table[layer.getName()] = m_rData.mapLayerHolder.store(std::make_unique<TileMapLayer>(layer));

			return table;
		}

		luabridge::LuaRef getObjectsInLayer(ResourceID<TMX> tmxID, const std::string& layerName)
		{
			lua_State* L = m_rData.pL.get();
			luabridge::LuaRef table = luabridge::newTable(L);

			const TMX& tmx = get(m_rData, tmxID);
			std::vector<TMX::Object> objects;
			getObjectsInGroup(tmx, layerName, std::back_inserter(objects));

			int index = 1;
			std::for_each(objects.begin(), objects.end(), [L, &table, &index](TMX::Object& area) {
				luabridge::LuaRef obj = luabridge::newTable(L);
				obj["id"] = area.id;
				if (area.name != "") obj["name"] = area.name;
				if (area.type != "") obj["type"] = area.type;
				obj["x"] = area.x;
				obj["y"] = area.y;
				obj["w"] = area.width;
				obj["h"] = area.height;
				table[index++] = obj;
			});

			return table;
		}

		unsigned getTileLayerIndex(ResourceID<TileMapLayer> id)
		{
			return m_rData.mapLayerHolder.get(id).getIndex();
		}

		ResourceID<TextureAtlas> loadAtlas(const std::string& filename)
		{
			return load<TextureAtlas>(m_rData, filename);
		}

		void setKeymap(unsigned playerID, KeymapImpl keymap)
		{
			auto& storedMap = m_rData.keymaps[playerID];
			storedMap.lightAttack = static_cast<XBoxInput>(keymap.lightAttack);
			storedMap.parry = static_cast<XBoxInput>(keymap.parry);
			storedMap.heavyAttack = static_cast<sf::Joystick::Axis>(keymap.heavyAttack);
			storedMap.lock = static_cast<sf::Joystick::Axis>(keymap.lock);
		}
	};

	ScriptInit::ScriptInit(GameData& data)
		: m_pImpl{std::make_unique<Impl>(data)}
	{}
	ScriptInit::ScriptInit(ScriptInit&&) = default;
	ScriptInit& ScriptInit::operator=(ScriptInit&&) = default;
	ScriptInit::~ScriptInit() = default;
}
