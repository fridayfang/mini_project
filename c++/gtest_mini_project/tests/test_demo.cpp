
#include "../src/my_even.h"
#include <gtest/gtest.h>

TEST(YourTest, IsEven) {
    EXPECT_TRUE(is_even(2));
    EXPECT_FALSE(is_even(3));
}
