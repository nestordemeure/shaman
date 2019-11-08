#pragma  once

#include <Kokkos_ArithTraits.hpp>
#include <Kokkos_Random.hpp>

/*
 * defines arithmetic traits needed by Kokkos
 * see Kokkos_ArithTraits.hpp for examples
 */
namespace Kokkos
{
    namespace Details
    {
        template<> class ArithTraits<Sfloat>
        {
            public:
            typedef Sfloat val_type;
            typedef val_type mag_type;

            static const bool is_specialized = true;
            static const bool is_signed = true;
            static const bool is_integer = false;
            static const bool is_exact = false;
            static const bool is_complex = false;
            static constexpr bool has_infinity = true;

            static KOKKOS_FORCEINLINE_FUNCTION Sfloat infinity() { return HUGE_VALF; }
            static KOKKOS_FORCEINLINE_FUNCTION bool isInf (const Sfloat x) { return Sstd::isinf(x); }
            static KOKKOS_FORCEINLINE_FUNCTION bool isNan (const Sfloat x) { return Sstd::isnan(x); }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type abs (const Sfloat x) { return Sstd::abs(x); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat zero () { return 0.0f; }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat one () { return 1.0f; }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat min () { return -FLT_MAX; }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat max () { return FLT_MAX; }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type real (const Sfloat x) { return x; }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type imag (const Sfloat) { return zero(); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat conj (const Sfloat x) { return x; }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat pow (const Sfloat x, const Sfloat y) { return Sstd::pow(x, y); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat sqrt (const Sfloat x) { return Sstd::sqrt(x); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat cbrt (const Sfloat x) { return Sstd::cbrt(x); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat exp (const Sfloat x) { return Sstd::exp(x); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat log (const Sfloat x) { return Sstd::log(x); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat log10 (const Sfloat x) { return Sstd::log10(x); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat sin (const Sfloat x) { return Sstd::sin(x); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat cos (const Sfloat x) { return Sstd::cos(x); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat tan (const Sfloat x) { return Sstd::tan(x); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat sinh (const Sfloat x) { return Sstd::sinh (x); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat cosh (const Sfloat x) { return Sstd::cosh (x); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat tanh (const Sfloat x) { return Sstd::tanh (x); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat asin (const Sfloat x) { return Sstd::asin (x); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat acos (const Sfloat x) { return Sstd::acos (x); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat atan (const Sfloat x) { return Sstd::atan (x); }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type epsilon () { return FLT_EPSILON; }

            typedef mag_type magnitudeType;
            typedef Sfloat halfPrecision; // C++ doesn't have a standard "half-float" type.
            typedef Sdouble doublePrecision;
            static const bool isComplex = false;
            static const bool isOrdinal = false;
            static const bool isComparable = true;
            static const bool hasMachineParameters = true;

            static KOKKOS_FORCEINLINE_FUNCTION bool isnaninf (const Sfloat x) { return isNan (x) || isInf (x); }
            static KOKKOS_FORCEINLINE_FUNCTION magnitudeType magnitude (const Sfloat x) { return abs (x); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat conjugate (const Sfloat x) { return conj (x); }
            static std::string name () { return "Sfloat"; }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat squareroot (const Sfloat x) { return sqrt (x); }
            static KOKKOS_FORCEINLINE_FUNCTION Sfloat nan () { return std::numeric_limits<float>::quiet_NaN(); }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type eps () { return epsilon (); }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type sfmin () { return FLT_MIN; }
            static KOKKOS_FORCEINLINE_FUNCTION int base () { return FLT_RADIX; }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type prec () { return eps () * static_cast<float> (base ()); }
            static KOKKOS_FORCEINLINE_FUNCTION int t () { return FLT_MANT_DIG; }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type rnd () { return one(); }
            static KOKKOS_FORCEINLINE_FUNCTION int emin () { return FLT_MIN_EXP; }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type rmin () { return FLT_MIN; }
            static KOKKOS_FORCEINLINE_FUNCTION int emax () { return FLT_MAX_EXP; }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type rmax () { return FLT_MAX; }
        };

        template<> class ArithTraits<Sdouble>
        {
            public:
            typedef Sdouble val_type;
            typedef val_type mag_type;

            static const bool is_specialized = true;
            static const bool is_signed = true;
            static const bool is_integer = false;
            static const bool is_exact = false;
            static const bool is_complex = false;
            static constexpr bool has_infinity = true;

            static KOKKOS_FORCEINLINE_FUNCTION Sdouble infinity() { return HUGE_VAL; }
            static KOKKOS_FORCEINLINE_FUNCTION bool isInf (const val_type x) { return Sstd::isinf(x); }
            static KOKKOS_FORCEINLINE_FUNCTION bool isNan (const val_type x) { return Sstd::isnan(x); }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type abs (const val_type x) { return Sstd::abs(x); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type zero () { return 0.0; }
            static KOKKOS_FORCEINLINE_FUNCTION val_type one () { return 1.0; }
            static KOKKOS_FORCEINLINE_FUNCTION val_type min () { return -DBL_MAX; }
            static KOKKOS_FORCEINLINE_FUNCTION val_type max () { return DBL_MAX; }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type real (const val_type x) { return x; }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type imag (const val_type) { return zero(); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type conj (const val_type x) { return x; }
            static KOKKOS_FORCEINLINE_FUNCTION val_type pow (const val_type x, const val_type y) { return Sstd::pow(x, y); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type sqrt (const val_type x) { return Sstd::sqrt(x); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type cbrt (const val_type x) { return Sstd::cbrt(x); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type exp (const val_type x) { return Sstd::exp(x); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type log (const val_type x) { return Sstd::log (x); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type log10 (const val_type x) { return Sstd::log10 (x); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type sin (const val_type x) { return Sstd::sin (x); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type cos (const val_type x) { return Sstd::cos (x); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type tan (const val_type x) { return Sstd::tan (x); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type sinh (const val_type x) { return Sstd::sinh (x); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type cosh (const val_type x) { return Sstd::cosh (x); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type tanh (const val_type x) { return Sstd::tanh (x); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type asin (const val_type x) { return Sstd::asin (x); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type acos (const val_type x) { return Sstd::acos (x); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type atan (const val_type x) { return Sstd::atan (x); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type nan () { return std::numeric_limits<double>::quiet_NaN(); }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type epsilon () { return DBL_EPSILON; }

            typedef mag_type magnitudeType;
            typedef Sfloat halfPrecision;
            typedef Slong_double doublePrecision;
            static const bool isComplex = false;
            static const bool isOrdinal = false;
            static const bool isComparable = true;
            static const bool hasMachineParameters = true;

            static bool isnaninf (const val_type& x) { return isNan (x) || isInf (x); }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type magnitude (const val_type x) { return abs (x); }
            static KOKKOS_FORCEINLINE_FUNCTION val_type conjugate (const val_type x) { return conj (x); }
            static std::string name () { return "Sdouble"; }
            static KOKKOS_FORCEINLINE_FUNCTION val_type squareroot (const val_type x) { return sqrt (x); }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type eps () { return epsilon (); }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type sfmin () { return DBL_MIN; }
            static KOKKOS_FORCEINLINE_FUNCTION int base () { return FLT_RADIX; }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type prec () { return eps () * static_cast<double> (base ()); }
            static KOKKOS_FORCEINLINE_FUNCTION int t () { return DBL_MANT_DIG; }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type rnd () { return one(); }
            static KOKKOS_FORCEINLINE_FUNCTION int emin () { return DBL_MIN_EXP; }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type rmin () { return DBL_MIN; }
            static KOKKOS_FORCEINLINE_FUNCTION int emax () { return DBL_MAX_EXP; }
            static KOKKOS_FORCEINLINE_FUNCTION mag_type rmax () { return DBL_MAX; }
        };

        template<>
        class ArithTraits<Slong_double>
        {
            public:
            typedef Slong_double val_type;
            typedef Slong_double mag_type;

            static const bool is_specialized = true;
            static const bool is_signed = true;
            static const bool is_integer = false;
            static const bool is_exact = false;
            static const bool is_complex = false;
            static constexpr bool has_infinity = true;

            static KOKKOS_FORCEINLINE_FUNCTION Slong_double infinity() { return HUGE_VALL; }
            static bool isInf (const val_type& x) { return Sstd::isinf(x); }
            static bool isNan (const val_type& x) { return Sstd::isnan(x); }
            static mag_type abs (const val_type& x) { return Sstd::abs(x); }
            static val_type zero () { return 0.0L; }
            static val_type one () { return 1.0L; }
            static val_type min () { return -LDBL_MAX; }
            static val_type max () { return LDBL_MAX; }
            static mag_type real (const val_type& x) { return x; }
            static mag_type imag (const val_type&) { return zero (); }
            static val_type conj (const val_type& x) { return x; }
            static val_type pow (const val_type& x, const val_type& y) { return Sstd::pow(x, y); }
            static val_type sqrt (const val_type& x) { return Sstd::sqrt(x); }
            static val_type cbrt (const val_type& x) { return Sstd::cbrt(x); }
            static val_type exp (const val_type& x) { return Sstd::exp(x); }
            static val_type log (const val_type& x) { return Sstd::log(x); }
            static val_type log10 (const val_type& x) { return Sstd::log10(x); }
            static val_type sin (const val_type& x) { return Sstd::sin(x); }
            static val_type cos (const val_type& x) { return Sstd::cos(x); }
            static val_type tan (const val_type& x) { return Sstd::tan(x); }
            static val_type sinh (const val_type& x) { return Sstd::sinh(x); }
            static val_type cosh (const val_type& x) { return Sstd::cosh(x); }
            static val_type tanh (const val_type& x) { return Sstd::tanh(x); }
            static val_type asin (const val_type& x) { return Sstd::asin(x); }
            static val_type acos (const val_type& x) { return Sstd::acos(x); }
            static val_type atan (const val_type& x) { return Sstd::atan(x); }
            static val_type nan () { return std::numeric_limits<long double>::quiet_NaN(); }
            static mag_type epsilon () { return LDBL_EPSILON; }

            typedef mag_type magnitudeType;
            typedef Sdouble halfPrecision;
            typedef Slong_double doublePrecision; // no long long double
            static const bool isComplex = false;
            static const bool isOrdinal = false;
            static const bool isComparable = true;
            static const bool hasMachineParameters = true;

            static bool isnaninf (const val_type& x) { return isNan (x) || isInf (x); }
            static mag_type magnitude (const val_type& x) { return abs (x); }
            static val_type conjugate (const val_type& x) { return conj (x); }
            static std::string name () { return "Slong_double"; }
            static val_type squareroot (const val_type& x) { return sqrt (x); }
            static mag_type eps () { return epsilon (); }
            static mag_type sfmin () { return LDBL_MIN; }
            static int base () { return FLT_RADIX; }
            static mag_type prec () { return eps () * static_cast<long double> (base ()); }
            static int t () { return LDBL_MANT_DIG; }
            static mag_type rnd () { return one (); }
            static int emin () { return LDBL_MIN_EXP; }
            static mag_type rmin () { return LDBL_MIN; }
            static int emax () { return LDBL_MAX_EXP; }
            static mag_type rmax () { return LDBL_MAX; }
        };

    }

    template<class Generator> struct rand<Generator,Sfloat>
    {
        KOKKOS_INLINE_FUNCTION static Sfloat max(){return 1.0f;}
        KOKKOS_INLINE_FUNCTION static Sfloat draw(Generator& gen) {return gen.frand();}
        KOKKOS_INLINE_FUNCTION static Sfloat draw(Generator& gen, const Sfloat& range) {return gen.frand(range.number);}
        KOKKOS_INLINE_FUNCTION static Sfloat draw(Generator& gen, const Sfloat& start, const Sfloat& end) {return gen.frand(start.number,end.number);}
    };

    template<class Generator> struct rand<Generator,Sdouble>
    {
        KOKKOS_INLINE_FUNCTION static Sdouble max(){return 1.0;}
        KOKKOS_INLINE_FUNCTION static Sdouble draw(Generator& gen) {return gen.drand();}
        KOKKOS_INLINE_FUNCTION static Sdouble draw(Generator& gen, const Sdouble& range) {return gen.drand(range.number);}
        KOKKOS_INLINE_FUNCTION static Sdouble draw(Generator& gen, const Sdouble& start, const Sdouble& end) {return gen.drand(start.number,end.number);}
    };

    template<class Generator> struct rand<Generator, Kokkos::complex<Sfloat> >
    {
        KOKKOS_INLINE_FUNCTION static Kokkos::complex<Sfloat> max ()
        {
            return Kokkos::complex<Sfloat> (1.0f, 1.0f);
        }
        KOKKOS_INLINE_FUNCTION static Kokkos::complex<Sfloat> draw (Generator& gen)
        {
            const Sfloat re = gen.frand ();
            const Sfloat im = gen.frand ();
            return Kokkos::complex<Sfloat> (re, im);
        }
        KOKKOS_INLINE_FUNCTION static Kokkos::complex<Sfloat> draw (Generator& gen, const Kokkos::complex<Sfloat>& range)
        {
            const Sfloat re = gen.frand (real(range).number);
            const Sfloat im = gen.frand (imag(range).number);
            return Kokkos::complex<Sfloat> (re, im);
        }
        KOKKOS_INLINE_FUNCTION static Kokkos::complex<Sfloat> draw (Generator& gen, const Kokkos::complex<Sfloat>& start, const Kokkos::complex<Sfloat>& end)
        {
            const Sfloat re = gen.frand (real(start).number, real(end).number);
            const Sfloat im = gen.frand (imag(start).number, imag(end).number);
            return Kokkos::complex<Sfloat> (re, im);
        }
    };

    template<class Generator> struct rand<Generator, Kokkos::complex<Sdouble> >
    {
        KOKKOS_INLINE_FUNCTION static Kokkos::complex<Sdouble> max ()
        {
            return Kokkos::complex<Sdouble> (1.0, 1.0);
        }
        KOKKOS_INLINE_FUNCTION static Kokkos::complex<Sdouble> draw (Generator& gen)
        {
            const Sdouble re = gen.drand ();
            const Sdouble im = gen.drand ();
            return Kokkos::complex<Sdouble> (re, im);
        }
        KOKKOS_INLINE_FUNCTION static Kokkos::complex<Sdouble> draw (Generator& gen, const Kokkos::complex<Sdouble>& range)
        {
            const Sdouble re = gen.drand (real(range)).number;
            const Sdouble im = gen.drand (imag(range).number);
            return Kokkos::complex<Sdouble> (re, im);
        }
        KOKKOS_INLINE_FUNCTION static Kokkos::complex<Sdouble> draw (Generator& gen, const Kokkos::complex<Sdouble>& start, const Kokkos::complex<Sdouble>& end)
        {
            const Sdouble re = gen.drand (real(start).number, real(end).number);
            const Sdouble im = gen.drand (imag(start).number, imag(end).number);
            return Kokkos::complex<Sdouble> (re, im);
        }
    };

};

