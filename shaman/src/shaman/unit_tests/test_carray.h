//
// Created by demeuren on 14/06/18.
//

#ifndef SHAMAN_TEST_CARRAY_H
#define SHAMAN_TEST_CARRAY_H

std::vector<Sdouble> vec;

/*
 * trying to reproduce lulesh segfault
 * currently unable to reproduce the problem
 */
void test_cArray()
{
    int size = 10;

    vec.resize(size);

    for(int j = 0; j < size; j++)
    {
        Sdouble *tab = static_cast<Sdouble *>(malloc(sizeof(Sdouble)*size));

        std::cerr << "loop" << std::endl;

        for (int i = 0; i < size; i++)
        {
            tab[i] = vec[i];
        }

        free(tab);
    }
}

#endif //SHAMAN_TEST_CARRAY_H
