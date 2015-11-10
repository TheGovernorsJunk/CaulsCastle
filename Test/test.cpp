#include <wrappers.h>
#include <entity_manager.h>

#include <gtest/gtest.h>

namespace te
{
    class Env : public ::testing::Environment {
    private:
        Initialization init;
    };

    ::testing::Environment* const env = ::testing::AddGlobalTestEnvironment(new Env());

    TEST(EntityManager, basic) {
        te::EntityManager em;
        te::Entity entity = em.create();
        EXPECT_EQ(true, em.isAlive(entity));
        em.destroy(entity);
        EXPECT_EQ(false, em.isAlive(entity));
    }
}
