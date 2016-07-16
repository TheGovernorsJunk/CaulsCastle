#include "scripting.h"
#include "game_data.h"
#include "utilities.h"

#include <lua.hpp>
#include <LuaBridge.h>

#include <regex>

namespace te
{
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
		struct ProxyEntity
		{
			ProxyEntity(GameData& data)
				: m_rData{data}
				, m_ID{m_rData.entityIDManager.getNextID()}
			{}

			ResourceID<TileMapLayer> getLayer() const
			{
				return m_LayerID;
			}

			void setLayer(ResourceID<TileMapLayer> id)
			{
				m_LayerID = id;
				m_rData.mapLayers[m_ID] = m_rData.mapLayerHolder.get(id);
			}

			int getSortingLayer() const { return m_rData.sortingLayers.at(m_ID); }
			void setSortingLayer(int idx) { m_rData.sortingLayers[m_ID] = idx; }

			GameData& m_rData;
			EntityID m_ID;
			ResourceID<TileMapLayer> m_LayerID;
		};

		GameData& m_rData;

		Impl::Impl(GameData& data)
			: m_rData{ data }
		{
			lua_State* L = m_rData.pL.get();

			luabridge::getGlobalNamespace(L)
				.beginClass<ResourceID<TMX>>("TMXID").endClass()
				.beginClass<ResourceID<TileMapLayer>>("LayerID").endClass()
				.beginClass<Impl>("Game")
					.addFunction("makeEntity", &Impl::makeEntity)
					.addFunction("loadTMX", &Impl::loadTMX)
					.addFunction("makeTileLayers", &Impl::makeTileLayers)
					.addFunction("getTileLayerIndex", &Impl::getTileLayerIndex)
				.endClass()
				.beginClass<ProxyEntity>("Entity")
					.addData("id", &ProxyEntity::m_ID, false)
					.addProperty("layer", &ProxyEntity::getLayer, &ProxyEntity::setLayer)
					.addProperty("sortingLayer", &ProxyEntity::getSortingLayer, &ProxyEntity::setSortingLayer)
				.endClass();

			doLuaFile(*L, m_rData.config.initialScript);

			std::string packageName = getPackageName(m_rData.config.initialScript);

			luabridge::LuaRef pkg = luabridge::getGlobal(L, packageName.c_str());
			if (!pkg.isTable()) throw std::runtime_error{ "Package `" + packageName + "' not found. Package name must match filename." };

			luabridge::LuaRef init = pkg["init"];
			if (!init.isFunction()) throw std::runtime_error{ "Must supply init function." };

			init(this);
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

		unsigned getTileLayerIndex(ResourceID<TileMapLayer> id)
		{
			return m_rData.mapLayerHolder.get(id).getIndex();
		}
	};

	ScriptInit::ScriptInit(GameData& data)
		: m_pImpl{std::make_unique<Impl>(data)}
	{}
	ScriptInit::ScriptInit(ScriptInit&&) = default;
	ScriptInit& ScriptInit::operator=(ScriptInit&&) = default;
	ScriptInit::~ScriptInit() = default;
}
