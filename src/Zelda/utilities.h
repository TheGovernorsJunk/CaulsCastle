#ifndef TE_UTILITIES_H
#define TE_UTILITIES_H

#include "typedefs.h"
#include "base_game_entity.h"
#include "tmx.h"
#include "game_data.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <limits>

struct lua_State;

namespace te
{
	class Game;

	const float PI = 3.14159265358979323846f;

	std::string getDir(const std::string& filename, const std::string& delimiter = "/");

	sf::Texture makeIsometricPlaceholder(int scale = 1);

	void doLuaFile(lua_State& L, const std::string& filename);

	struct RayCastHit
	{
		EntityID entityID;
		sf::Vector2f point;
		RayCastHit(EntityID id = BaseGameEntity::UNREGISTERED_ID, sf::Vector2f p = {0, 0})
			: entityID{id}
			, point{p}
		{}
	};
	bool rayCast(Game* world, sf::Vector2f origin, sf::Vector2f direction, RayCastHit* hitInfo, float maxDistance = std::numeric_limits<float>::max() * 0.5f);

	template <typename Iter>
	void getTilesetFilenames(const TMX& tmx, Iter out)
	{
		std::string dir = getDir(tmx.getFilename());
		for (auto it = tmx.tilesetsBegin(); it != tmx.tilesetsEnd(); ++it)
		{
			out++ = dir + it->image.source;
		}
	}

	template <typename Iter>
	void getObjectsInGroup(const TMX& tmx, const std::string& groupName, Iter out)
	{
		std::vector<TMX::ObjectGroup> objectGroups{ tmx.getObjectGroups() };
		std::for_each(objectGroups.begin(), objectGroups.end(), [&groupName, &out](TMX::ObjectGroup& objectGroup) {
			if (groupName == objectGroup.name)
			{
				std::for_each(objectGroup.objects.begin(), objectGroup.objects.end(), [this, &out](TMX::Object& object) {
					(out++) = object;
				});
			}
		});
	}

	template <typename Resource>
	inline Resource& get(GameData& data, ResourceID<Resource> id);

	template <>
	inline TMX& get(GameData& data, ResourceID<TMX> id)
	{
		return data.tmxHolder.get(id);
	}

	template <typename Resource>
	inline ResourceID<Resource> load(GameData& data, const std::string& filename);

	template <>
	inline ResourceID<TextureAtlas> load(GameData& data, const std::string& filename)
	{
		return data.atlasHolder.load(filename);
	}
}

#endif
