//
// Created by nestor on 18/05/18.
//

#ifndef SHAMAN_DEMO_H
#define SHAMAN_DEMO_H

#include "../../shaman/Shaman.h"

void demo()
{
    Sdouble u0 = 0.;
    Sdouble u1 = 1.;

    for(int i = 1; i < 100; i++)
    {
        Sdouble u2 = u0 + u1;
        u0 = u1;
        u1 = u2;
        std::cout << "output:" << u2
                  << " number:" << u2.number
                  << " error:" << u2.error << std::endl;
    }
}

#endif //SHAMAN_DEMO_H