#include "tile_map.h"

#include <algorithm>

namespace te
{
	TileMap::TileMap(const std::vector<std::shared_ptr<sf::Texture>>& textures, const std::vector<std::vector<sf::VertexArray>>& layers)
		: mTextures(textures)
		, mLayers(layers)
	{
		std::for_each(mLayers.begin(), mLayers.end(), [this](const auto& layerComponents) {
			if (layerComponents.size() != mTextures.size()) {
				throw std::runtime_error("Texture and layer component counts are inconsistent.");
			}
		});
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
