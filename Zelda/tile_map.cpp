#include "tile_map.h"
#include "texture_manager.h"

#include <algorithm>

namespace te
{
	TileMap::TileMap(TextureManager& textureManager, TMX&& tmx)
		: mTMX(std::move(tmx))
		, mTextures()
		, mLayers()
		, mCollider(mTMX.makeCollider())
		, mNavGraph(mTMX.makeNavGraph())
		, mDrawFlags(0)
	{
		mTMX.makeVertices(textureManager, mTextures, mLayers);
		std::for_each(mLayers.begin(), mLayers.end(), [this](const auto& layerComponents) {
			if (layerComponents.size() != mTextures.size()) {
				throw std::runtime_error("Texture and layer component counts are inconsistent.");
			}
		});
	}

	const std::vector<Wall2f>& TileMap::getWalls() const
	{
		return mCollider.getWalls();
	}

	const TileMap::NavGraph& TileMap::getNavGraph() const
	{
		return mNavGraph;
	}

	TileMap::NavGraph& TileMap::getNavGraph()
	{
		return mNavGraph;
	}

	void TileMap::setDrawColliderEnabled(bool enabled)
	{
		mDrawFlags = enabled ? mDrawFlags | COLLIDER : mDrawFlags ^ COLLIDER;
	}

	void TileMap::setDrawNavGraphEnabled(bool enabled)
	{
		mDrawFlags = enabled ? mDrawFlags | NAV_GRAPH : mDrawFlags ^ NAV_GRAPH;
	}

	void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		std::for_each(mLayers.begin(), mLayers.end(), [this, &target, &states](const std::vector<sf::VertexArray>& layerComponents) {
			for (auto iter = layerComponents.begin(); iter != layerComponents.end(); ++iter)
			{
				states.texture = mTextures[iter - layerComponents.begin()].get();
				target.draw(*iter, states);
			}
		});

		states.texture = NULL;
		if ((mDrawFlags & COLLIDER) > 0)
		{
			target.draw(mCollider, states);
		}

		if ((mDrawFlags & NAV_GRAPH) > 0)
		{
			target.draw(mNavGraph, states);
		}
	}

}
