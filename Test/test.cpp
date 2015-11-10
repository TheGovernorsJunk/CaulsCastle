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

    TEST(EntityManager, Basic) {
        te::EntityManager em;
        te::Entity entity = em.create();
        EXPECT_EQ(true, em.isAlive(entity));
        em.destroy(entity);
        EXPECT_EQ(false, em.isAlive(entity));
    }

    //class WindowTest : public ::testing::Test {
    //public:
    //    WindowPtr pWindow;
    //    WindowTest() : pWindow(createWindowOpenGL("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 32, 32, SDL_WINDOW_SHOWN)) {}
    //};

    //TEST_F(WindowTest, Basic) {
    //}
}
