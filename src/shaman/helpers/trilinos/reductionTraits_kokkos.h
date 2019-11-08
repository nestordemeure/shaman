#pragma  once

#include <Kokkos_NumericTraits.hpp>

/*
 * used for parallel reduce
 */
namespace Kokkos
{
    template<>
    struct reduction_identity<Sfloat> {
        KOKKOS_FORCEINLINE_FUNCTION static Sfloat sum()  {return static_cast<Sfloat>(0.0f);}
        KOKKOS_FORCEINLINE_FUNCTION static Sfloat prod() {return static_cast<Sfloat>(1.0f);}
        KOKKOS_FORCEINLINE_FUNCTION static Sfloat max()  {return -FLT_MAX;}
        KOKKOS_FORCEINLINE_FUNCTION static Sfloat min()  {return FLT_MAX;}
    };

    template<>
    struct reduction_identity<Sdouble> {
        KOKKOS_FORCEINLINE_FUNCTION static Sdouble sum()  {return static_cast<Sdouble>(0.0);}
        KOKKOS_FORCEINLINE_FUNCTION static Sdouble prod() {return static_cast<Sdouble>(1.0);}
        KOKKOS_FORCEINLINE_FUNCTION static Sdouble max()  {return -DBL_MAX;}
        KOKKOS_FORCEINLINE_FUNCTION static Sdouble min()  {return DBL_MAX;}
    };

    template<>
    struct reduction_identity<Slong_double> {
        KOKKOS_FORCEINLINE_FUNCTION static Slong_double sum()  {return static_cast<Slong_double>(0.0L);}
        KOKKOS_FORCEINLINE_FUNCTION static Slong_double prod() {return static_cast<Slong_double>(1.0L);}
        KOKKOS_FORCEINLINE_FUNCTION static Slong_double max()  {return -LDBL_MAX;}
        KOKKOS_FORCEINLINE_FUNCTION static Slong_double min()  {return LDBL_MAX;}
    };
}

