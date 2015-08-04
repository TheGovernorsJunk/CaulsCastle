#ifndef TE_COMPONENT_H
#define TE_COMPONENT_H

#include <functional>
#include <vector>
#include <map>
#include <algorithm>
#include "entity_manager.h"

namespace te
{
    template <class Instance>
    class Component
    {
    protected:
        struct Entry
        {
            Entity entity;
            Instance instance;
        };

        Component(std::size_t capacity = 1024)
            : mData()
            , mMap()
        {
            mData.reserve(capacity);
        }
        virtual ~Component() {}

        Instance& createInstance(const Entity& entity, Instance&& instance)
        {
            if (hasInstance(entity))
            {
                throw std::runtime_error("Instance already exists for entity.");
            }

            if (mData.size() == mData.capacity())
            {
                mData.reserve(mData.capacity() + 1024);
            }
            unsigned i = mData.size();
            mData.push_back(std::move(Entry{ entity, instance }));
            mMap.insert(std::make_pair(entity, i));

            return at(entity);
        }

        bool hasInstance(const Entity& entity) const
        {
            return mMap.find(entity) != mMap.end();
        }

        const Instance& at(const Entity& entity) const
        {
            auto it = mMap.find(entity);
            if (it == mMap.end()) { throw std::out_of_range("No instance for entity."); }
            return mData[it->second].instance;
        }

        Instance& at(const Entity& entity)
        {
            return const_cast<Instance&>(static_cast<const Component&>(*this).at(entity));
        }

        virtual void destroyInstance(const Entity& entity)
        {
            auto it = mMap.find(entity);
            if (it == mMap.end()) { return; }

            unsigned i = it->second;
            unsigned lastI = mData.size() - 1;
            Entity lastEntity = mData[lastI].entity;

            mData[i] = mData[lastI];

            mMap[lastEntity] = i;

            mMap.erase(it);
            mData.pop_back();
        }

    public:
        void forEach(std::function<void(const Entity&, Instance&)> f)
        {
            std::for_each(std::begin(mData), std::end(mData), [&f](Entry& entry)
            {
                f(entry.entity, entry.instance);
            });
        }
    private:
        std::vector<Entry> mData;
        std::map<Entity, unsigned> mMap;
    };
}

#endif
