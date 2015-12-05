#ifndef TE_SYSTEM_H
#define TE_SYSTEM_H

#include "ecs.h"

#include <cassert>

namespace te
{
    class System
    {
    public:
        System(const ECS& ecs);
        virtual ~System();

    protected:
        template <typename T>
        T& get() const;

        template<> TransformComponent& get() const
        { 
            assert(mECS.pTransformComponent);
            return *mECS.pTransformComponent;
        }
        template<> AnimationComponent& get() const
        {
            assert(mECS.pAnimationComponent);
            return *mECS.pAnimationComponent;
        }
        template<> DataComponent& get() const
        {
            assert(mECS.pDataComponent);
            return *mECS.pDataComponent;
        }
        template<> CommandComponent& get() const
        {
            assert(mECS.pCommandComponent);
            return *mECS.pCommandComponent;
        }
        template<> EntityManager& get() const
        {
            assert(mECS.pEntityManager);
            return *mECS.pEntityManager;
        }

    private:
        const ECS mECS;
    };
}

#endif
