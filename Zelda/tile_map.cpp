#include "tile_map.h"
#include "texture_manager.h"

#include <algorithm>

namespace te
{
	TileMap::TileMap(TextureManager& textureManager, TMX&& tmx)
		: mTMX(std::move(tmx))
		, mTextures()
		, mLayers()
		, mCollider(tmx.makeCollider())
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
	}
}
