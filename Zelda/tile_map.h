#ifndef TE_TILE_MAP_H
#define TE_TILE_MAP_H

#include "sparse_graph.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace te
{
	class TileMap : public sf::Drawable, public sf::Transformable
	{
	public:
		typedef SparseGraph<NavGraphNode, NavGraphEdge> NavGraph;

		TileMap(const std::vector<std::shared_ptr<sf::Texture>>& textures, const std::vector<std::vector<sf::VertexArray>>& layers);
		// TODO: Constructor with rvalue parameters
	private:
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

		std::vector<std::shared_ptr<sf::Texture>> mTextures;
		std::vector<std::vector<sf::VertexArray>> mLayers;
	};
}

#endif
