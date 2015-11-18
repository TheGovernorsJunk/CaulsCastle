#include <tmx.h>

#include <gtest/gtest.h>

namespace te
{
    TEST(TMX, Exception) {
        EXPECT_THROW(TMX("path/only/"), BadFilename) << "Argument must contain file.";
    }
}
