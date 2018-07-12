#ifndef TEST_EFT_H
#define TEST_EFT_H

#include "../shaman/Shaman.h"
#include <limits>

/*
 * test to check an operation with an arbitrary type
 */
template<typename number, typename FUN>
void test_eft_general(FUN operation, number x, number y, number expectedError, std::string testName)
{
    std::cout << std::scientific << std::setprecision(18);
    auto result = operation(x,y);
    auto error = result.error;

    std::cout << result.number << std::endl;

    if (error == expectedError)
    {
        std::cout << "OK : Test " << testName << " pass : error = " << error << std::endl;
    }
    else
    {
        std::cout << "KO : Test " << testName << " failed : error = " << error << " != " << expectedError << std::endl;
    }
}

/*
 * test the sum operator for all types
 */
void test_operator_sum()
{
    // cancelation
    auto smallNumber = 1;
    test_eft_general<float>([](Sfloat x, Sfloat y){return x+y;}, std::numeric_limits<float>::max()/2, smallNumber, smallNumber, "sum-cancelation-float");
    test_eft_general<double>([](Sdouble x, Sdouble y){return x+y;}, std::numeric_limits<double>::max()/2, smallNumber, smallNumber, "sum-cancelation-double");
    test_eft_general<long double>([](Slong_double x, Slong_double y){return x+y;}, std::numeric_limits<long double>::max()/2, smallNumber, smallNumber, "sum-cancelation-long_double");

    // common operation
    // TODO use std::numeric_limits to produce an example ?
    // TODO KO if double but OK if float !!!
    float x = 0.3;
    float y = 0.2;
    float errorf = ((long double)x+(long double)y) - ((float)x+(float)y);
    double errord = ((long double)x+(long double)y) - ((double)x+(double)y);
    long double errorl = ((long double)x+(long double)y) - ((long double)x+(long double)y); // 0 by definition
    test_eft_general([](Sfloat x, Sfloat y){return x+y;}, (float)x, (float)y, errorf, "sum-common-float");
    test_eft_general([](Sdouble x, Sdouble y){return x+y;}, (double)x, (double)y, errord, "sum-common-double");
    test_eft_general([](Slong_double x, Slong_double y){return x+y;}, (long double)x, (long double)y, errorl, "sum-common-long_double");
}

/*
 * small test to check whether the EFT for the sum is computed accurately
 * TODO : this test pass even with -Ofast -ffast-math
 *
 * TODO a good eft test is : operation_precise(x,y) == operation(x,y) +_precise error_eft
 */
void test_eft()
{
    float x = 1e30;
    float y = 1.f;
    float z = x + y;
    float error = EFT::TwoSum(x, y, z);

    std::cout << x << " + " << y << " = " << z << " (error=" << error << ')' << std::endl;

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