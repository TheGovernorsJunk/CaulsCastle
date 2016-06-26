#include "utilities.h"
#include "game.h"
#include "vector_ops.h"

#include <lua.hpp>
#include <Box2D/Box2D.h>

#include <algorithm>
#include <array>

namespace te
{
	std::string getDir(const std::string& filename, const std::string& delimiter)
	{
		auto result = std::find_end(filename.begin(), filename.end(), delimiter.begin(), delimiter.end());
		return result != filename.end() ? std::string(filename.begin(), result + 1) : "";
	}

	sf::Texture makeIsometricPlaceholder(int scale)
	{
		sf::RenderTexture texture;
		texture.create(32 * scale, 16 * scale);
		texture.setRepeated(true);

		std::array<sf::Vertex, 8> lines = {
			sf::Vertex(sf::Vector2f(17.f, 0)), sf::Vertex(sf::Vector2f(33.f, 8.f)),
			sf::Vertex(sf::Vector2f(32.f, 8.f)), sf::Vertex(sf::Vector2f(16.f, 16.f)),
			sf::Vertex(sf::Vector2f(17.f, 16.f)), sf::Vertex(sf::Vector2f(1, 8.f)),
			sf::Vertex(sf::Vector2f(0, 8.f)), sf::Vertex(sf::Vector2f(16.f, 0))
		};
		std::for_each(lines.begin(), lines.end(), [scale](sf::Vertex& v) {
			v.position = v.position * (float)scale;
		});
		texture.draw(lines.data(), 8, sf::Lines);

		texture.display();

		return texture.getTexture();
	}

	void doLuaFile(lua_State& L, const std::string& filename)
	{
		if (luaL_dofile(&L, filename.c_str())) throw std::runtime_error(lua_tostring(&L, -1));
	}

	class RayCastCallback : public b2RayCastCallback
	{
	public:
		bool mResult;
		RayCastHit& mHitInfo;

		RayCastCallback(RayCastHit& hitInfo)
			: mResult{false}
			, mHitInfo{hitInfo}
		{}

		float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
		{
			mResult = true;
			auto* pNode = static_cast<SceneNode*>(fixture->GetBody()->GetUserData());
			auto* pEntity = dynamic_cast<BaseGameEntity*>(pNode);
			EntityID id = pEntity ? pEntity->getID() : BaseGameEntity::UNREGISTERED_ID;
			mHitInfo = {id, {point.x, point.y}};
			return 0;
		}
	};

	bool rayCast(Game* world, sf::Vector2f origin, sf::Vector2f direction, RayCastHit* hitInfo, float maxDistance)
	{
		assert(world && hitInfo);
		RayCastCallback callback{*hitInfo};
		sf::Vector2f end = normalize(direction) * maxDistance + origin;
		world->getPhysicsWorld().RayCast(&callback, b2Vec2(origin.x, origin.y), b2Vec2(end.x, end.y));
		return callback.mResult;
	}
}
