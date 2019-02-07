//
// Created by demeuren on 18/06/18.
//

#pragma  once

#include <cstdlib>

/*
 * a variant of malloc that calls the default constructor
 * useful to minimize modifications of a source code
 * note that it can be replaced by `new T[elementNumber]`
 */
template <typename T>
void* Shaman_malloc(size_t size)
{
    size_t elementNumber = size / sizeof(T);
    return new T[elementNumber];
}

/*
 * a variant of free that insures the use of the destructor
 * useful to minimize modifications to a source code
 * note that it can be replaced by `delete [] ptr`
 */
template <typename T>
void Shaman_free(void* ptr)
{
    delete [] (static_cast<T*>(ptr));
}

