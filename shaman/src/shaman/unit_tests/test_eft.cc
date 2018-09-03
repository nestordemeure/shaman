#include "../eft.h"

#include <gtest/gtest.h>

/*
 * NOTE avoid power of two in tests, they tend to produce correct roundings a lot more often
 */
namespace
{
    /*
     * test whether an EFT is computed accurately in case of cancellation
     * x is much larger than y
     */
    template<typename F>
    bool test_cancellation(F x, F y)
    {
        F z = x + y;
        F error = EFT::TwoSum(x, y, z);
        return (error == y);
    }

    /*
     * test whether an EFT is computed accurately in case of exact subtraction
     * y/2 < x < y*2
     * (requires larger bound to make sure that rounding will not get lucky)
     */
    template<typename F>
    bool test_exactSub(F x, F y)
    {
        F z = x - y;
        F error = EFT::TwoSum(x, -y, z);
        return (error == 0);
    }

    TEST(EFT, float)
    {
        // cancellation
        EXPECT_TRUE(test_cancellation<float>(1e30, 1.1));
        EXPECT_FALSE(test_cancellation<float>(1.2, 1.2));
        // exact subtraction
        EXPECT_TRUE(test_exactSub<float>(1.3, 0.7));
        EXPECT_TRUE(test_exactSub<float>(1.4, 2.1));
        EXPECT_FALSE(test_exactSub<float>(1.5, 0.3));
        EXPECT_FALSE(test_exactSub<float>(1.7, 4.7));
    }

    TEST(EFT, double)
    {
        // cancellation
        EXPECT_TRUE(test_cancellation<double>(1e30, 1.1));
        EXPECT_FALSE(test_cancellation<double>(1.2, 1.2));
        // exact subtraction
        EXPECT_TRUE(test_exactSub<double>(1.3, 0.7));
        EXPECT_TRUE(test_exactSub<double>(1.4, 2.1));
        EXPECT_FALSE(test_exactSub<double>(1.5, 0.3));
        EXPECT_FALSE(test_exactSub<double>(1.7, 4.7));
    }
}