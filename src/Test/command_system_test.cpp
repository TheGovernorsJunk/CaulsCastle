#include <command_system.h>

#include <gtest/gtest.h>

namespace te
{
    TEST(CommandSystemTest, ConstructorException)
    {
        EXPECT_THROW(CommandSystem(nullptr), std::runtime_error);
    }
}
