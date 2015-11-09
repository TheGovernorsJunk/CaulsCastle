#include <entity_manager.h>

#include <gtest/gtest.h>

TEST(EntityManager, basic) {
    te::EntityManager em;
    te::Entity entity = em.create();
    EXPECT_EQ(true, em.isAlive(entity));
    em.destroy(entity);
    EXPECT_EQ(false, em.isAlive(entity));
}
