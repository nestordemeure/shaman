//
// Created by nestor on 18/05/18.
//

#ifndef SHAMAN_DEMO_H
#define SHAMAN_DEMO_H

#include "../../shaman/Shaman.h"

/*
 * computing sqrt(x) using Heron's algorithm
 *
 * iterations with unstable operations (6 being the last iteration) :
 * - cancelation in test : 4,5,6
 * - unstable abs and < in test : 6
 */
void sqrtHeron()
{
    Sdouble x = 2;
    Sdouble sqrtx = x/2;

    while(1e-15 < abs(sqrtx*sqrtx - x))
    {
        sqrtx = (sqrtx + x/sqrtx) / 2;
        printf("result=%.15e error≈%.15e\n", sqrtx.number, sqrtx.error);
    }
}

#endif //SHAMAN_DEMO_H