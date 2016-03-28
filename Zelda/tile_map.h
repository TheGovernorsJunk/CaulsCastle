#ifndef TE_TILE_MAP_H
#define TE_TILE_MAP_H

#include "sparse_graph.h"
#include "tmx.h"
#include "composite_collider.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace te
{
	class TextureManager;

	class TileMap : public sf::Drawable, public sf::Transformable
	{
	public:
		typedef SparseGraph<NavGraphNode, NavGraphEdge> NavGraph;

		TileMap(TextureManager& textureManager, TMX&& tmx);

	private:
		TileMap(const TileMap&) = delete;
		TileMap& operator=(const TileMap&) = delete;

		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

		TMX mTMX;
		std::vector<std::shared_ptr<sf::Texture>> mTextures;
		std::vector<std::vector<sf::VertexArray>> mLayers;
		CompositeCollider mCollider;
	};
}

#endif
