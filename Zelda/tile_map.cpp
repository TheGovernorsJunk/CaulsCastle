#include "tile_map.h"
#include "texture_manager.h"
#include "vector_ops.h"

#include <algorithm>

namespace te
{
	static float calculateAverageGraphEdgeLength(const TileMap::NavGraph& navGraph)
	{
		float totalLength = 0;
		int numEdgesCounted = 0;

		TileMap::NavGraph::ConstNodeIterator nodeIter(navGraph);
		for (const TileMap::NavGraph::Node* pNode = nodeIter.begin(); !nodeIter.end(); pNode = nodeIter.next())
		{
			TileMap::NavGraph::ConstEdgeIterator edgeIter(navGraph, pNode->getIndex());
			for (const TileMap::NavGraph::Edge* pEdge = edgeIter.begin(); !edgeIter.end(); pEdge = edgeIter.next())
			{
				++numEdgesCounted;
				totalLength += distance(navGraph.getNode(pEdge->getFrom()).getPosition(), navGraph.getNode(pEdge->getTo()).getPosition());
			}
		}

		return totalLength / numEdgesCounted;
	}

	TileMap::TileMap(TextureManager& textureManager, const TMX& tmx, int widthUnitsPerTile, int heightUnitsPerTile)
		: mTextures()
		, mLayers()
		, mpCollider(nullptr)
		, mpNavGraph(nullptr)
		, mDrawFlags(0)
		, mCellSpaceNeighborhoodRange(1)
		, mpCellSpacePartition(nullptr)
	{
		tmx.makeVertices(textureManager, mTextures, mLayers, widthUnitsPerTile, heightUnitsPerTile);
		std::for_each(mLayers.begin(), mLayers.end(), [this](const auto& layerComponents) {
			if (layerComponents.size() != mTextures.size()) {
				throw std::runtime_error("Texture and layer component counts are inconsistent.");
			}
		});

		sf::Transform transform;
		transform.scale((float)widthUnitsPerTile / tmx.getTileWidth(), (float)heightUnitsPerTile / tmx.getTileHeight());

		mpCollider = std::unique_ptr<CompositeCollider>(tmx.makeCollider(transform));

		mpNavGraph = std::unique_ptr<NavGraph>(tmx.makeNavGraph(transform));

		mCellSpaceNeighborhoodRange = calculateAverageGraphEdgeLength(*mpNavGraph) + 1;

		mpCellSpacePartition = std::make_unique<NavCellSpace>(tmx.getWidth() * tmx.getTileWidth(), tmx.getHeight() * tmx.getTileHeight(), tmx.getWidth() / 4, tmx.getHeight() / 4, mpNavGraph->numNodes());

		TileMap::NavGraph::ConstNodeIterator nodeIter(*mpNavGraph);
		for (const TileMap::NavGraph::Node* pNode = nodeIter.begin(); !nodeIter.end(); pNode = nodeIter.next())
		{
			mpCellSpacePartition->addEntity(pNode);
		}
	}

	const std::vector<Wall2f>& TileMap::getWalls() const
	{
		return mpCollider->getWalls();
	}

	const TileMap::NavGraph& TileMap::getNavGraph() const
	{
		return *mpNavGraph;
	}

	void TileMap::setDrawColliderEnabled(bool enabled)
	{
		mDrawFlags = enabled ? mDrawFlags | COLLIDER : mDrawFlags ^ COLLIDER;
	}

	void TileMap::setDrawNavGraphEnabled(bool enabled)
	{
		mDrawFlags = enabled ? mDrawFlags | NAV_GRAPH : mDrawFlags ^ NAV_GRAPH;
		if (enabled)
			mpNavGraph->prepareVerticesForDrawing();
	}

	float TileMap::getCellSpaceNeighborhoodRange() const
	{
		return mCellSpaceNeighborhoodRange;
	}

	TileMap::NavCellSpace& TileMap::getCellSpace()
	{
		return *mpCellSpacePartition;
	}

	bool TileMap::intersects(const BoxCollider& o) const
	{
		return mpCollider->transform(getTransform()).intersects(o);
	}

	bool TileMap::intersects(const BoxCollider& o, sf::FloatRect& collision) const
	{
		return mpCollider->transform(getTransform()).intersects(o, collision);
	}

	bool TileMap::intersects(const CompositeCollider& o) const
	{
		return mpCollider->transform(getTransform()).intersects(o);
	}

	bool TileMap::intersects(const CompositeCollider& o, sf::FloatRect& collision) const
	{
		return mpCollider->transform(getTransform()).intersects(o, collision);
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
			target.draw(*mpCollider, states);
		}

		if ((mDrawFlags & NAV_GRAPH) > 0)
		{
			target.draw(*mpNavGraph, states);
		}
	}

}
