#pragma once
#include <complex>

/*
 * A more numerically stable complex division algorithm.
 * Designed to mitigate overflows/underflows.
 * It was picked because it does not rely on an FMA (which would be detrimental to performance if we were to use pair arithmetic on top of it)
 *
 * See also:
 * https://hal-ens-lyon.archives-ouvertes.fr/ensl-00734339v2/document
 * https://arxiv.org/pdf/1210.4539.pdf
 */
template<typename T>
std::complex<T> smithComplexDivision(const std::complex<T>& x, const std::complex<T>& y)
{
    const T xr = x.real();
    const T xi = x.imag();
    const T yr = y.real();
    const T yi = y.imag();

    if (abs(yi) <= abs(yr) )
    {
        const T yratio = yi / yr;
        const T denom = yr + yi * yratio;
        const T re = (xr + xi * yratio) / denom;
        const T im = (xi - xr * yratio) / denom;

        return std::complex<T>(re, im);
    }
    else
    {
        const T yratio = yr / yi;
        const T denom = yr * yratio + yi;
        const T re = (xr * yratio + xi) / denom;
        const T im = (xi * yratio - xr) / denom;

        return std::complex<T>(re, im);
    }
}

namespace std
{
    /*
     * Adapted from std::complex<Snum>
     * needed because std::complex<T> uses a naive algorithm, *different from the one used by std::complex<double>*
     * to deal with multiplication and division
     */
    templated struct complex<Snum>
    {
        /// Value typedef.
        typedef Snum value_type;

        ///  Default constructor.  First parameter is x, second parameter is y.
        ///  Unspecified parameters default to 0.
        constexpr complex(const Snum& __r = Snum(), const Snum& __i = Snum()): _M_real(__r), _M_imag(__i) {}

        // Let the compiler synthesize the copy constructor
        constexpr complex(const complex&) = default;

        ///  Converting constructor.
        template<typename _Up>
        constexpr complex(const complex<_Up>& __z): _M_real(__z.real()), _M_imag(__z.imag()) {}

        constexpr Snum real() const { return _M_real; }
        constexpr Snum imag() const { return _M_imag; }
        void real(Snum __val) { _M_real = __val; }
        void imag(Snum __val) { _M_imag = __val; }

        //----- operations with numberType -----

        /// Assign a scalar to this complex number.
        complex<Snum>& operator=(const numberType& __t)
        {
            _M_real = __t;
            _M_imag = Snum();
            return *this;
        }

        /// Add a scalar to this complex number.
        complex<Snum>& operator+=(const numberType& __t)
        {
            _M_real += __t;
            return *this;
        }

        /// Subtract a scalar from this complex number.
        complex<Snum>& operator-=(const numberType& __t)
        {
            _M_real -= __t;
            return *this;
        }

        /// Multiply this complex number by a scalar.
        complex<Snum>& operator*=(const numberType& __t)
        {
            _M_real *= __t;
            _M_imag *= __t;
            return *this;
        }

        /// Divide this complex number by a scalar.
        complex<Snum>& operator/=(const numberType& __t)
        {
            _M_real /= __t;
            _M_imag /= __t;
            return *this;
        }
        
        //----- operations with Snum -----
        
        /// Assign a scalar to this complex number.
        complex<Snum>& operator=(const Snum& __t)
        {
            _M_real = __t;
            _M_imag = Snum();
            return *this;
        }

        /// Add a scalar to this complex number.
        complex<Snum>& operator+=(const Snum& __t)
        {
            _M_real += __t;
            return *this;
        }

        /// Subtract a scalar from this complex number.
        complex<Snum>& operator-=(const Snum& __t)
        {
            _M_real -= __t;
            return *this;
        }

        /// Multiply this complex number by a scalar.
        complex<Snum>& operator*=(const Snum& __t)
        {
            _M_real *= __t;
            _M_imag *= __t;
            return *this;
        }

        /// Divide this complex number by a scalar.
        complex<Snum>& operator/=(const Snum& __t)
        {
            _M_real /= __t;
            _M_imag /= __t;
            return *this;
        }

        //----- operations with complex -----

        // Let the compiler synthesize the copy assignment operator
        complex& operator=(const complex&) = default;

        /// Assign another complex number to this one.
        template<typename _Up>
        complex<Snum>& operator=(const complex<_Up>& __z)
        {
            _M_real = __z.real();
            _M_imag = __z.imag();
            return *this;
        }

        /// Add another complex number to this one.
        template<typename _Up>
        complex<Snum>& operator+=(const complex<_Up>& __z)
        {
            _M_real += __z.real();
            _M_imag += __z.imag();
            return *this;
        }

        /// Subtract another complex number from this one.
        template<typename _Up>
        complex<Snum>& operator-=(const complex<_Up>& __z)
        {
            _M_real -= __z.real();
            _M_imag -= __z.imag();
            return *this;
        }

        /// Multiply this complex number by another.
        template<typename _Up>
        complex<Snum>& operator*=(const complex<_Up>& __z)
        {
            // the double/float/long double implementation is higher precision than the naive formula used in std::complex<T>
            // hence the need for a specialized implementation
            std::complex<numberType> output(_M_real.number, _M_imag.number);
            output *= std::complex<numberType>(__z.real().number, __z.imag().number);
            const numberType resultReal = output.real();
            const numberType resultImag = output.imag();

            // use pair arithmetic as our reference higher precision implementation
            const Snum preciseResultReal = _M_real * __z.real() - _M_imag * __z.imag();
            const Snum preciseResultImag = _M_real * __z.imag() + _M_imag * __z.real();
            const preciseType errorReal = (preciseResultReal - resultReal).corrected_number();
            const preciseType errorImag = (preciseResultImag - resultImag).corrected_number();

            #ifdef SHAMAN_TAGGED_ERROR
            Serror newErrorCompReal = preciseResultReal.errorComposants;
            newErrorCompReal.addError(errorReal - preciseResultReal.error);
            _M_real = Snum(resultReal, errorReal, newErrorCompReal);

            Serror newErrorCompImag = preciseResultImag.errorComposants;
            newErrorCompImag.addError(errorImag - preciseResultImag.error);
            _M_imag = Snum(resultImag, errorImag, newErrorCompImag);
            #else
            _M_real = Snum(output.real(), errorReal);
            _M_imag = Snum(output.imag(), errorImag);
            #endif

            return *this;
        }

        /// Divide this complex number by another.
        template<typename _Up>
        complex<Snum>& operator/=(const complex<_Up>& __z)
        {
            // the double/float/long double implementation is higher precision than the naive formula used in std::complex<T>
            // hence the need for a specialized implementation
            std::complex<numberType> output(_M_real.number, _M_imag.number);
            output /= std::complex<numberType>(__z.real().number, __z.imag().number);
            const numberType resultReal = output.real();
            const numberType resultImag = output.imag();

            // use a pair arithmetic implementation of Smith's algorithm as our reference higher precision implementation
            const complex<Snum> preciseOutput = smithComplexDivision(*this, __z);
            const preciseType errorReal = (preciseOutput.real() - resultReal).corrected_number();
            const preciseType errorImag = (preciseOutput.imag() - resultImag).corrected_number();

            // deduce the correct numerical error
            #ifdef SHAMAN_TAGGED_ERROR
            Serror newErrorCompReal = preciseOutput.real().errorComposants;
            newErrorCompReal.addError(errorReal - preciseOutput.real().error);
            _M_real = Snum(resultReal, errorReal, newErrorCompReal);

            Serror newErrorCompImag = preciseOutput.imag().errorComposants;
            newErrorCompImag.addError(errorImag - preciseOutput.imag().error);
            _M_imag = Snum(resultImag, errorImag, newErrorCompImag);
            #else
            _M_real = Snum(resultReal, errorReal);
            _M_imag = Snum(resultImag, errorImag);
            #endif

            return *this;
        }

        constexpr complex __rep() const
        {
            return *this;
        }

        std::complex<preciseType> corrected_number() const
        {
            return std::complex<preciseType>(_M_real.corrected_number(), _M_imag.corrected_number());
        };

    private:
        Snum _M_real;
        Snum _M_imag;
    };

    // ----- operations with numberType -----

    ///  Return new complex value @a x plus @a y.
    templated inline complex<Snum> operator+(const complex<Snum>& __x, const numberType& __y)
    {
        complex<Snum> __r = __x;
        __r += __y;
        return __r;
    }

    templated inline complex<Snum> operator+(const numberType& __x, const complex<Snum>& __y)
    {
        complex<Snum> __r = __y;
        __r += __x;
        return __r;
    }

    ///  Return new complex value @a x minus @a y.
    templated inline complex<Snum> operator-(const complex<Snum>& __x, const numberType& __y)
    {
        complex<Snum> __r = __x;
        __r -= __y;
        return __r;
    }

    templated inline complex<Snum> operator-(const numberType& __x, const complex<Snum>& __y)
    {
        complex<Snum> __r = -__y;
        __r += __x;
        return __r;
    }

    ///  Return new complex value @a x times @a y.
    templated inline complex<Snum> operator*(const complex<Snum>& __x, const numberType& __y)
    {
        complex<Snum> __r = __x;
        __r *= __y;
        return __r;
    }

    templated inline complex<Snum> operator*(const numberType& __x, const complex<Snum>& __y)
    {
        complex<Snum> __r = __y;
        __r *= __x;
        return __r;
    }

    ///  Return new complex value @a x divided by @a y.
    templated inline complex<Snum> operator/(const complex<Snum>& __x, const numberType& __y)
    {
        complex<Snum> __r = __x;
        __r /= __y;
        return __r;
    }

    templated inline complex<Snum> operator/(const numberType& __x, const complex<Snum>& __y)
    {
        complex<Snum> __r = __x;
        __r /= __y;
        return __r;
    }

    ///  Return true if @a x is equal to @a y.
    templated inline constexpr bool operator==(const complex<Snum>& __x, const numberType& __y)
    {
        return __x.real() == __y && __x.imag() == Snum();
    }

    templated inline constexpr bool operator==(const numberType& __x, const complex<Snum>& __y)
    {
        return __x == __y.real() && Snum() == __y.imag();
    }

    ///  Return false if @a x is equal to @a y.
    templated inline constexpr bool operator!=(const complex<Snum>& __x, const numberType& __y)
    {
        return __x.real() != __y || __x.imag() != Snum();
    }

    templated inline constexpr bool operator!=(const numberType& __x, const complex<Snum>& __y)
    {
        return __x != __y.real() || Snum() != __y.imag();
    }
}
