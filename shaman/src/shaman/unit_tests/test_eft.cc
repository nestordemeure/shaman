#include <utility> // for std::swap
#include "../eft.h"

#include <gtest/gtest.h>

namespace {
/*
 * small test to check whether the EFT for the sum is computed accurately
 * TODO : this test pass even with -Ofast -ffast-math
 */
template<typename F>
bool test_eft(F x, F y) {
//    F x = 1e30;
//    F y = 1.f;
    F z = x + y;
    F error = EFT::TwoSum(x, y, z);

    return (error == y);
}

TEST(EFT, float) {
    EXPECT_TRUE(test_eft<float>(1e30, 1.f));
}

TEST(EFT, double) {
  EXPECT_TRUE(test_eft<double>(1e30, 1.));
  EXPECT_FALSE(test_eft<double>(1., 1.));
}

}