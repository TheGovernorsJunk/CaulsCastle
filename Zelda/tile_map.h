#ifndef TE_TILE_MAP_H
#define TE_TILE_MAP_H

#include "sparse_graph.h"
#include "tmx.h"
#include "composite_collider.h"
#include "cell_space_partition.h"

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
		typedef CellSpacePartition<const NavGraph::Node*> NavCellSpace;

		TileMap(TextureManager& textureManager, TMX&& tmx);

		const std::vector<Wall2f>& getWalls() const;
		const NavGraph& getNavGraph() const;

		void setDrawColliderEnabled(bool enabled);
		void setDrawNavGraphEnabled(bool enabled);

		float getCellSpaceNeighborhoodRange() const;
		NavCellSpace& getCellSpace();

	private:
		enum DrawFlags
		{ COLLIDER = 0x01, NAV_GRAPH = 0x02 };

		TileMap(const TileMap&) = delete;
		TileMap& operator=(const TileMap&) = delete;

		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

		TMX mTMX;
		std::vector<std::shared_ptr<sf::Texture>> mTextures;
		std::vector<std::vector<sf::VertexArray>> mLayers;
		CompositeCollider mCollider;
		NavGraph mNavGraph;

		int mDrawFlags;
		float mCellSpaceNeighborhoodRange;
		NavCellSpace mCellSpacePartition;
	};
}

#endif
