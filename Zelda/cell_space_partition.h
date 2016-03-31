#ifndef TE_CELL_SPACE_PARTITION_H
#define TE_CELL_SPACE_PARTITION_H

#include <SFML/Graphics.hpp>

#include <list>
#include <vector>
#include <cassert>

namespace te
{
	template <class Entity>
	struct Cell
	{
		std::list<Entity> members;
		sf::FloatRect boundingBox;

		Cell(sf::FloatRect bb) : boundingBox(bb) {}
	};

	template <class Entity>
	class CellSpacePartition
	{
	public:
		CellSpacePartition(double width, double height, int cellsX, int cellsY, int maxEntities)
			: mCells()
			, mNeighbors(maxEntities, Entity())
			, mCurrNeighbor(mNeighbors.begin())
			, mSpaceWidth(width)
			, mSpaceHeight(height)
			, mNumCellsX(cellsX)
			, mNumCellsY(cellsY)
			, mCellSizeX(width / cellsX)
			, mCellSizeY(height / cellsY)
		{
			for (int y = 0; y < mNumCellsY; ++y)
			{
				for (int x = 0; x < mNumCellsX; ++x)
				{
					mCells.push_back(Cell<Entity>(sf::FloatRect(x * mCellSizeX, y * mCellSizeY, mCellSizeX, mCellSizeY)));
				}
			}
		}

		inline void addEntity(const Entity& entity)
		{
			int index = positionToIndex(entity->getPosition());
			assert(index < (int)mCells.size());
			mCells[index].members.push_back(entity);
		}

		inline void updateEntity(const Entity& entity, sf::Vector2f oldPosition)
		{}

		inline Entity& begin()
		{
			mCurrNeighbor = mNeighbors.begin();
			return *mCurrNeighbor;
		}

		inline Entity& next()
		{
			++mCurrNeighbor;
			return *mCurrNeighbor;
		}

		inline bool end()
		{
			return (mCurrNeighbor == mNeighbors.end() || *mCurrNeighbor == 0);
		}

		void emptyCells()
		{
			for (auto& cell : mCells)
				cell.members.clear();
		}

		void calculateNeighbors(sf::Vector2f targetPos, float queryRadius)
		{
			typename std::vector<Entity>::iterator currNeighbor = mNeighbors.begin();
			sf::FloatRect queryBox{ targetPos.x - queryRadius, targetPos.y - queryRadius, 2 * queryRadius, 2 * queryRadius };

			for (auto& cell : mCells)
			{
				if (cell.boundingBox.intersects(queryBox) && !cell.members.empty())
				{
					for (auto& member : cell.members)
					{
						if (distanceSq(member->getPosition(), targetPos) < queryRadius * queryRadius)
						{
							*currNeighbor++ = member;
						}
					}
				}
			}

			*currNeighbor = 0;
		}

	private:
		inline int positionToIndex(const sf::Vector2f& position) const
		{
			int index = (int)(mNumCellsX * position.x / mSpaceWidth) +
				((int)(mNumCellsY * position.y / mSpaceHeight) * mNumCellsX);
			if (index > (int)mCells.size() - 1)
			{
				index = (int)mCells.size() - 1;
			}
			return index;
		}

		std::vector<Cell<Entity>> mCells;
		std::vector<Entity> mNeighbors;
		typename std::vector<Entity>::iterator mCurrNeighbor;

		float mSpaceWidth;
		float mSpaceHeight;

		int mNumCellsX;
		int mNumCellsY;

		float mCellSizeX;
		float mCellSizeY;
	};
}

#endif
