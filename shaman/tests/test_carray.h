//
// Created by demeuren on 14/06/18.
//

#ifndef SHAMAN_TEST_CARRAY_H
#define SHAMAN_TEST_CARRAY_H

/*
 * trying to reproduce lulesh segfault
 * currently unable to reproduce the problem
 */
void test_cArray()
{
    int size = 100;
    Sdouble *tab = static_cast<Sdouble *>(malloc(sizeof(Sdouble)*size));
    std::vector<Sdouble> vec;
    vec.resize(size);
    /*
    for (int i = 0; i < size; i++)
    {
        tab[i] = vec[i];
    }
     */

    tab[0] = Sdouble(11.);
    std::cout << tab[0] << std::endl;

    tab[0] += 1.;
    std::cout << tab[0] << std::endl;

    free(tab);
}

#endif //SHAMAN_TEST_CARRAY_H
