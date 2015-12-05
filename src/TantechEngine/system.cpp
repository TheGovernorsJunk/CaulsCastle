#include "system.h"

namespace te
{
    System::System(const ECS& ecs)
        : mECS(ecs) {}

    System::~System()
    {}
}
