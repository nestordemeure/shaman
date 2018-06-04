#ifndef SHAMAN_TEST_LOOP_H
#define SHAMAN_TEST_LOOP_H

#include "../shaman/Shaman.h"

/*
 * a test designed to produce an arbitrary errors using the pattern :
 *
 * x <- initialisation()
 * while not valid(x) do
 *    x <- correction(x)
 * return x
 *
 * shaman will take the initialisation and correction errors into account in the quality of the result
 * but they have no impact since we use an iterativ process that validates x
 * (we can even make those errors arbitrarly large)
 * such cases can be detected and manually corrected with tagged errors
 *
 * asynchronous stochastic approaches can deal with such algorithms
 * but, for the same reasons, they fail for systems that converges toward a wrong answer
 */
void test_loop()
{
    Sdouble finalError = 2.;
    Sdouble bigNumber = 1e100;
    Sdouble result = (finalError + bigNumber) - bigNumber;
    std::cout << "intermediate result = " << result << std::endl;

    Sdouble targetNumber = 1.;
    Sdouble epsilon = 0.1;
    Sdouble minThreshold = targetNumber - epsilon / 2.;
    Sdouble maxThreshold = targetNumber + epsilon / 2.;
    while (result <= minThreshold || result >= maxThreshold)
    {
        if (result <= minThreshold)
        {
            result += epsilon;
        }
        else
        {
            result -= epsilon;
        }

        std::cout << result << " (number=" << result.number << " errors=" << result.errors << ')' << std::endl;
    }

    std::cout << "result = " << result << std::endl;
}

#endif //SHAMAN_TEST_LOOP_H