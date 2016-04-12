#ifndef TE_TILE_MAP_H
#define TE_TILE_MAP_H

#include "sparse_graph.h"
#include "tmx.h"
#include "composite_collider.h"
#include "cell_space_partition.h"
#include "base_game_entity.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace te
{
	class Game;
	class TextureManager;

	class TileMap : public BaseGameEntity, public Collidable
	{
	public:
		typedef SparseGraph<NavGraphNode, NavGraphEdge> NavGraph;
		typedef CellSpacePartition<const NavGraph::Node*> NavCellSpace;

		TileMap(Game& world, TextureManager& textureManager, const TMX& tmx, int widthUnitsPerTile = 0, int heightUnitsPerTile = 0);

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
		class Layer : public SceneNode
		{
		public:
			Layer(Game& world, std::vector<sf::VertexArray>&&, std::vector<sf::Texture*>&);
		private:
			void onDraw(sf::RenderTarget&, sf::RenderStates) const;

			std::vector<sf::VertexArray> mVertexArrays;
			std::vector<sf::Texture*>* mTextures;
		};
		enum DrawFlags
		{ COLLIDER = 0x01, NAV_GRAPH = 0x02 };

		TileMap(const TileMap&) = delete;
		TileMap& operator=(const TileMap&) = delete;

		virtual void onDraw(sf::RenderTarget&, sf::RenderStates) const;

		Game& mWorld;

		std::vector<sf::Texture*> mTextures;
		//std::vector<std::vector<sf::VertexArray>> mLayers;
		std::unique_ptr<CompositeCollider> mpCollider;
		std::unique_ptr<NavGraph> mpNavGraph;

		int mDrawFlags;
		float mCellSpaceNeighborhoodRange;
		std::unique_ptr<NavCellSpace> mpCellSpacePartition;
	};
}

#endif
