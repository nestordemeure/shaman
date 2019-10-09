#pragma  once

#include <Teuchos_ScalarTraitsDecl.hpp>

/*
 * defines traits needed by Tpetra to do computations with a user-defined type
 * see Teuchos_ScalarTraits.hpp for examples
 */
namespace Teuchos
{
    template<>
    struct ScalarTraits<Sfloat>
    {
        typedef Sfloat magnitudeType;
        typedef Sfloat halfPrecision; // no half float
        typedef Sdouble doublePrecision;
        typedef Sfloat coordinateType;

        static const bool isComplex = false;
        static const bool isOrdinal = false;
        static const bool isComparable = true;
        static const bool hasMachineParameters = true;

        static inline Sfloat eps() { return std::numeric_limits<float>::epsilon(); }
        static inline Sfloat sfmin() { return std::numeric_limits<float>::min(); }
        static inline Sfloat base() { return static_cast<Sfloat>(std::numeric_limits<float>::radix); }
        static inline Sfloat prec() { return eps()*base(); }
        static inline Sfloat t() { return static_cast<Sfloat>(std::numeric_limits<float>::digits); }
        static inline Sfloat rnd() { return one(); }
        static inline Sfloat emin() { return static_cast<Sfloat>(std::numeric_limits<float>::min_exponent); }
        static inline Sfloat rmin() { return std::numeric_limits<float>::min(); }
        static inline Sfloat emax() { return static_cast<Sfloat>(std::numeric_limits<float>::max_exponent); }
        static inline Sfloat rmax() { return std::numeric_limits<float>::max(); }
        static inline magnitudeType magnitude(Sfloat a) { return Sstd::abs(a); }
        static inline Sfloat zero() { return 0.0f; }
        static inline Sfloat one() { return 1.0f; }
        static inline Sfloat conjugate(Sfloat x) { return(x); }
        static inline Sfloat real(Sfloat x) { return x; }
        static inline Sfloat imag(Sfloat) { return zero(); }
        static inline Sfloat nan() { return std::numeric_limits<float>::quiet_NaN(); }
        static inline bool isnaninf(Sfloat x) { return Sstd::isnan(x) || Sstd::isinf(x); }
        static inline std::string name() { return "Sfloat"; }
        static inline Sfloat squareroot(Sfloat x) { return Sstd::sqrt(x); }
        static inline Sfloat pow(Sfloat x, Sfloat y) { return Sstd::pow(x,y); }
        static inline Sfloat pi() { return 3.14159265358979323846f; }
        static inline Sfloat log(Sfloat x) { return Sstd::log(x); }
        static inline Sfloat log10(Sfloat x) { return Sstd::log10(x); }
        static inline Sfloat random() { float rnd = (float) std::rand() / RAND_MAX; return -1.0f + 2.0f * rnd; }
        static inline void seedrandom(unsigned int s) {
            std::srand(s);
            #ifdef __APPLE__
                // throw away first random number to address bug 3655
                // http://software.sandia.gov/bugzilla/show_bug.cgi?id=3655
                random();
            #endif
        }
    };

    template<> struct ScalarTraits<Sdouble>
    {
        typedef Sdouble magnitudeType;
        typedef Sdouble coordinateType;
        typedef Sfloat halfPrecision;
        #if defined(HAVE_TEUCHOS_DOUBLE_TO_QD) || defined(HAVE_TEUCHOS_DOUBLE_TO_ARPREC)
                typedef Slong_double doublePrecision; // using Slong_double instead of dd_real or mp_real
        #else
                typedef Sdouble doublePrecision;
        #endif

        static const bool isComplex = false;
        static const bool isOrdinal = false;
        static const bool isComparable = true;
        static const bool hasMachineParameters = true;

        static inline Sdouble eps() { return std::numeric_limits<double>::epsilon(); }
        static inline Sdouble sfmin() { return std::numeric_limits<double>::min(); }
        static inline Sdouble base() { return std::numeric_limits<double>::radix; }
        static inline Sdouble prec() { return eps()*base(); }
        static inline Sdouble t() { return std::numeric_limits<double>::digits; }
        static inline Sdouble rnd() { return one(); }
        static inline Sdouble emin() { return std::numeric_limits<double>::min_exponent; }
        static inline Sdouble rmin() { return std::numeric_limits<double>::min(); }
        static inline Sdouble emax() { return std::numeric_limits<double>::max_exponent; }
        static inline Sdouble rmax() { return std::numeric_limits<double>::max(); }
        static inline magnitudeType magnitude(Sdouble a) { return Sstd::abs(a); }
        static inline Sdouble zero() { return 0.0; }
        static inline Sdouble one() { return 1.0; }
        static inline Sdouble conjugate(Sdouble x) { return(x); }
        static inline Sdouble real(Sdouble x) { return(x); }
        static inline Sdouble imag(Sdouble) { return zero(); }
        static inline Sdouble nan() { return std::numeric_limits<double>::quiet_NaN(); }
        static inline bool isnaninf(Sdouble x) { return Sstd::isnan(x) || Sstd::isinf(x); }
        static inline std::string name() { return "Sdouble"; }
        static inline Sdouble squareroot(Sdouble x) { return Sstd::sqrt(x); }
        static inline Sdouble pow(Sdouble x, Sdouble y) { return Sstd::pow(x,y); }
        static inline Sdouble pi() { return 3.14159265358979323846; }
        static inline Sdouble log(Sdouble x) { return Sstd::log(x); }
        static inline Sdouble log10(Sdouble x) { return Sstd::log10(x); }
        static inline Sdouble random() { double rnd = (double) std::rand() / RAND_MAX; return -1.0 + 2.0 * rnd; }
        static inline void seedrandom(unsigned int s) {
            std::srand(s);
            #ifdef __APPLE__
                // throw away first random number to address bug 3655
                // http://software.sandia.gov/bugzilla/show_bug.cgi?id=3655
                random();
            #endif
        }
    };

    template<> struct ScalarTraits<Slong_double>
    {
        typedef Slong_double magnitudeType;
        typedef Slong_double coordinateType;
        typedef Sdouble halfPrecision;
        typedef Slong_double doublePrecision; // no long long double

        static const bool isComplex = false;
        static const bool isOrdinal = false;
        static const bool isComparable = true;
        static const bool hasMachineParameters = true;

        static inline Slong_double eps() { return std::numeric_limits<long double>::epsilon(); }
        static inline Slong_double sfmin() { return std::numeric_limits<long double>::min(); }
        static inline Slong_double base() { return std::numeric_limits<long double>::radix; }
        static inline Slong_double prec() { return eps()*base(); }
        static inline Slong_double t() { return std::numeric_limits<long double>::digits; }
        static inline Slong_double rnd() { return one(); }
        static inline Slong_double emin() { return std::numeric_limits<long double>::min_exponent; }
        static inline Slong_double rmin() { return std::numeric_limits<long double>::min(); }
        static inline Slong_double emax() { return std::numeric_limits<long double>::max_exponent; }
        static inline Slong_double rmax() { return std::numeric_limits<long double>::max(); }
        static inline magnitudeType magnitude(Slong_double a) { return Sstd::abs(a); }
        static inline Slong_double zero() { return 0.0L; }
        static inline Slong_double one() { return 1.0L; }
        static inline Slong_double conjugate(Slong_double x) { return(x); }
        static inline Slong_double real(Slong_double x) { return(x); }
        static inline Slong_double imag(Slong_double) { return zero(); }
        static inline Slong_double nan() { return std::numeric_limits<long double>::quiet_NaN(); }
        static inline bool isnaninf(Slong_double x) { return Sstd::isnan(x) || Sstd::isinf(x); }
        static inline std::string name() { return "Slong_double"; }
        static inline Slong_double squareroot(Slong_double x) { return Sstd::sqrt(x); }
        static inline Slong_double pow(Slong_double x, Slong_double y) { return Sstd::pow(x,y); }
        static inline Slong_double pi() { return 3.14159265358979323846L; }
        static inline Slong_double log(Slong_double x) { return Sstd::log(x); }
        static inline Slong_double log10(Slong_double x) { return Sstd::log10(x); }
        static inline Slong_double random() { long double rnd = (long double) std::rand() / RAND_MAX; return -1.0L + 2.0L * rnd; }
        static inline void seedrandom(unsigned int s) {
            std::srand(s);
            #ifdef __APPLE__
                // throw away first random number to address bug 3655
                // http://software.sandia.gov/bugzilla/show_bug.cgi?id=3655
                random();
            #endif
        }
    };
}

