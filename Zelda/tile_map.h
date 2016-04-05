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

	class TileMap : public sf::Drawable, public sf::Transformable, public Collidable
	{
	public:
		typedef SparseGraph<NavGraphNode, NavGraphEdge> NavGraph;
		typedef CellSpacePartition<const NavGraph::Node*> NavCellSpace;

		TileMap(TextureManager& textureManager, const TMX& tmx, int widthUnitsPerTile = 0, int heightUnitsPerTile = 0);

		const std::vector<Wall2f>& getWalls() const;
		const NavGraph& getNavGraph() const;

		void setDrawColliderEnabled(bool enabled);
		void setDrawNavGraphEnabled(bool enabled);

		float getCellSpaceNeighborhoodRange() const;
		NavCellSpace& getCellSpace();

		bool intersects(const BoxCollider&) const;
		bool intersects(const BoxCollider&, sf::FloatRect&) const;
		bool intersects(const CompositeCollider&) const;
		bool intersects(const CompositeCollider&, sf::FloatRect&) const;

	private:
		enum DrawFlags
		{ COLLIDER = 0x01, NAV_GRAPH = 0x02 };

		TileMap(const TileMap&) = delete;
		TileMap& operator=(const TileMap&) = delete;

		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

		std::vector<std::shared_ptr<sf::Texture>> mTextures;
		std::vector<std::vector<sf::VertexArray>> mLayers;
		std::unique_ptr<CompositeCollider> mpCollider;
		std::unique_ptr<NavGraph> mpNavGraph;

		int mDrawFlags;
		float mCellSpaceNeighborhoodRange;
		std::unique_ptr<NavCellSpace> mpCellSpacePartition;
	};
}

#endif
