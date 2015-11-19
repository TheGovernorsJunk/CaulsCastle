#ifndef TE_DATA_COMPONENT_H
#define TE_DATA_COMPONENT_H

#include "component.h"

#include <map>

namespace te
{
    struct DataInstance
    {
        unsigned id;
        std::string name;
        std::map<const std::string, const std::string> misc;

        DataInstance(unsigned id);
    };

    class DataComponent : public Component<DataInstance>
    {
    public:
        DataComponent(std::size_t capacity = 1024);

        void create(const Entity& entity, unsigned id);

        void setData(const Entity& entity, const std::map<const std::string, const std::string>& dataMap);
        void setData(const Entity& entity, const std::pair<const std::string, const std::string>& keyValue);

        Entity getEntity(unsigned id) const;

        void destroyInstance(const Entity& entity);
    private:
        DataComponent(const DataComponent&) = delete;
        DataComponent& operator=(const DataComponent&) = delete;

        std::map<unsigned, const Entity> mEntityIDs;
    };

    typedef std::shared_ptr<DataComponent> DataPtr;

    class ExistingDataException : public std::runtime_error
    {
    public:
        ExistingDataException(const char* msg);
    };

    class NoDataException : public std::runtime_error
    {
    public:
        NoDataException(const char* msg);
    };
}

#endif
