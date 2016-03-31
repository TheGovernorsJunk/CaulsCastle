#ifndef TE_INDEXED_PRIORITY_QUEUE_H
#define TE_INDEXED_PRIORITY_QUEUE_H

#include <queue>

namespace te
{
	template <class T>
	class IndexedPriorityQueue
	{
	public:
		IndexedPriorityQueue(const std::vector<T>& keys)
			: mCompare([&keys](size_t lhs, size_t rhs) { return keys.at(lhs) > keys.at(rhs); })
			, mPriorityQueue(mCompare)
		{
		}

		void insert(size_t index)
		{
			mPriorityQueue.push(index);
		}

		bool empty() const
		{
			return mPriorityQueue.empty();
		}

		size_t pop()
		{
			size_t index = mPriorityQueue.top();
			mPriorityQueue.pop();
			return index;
		}

		std::vector<size_t> popAll()
		{
			std::vector<size_t> elems;
			elems.reserve(mPriorityQueue.size());
			while (!mPriorityQueue.empty())
			{
				elems.push_back(pop());
			}
			return elems;
		}

		void assignElements(std::vector<size_t>&& elems)
		{
			mPriorityQueue = std::priority_queue<size_t, std::vector<size_t>, std::function<bool(size_t, size_t)>>(mCompare, std::move(elems));
		}

	private:
		std::function<bool(size_t, size_t)> mCompare;
		std::priority_queue<size_t, std::vector<size_t>, std::function<bool(size_t, size_t)>> mPriorityQueue;
	};
}

#endif
