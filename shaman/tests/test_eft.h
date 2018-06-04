#ifndef TEST_EFT_H
#define TEST_EFT_H

#include "../shaman/Shaman.h"

/*
 * small test to check whether the EFT for the sum is computed accurately
 * TODO : this test pass even with -Ofast -ffast-math
 */
void test_eft()
{
    float x = 1e30;
    float y = 1.f;
    float z = x + y;
    float error = EFT::TwoSum(x, y, z);

    std::cout << x << " + " << y << " = " << z << " (errors=" << error << ')' << std::endl;

    if (error != y)
    {
        std::cout << "KO : Test eft::sum failed." << std::endl;
    }
    else
    {
        std::cout << "OK : Test eft::sum pass." << std::endl;
    }
}

#endif // TEST_EFT_H