#include "data_component.h"
#include <cassert>

namespace te
{
    DataInstance::DataInstance(unsigned id)
        : id(id), name(), misc() {}

    DataComponent::DataComponent(std::size_t capacity)
        : Component(capacity)
        , mEntityIDs()
    {}

    void DataComponent::create(const Entity& entity, unsigned id)
    {
        if (hasInstance(entity)) {
            throw ExistingDataException("DataComponent::create: Data already exists for this entity.");
        }
        DataInstance& inst = createInstance(entity, DataInstance(id));
        mEntityIDs.insert({
            id,
            entity
        });
    }

    void DataComponent::setData(const Entity& entity, const std::map<const std::string, const std::string>& dataMap)
    {
        if (!hasInstance(entity)) {
            throw NoDataException("DataComponent::setData: Must create data instance before setting.");
        }

        DataInstance& inst = at(entity);

        for (auto it = dataMap.begin(); it != dataMap.end(); ++it) {
            if (it->first == "name") {
                inst.name = it->second;
            } else {
                inst.misc.insert({
                    it->first,
                    it->second
                });
            }
        }
    }

    void DataComponent::setData(const Entity& entity, const std::pair<const std::string, const std::string>& keyValue)
    {
        if (!hasInstance(entity)) {
            throw NoDataException("DataComponent::setData: Must create data instance before setting.");
        }

        DataInstance& inst = at(entity);

        if (keyValue.first == "name") {
            inst.name = keyValue.second;
        } else {
            inst.misc.insert({
                keyValue.first,
                keyValue.second
            });
        }
    }

    Entity DataComponent::getEntity(unsigned id) const
    {
        auto it = mEntityIDs.find(id);
        if (it == mEntityIDs.end()) {
            throw NoDataException(":DataComponent::getEntity: Entity not found.");
        }
        return it->second;
    }

    void DataComponent::destroyInstance(const Entity& entity)
    {
        if (!hasInstance(entity)) {
            auto it = mEntityIDs.find(at(entity).id);
            mEntityIDs.erase(it);
            Component::destroyInstance(entity);
        }
    }

    ExistingDataException::ExistingDataException(const char* msg)
        : std::runtime_error(msg) {}

    NoDataException::NoDataException(const char* msg)
        : std::runtime_error(msg) {}
}
