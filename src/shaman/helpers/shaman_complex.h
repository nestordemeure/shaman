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

            #ifdef SHAMAN_TAGGED_ERROR
            const std::complex<preciseType> preciseCorrectedResult(_M_real.corrected_number(), _M_imag.corrected_number());
            preciseCorrectedResult *= std::complex<preciseType>(__z.real().corrected_number(), __z.imag().corrected_number());
            const preciseType errorReal = preciseCorrectedResult.real() - resultReal;
            const preciseType errorImag = preciseCorrectedResult.imag() - resultImag;

            const std::complex<preciseType> preciseResult((preciseType)_M_real.number, (preciseType)_M_imag.number);
            preciseResult *= std::complex<preciseType>((preciseType)__z.real().number, (preciseType)__z.imag().number);
            const preciseType preciseResultReal = preciseResult.real();
            const preciseType preciseResultImag = preciseResult.imag();

            preciseType functionErrorReal = preciseResultReal - resultReal;
            preciseType functionErrorImag = preciseResultImag - resultImag;

            const numberType Mr = _M_real.number;
            const numberType Mi = _M_imag.number;
            const numberType zr = __z.real().number;
            const numberType zi = __z.imag().number;

            Serror newErrorCompReal(_M_real.errorComposants, __z.real().errorComposants, [Mr, zr](errorType eM, errorType ez){return Mr*ez + zr*eM;});
            newErrorCompReal.addErrorsTimeScalar(_M_imag.errorComposants, -zi);
            newErrorCompReal.addErrorsTimeScalar(__z.imag().errorComposants, -Mi);
            newErrorCompReal.addError(functionErrorReal);
            _M_real = Snum(output.real(), errorReal, newErrorCompReal);

            Serror newErrorCompImag(_M_real.errorComposants, __z.imag().errorComposants, [Mr, zi](errorType eM, errorType ez){return Mr*ez + zi*eM;});
            newErrorCompImag.addErrorsTimeScalar(_M_imag.errorComposants, zr);
            newErrorCompImag.addErrorsTimeScalar(__z.real().errorComposants, Mi);
            newErrorCompImag.addError(functionErrorReal);
            _M_imag = Snum(output.imag(), errorImag, newErrorCompImag);
            #else
            // use pair arithmetic as our reference higher precision implementation
            const Snum preciseCorrectedResultReal = _M_real * __z.real() - _M_imag * __z.imag();
            const Snum preciseCorrectedResultImag = _M_real * __z.imag() + _M_imag * __z.real();
            const preciseType errorReal = (preciseCorrectedResultReal - resultReal).corrected_number();
            const preciseType errorImag = (preciseCorrectedResultImag - resultImag).corrected_number();

            _M_imag = Snum(output.imag(), errorImag);
            _M_real = Snum(output.real(), errorReal);
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

            // deduce the correct numerical error
            #ifdef SHAMAN_TAGGED_ERROR
            const std::complex<preciseType> preciseCorrectedResult(_M_real.corrected_number(), _M_imag.corrected_number());
            preciseCorrectedResult /= std::complex<preciseType>(__z.real().corrected_number(), __z.imag().corrected_number());
            const preciseType errorReal = preciseCorrectedResult.real() - resultReal;
            const preciseType errorImag = preciseCorrectedResult.imag() - resultImag;

            const std::complex<preciseType> preciseResult((preciseType)_M_real.number, (preciseType)_M_imag.number);
            preciseResult /= std::complex<preciseType>((preciseType)__z.real().number, (preciseType)__z.imag().number);
            const preciseType preciseResultReal = preciseResult.real();
            const preciseType preciseResultImag = preciseResult.imag();

            preciseType functionErrorReal = preciseResultReal - resultReal;
            preciseType functionErrorImag = preciseResultImag - resultImag;

            const numberType Mr = _M_real.number;
            const numberType Mi = _M_imag.number;
            const numberType zr = __z.real().number;
            const numberType zi = __z.imag().number;
            const numberType denom = std::norm(std::complex<numberType>(__z.real().number, __z.imag().number));

            Serror newErrorCompReal(_M_real.errorComposants, __z.real().errorComposants, [Mr, zr](errorType eM, errorType ez){return Mr*ez + zr*eM;});
            newErrorCompReal.addErrorsTimeScalar(_M_imag.errorComposants, zi);
            newErrorCompReal.addErrorsTimeScalar(__z.imag().errorComposants, Mi);
            newErrorCompReal.divByScalar(denom);
            newErrorCompReal.addError(functionErrorReal);
            _M_real = Snum(output.real(), errorReal, newErrorCompReal);

            Serror newErrorCompImag(_M_real.errorComposants, __z.imag().errorComposants, [Mr, zi](errorType eM, errorType ez){return -(Mr*ez + zi*eM);});
            newErrorCompImag.addErrorsTimeScalar(_M_imag.errorComposants, zr);
            newErrorCompImag.addErrorsTimeScalar(__z.real().errorComposants, Mi);
            newErrorCompImag.divByScalar(denom);
            newErrorCompImag.addError(functionErrorReal);
            _M_imag = Snum(output.imag(), errorImag, newErrorCompImag);
            #else
            // use a pair arithmetic implementation of Smith's algorithm as our reference higher precision implementation
            // TODO compare perf of between this and doing the computation with preciseType
            const complex<Snum> preciseOutput = smithComplexDivision(*this, __z);

            _M_imag = Snum(resultImag, (preciseOutput.imag() - resultImag).corrected_number());
            _M_real = Snum(resultReal, (preciseOutput.real() - resultReal).corrected_number());
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