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
#include <algorithm>

namespace te
{
	class Game;
	class TextureManager;

	class TileMap : public BaseGameEntity, public Collidable
	{
	public:
		using Component = int;

		typedef SparseGraph<NavGraphNode, NavGraphEdge> NavGraph;
		typedef CellSpacePartition<const NavGraph::Node*> NavCellSpace;

		struct Area
		{
			int id;
			std::string name;
			sf::IntRect rect;
		};

		static std::unique_ptr<TileMap> make(Game& world, TextureManager& textureManager, TMX&& tmx);

		template <typename Iter>
		void getAreasInGroup(const std::string& groupName, Iter out) const
		{
			std::vector<TMX::ObjectGroup> objectGroups{mTMX.getObjectGroups()};
			std::for_each(objectGroups.begin(), objectGroups.end(), [&groupName, &out](TMX::ObjectGroup& objectGroup) {
				if (groupName == objectGroup.name)
				{
					std::for_each(objectGroup.objects.begin(), objectGroup.objects.end(), [&out](TMX::Object& object) {
						(out++) = Area{
							object.id,
							object.name,
							sf::IntRect{object.x, object.y, object.width, object.height}
						};
					});
				}
			});
		}

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

		void stitch(sf::Vector2i coords, TileMap& o, sf::Vector2i oCoords) const;

		sf::Transform getTileToWorldTransform() const;

	private:
		TileMap(Game& world, TextureManager& textureManager, TMX&& tmx);

		class Layer : public BaseGameEntity
		{
		public:
			Layer(Game& world, std::vector<sf::VertexArray>&&, std::vector<const sf::Texture*>&);
		private:
			void onDraw(sf::RenderTarget&, sf::RenderStates) const;

			std::vector<sf::VertexArray> mVertexArrays;
			std::vector<const sf::Texture*>* mTextures;
		};
		enum DrawFlags
		{ COLLIDER = 0x01, NAV_GRAPH = 0x02 };

		TileMap(const TileMap&) = delete;
		TileMap& operator=(const TileMap&) = delete;

		virtual void onDraw(sf::RenderTarget&, sf::RenderStates) const;

		TMX mTMX;

		Game& mWorld;

		std::vector<const sf::Texture*> mTextures;
		//std::vector<std::vector<sf::VertexArray>> mLayers;
		std::unique_ptr<CompositeCollider> mpCollider;
		std::unique_ptr<NavGraph> mpNavGraph;

		int mDrawFlags;
		float mCellSpaceNeighborhoodRange;
		std::unique_ptr<NavCellSpace> mpCellSpacePartition;
	};
}

#endif
