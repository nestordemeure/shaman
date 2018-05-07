#ifndef SHAMAN_TEST_LOOP_H
#define SHAMAN_TEST_LOOP_H

#include "../shaman/Shaman.h"

void loop_test()
{
    {
        Sdouble x = 0.;

        while(x < 1.)
        {
            x += 0.01;
        }

        std::cout << "x=" << x << std::endl;
    }

    {
        Sdouble x = 0.;

        LOOP_BEGIN(x);
        while LOOP_COND(lower(x, Sdouble(1.)))
        {
            x += 0.01;
        }
        LOOP_END(x);

        std::cout << "x=" << x << std::endl;
    }
}

#endif //SHAMAN_TEST_LOOP_H