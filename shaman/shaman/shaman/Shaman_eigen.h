//
// Created by demeuren on 06/03/18.
//

#ifndef SHAMAN_DEMO_SHAMAN_EIGEN_H
#define SHAMAN_DEMO_SHAMAN_EIGEN_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include "Shaman.h"

namespace Eigen
{
    /*
     * adds Sdouble to eigen
     * NumTraits<double> gives us the epsilon, dummy_precision, lowest and highest functions
     */
    template<> struct NumTraits<Sdouble> : NumTraits<double>
    {
        typedef Sdouble Real;
        typedef Sdouble NonInteger;
        typedef Sdouble Nested;
        enum
        {
            IsComplex = 0,
            IsInteger = 0,
            IsSigned = 1,
            RequireInitialization = 0,
            ReadCost = 1,
            AddCost = 1,
            MulCost = 1
        };
    };

    /*
     * adds Sfloat to eigen
     * NumTraits<float> gives us the epsilon, dummy_precision, lowest and highest functions
     */
    template<> struct NumTraits<Sfloat> : NumTraits<float>
    {
        typedef Sfloat Real;
        typedef Sfloat NonInteger;
        typedef Sfloat Nested;
        enum
        {
            IsComplex = 0,
            IsInteger = 0,
            IsSigned = 1,
            RequireInitialization = 0,
            ReadCost = 1,
            AddCost = 1,
            MulCost = 1
        };
    };

    /*
     * adds global matrix typedefs
     * see : https://eigen.tuxfamily.org/dox/group__matrixtypedefs.html
     */
    typedef Eigen::Matrix< std::complex< Sdouble >, 2, 2 > SMatrix2cd;
    typedef Eigen::Matrix< std::complex< Sfloat >, 2, 2 > SMatrix2cf;
    typedef Eigen::Matrix< Sdouble, 2, 2 > SMatrix2d;
    typedef Eigen::Matrix< Sfloat, 2, 2 > SMatrix2f;
    typedef Eigen::Matrix< std::complex< Sdouble >, 2, Eigen::Dynamic > SMatrix2Xcd;
    typedef Eigen::Matrix< std::complex< Sfloat >, 2, Eigen::Dynamic > SMatrix2Xcf;
    typedef Eigen::Matrix< Sdouble, 2, Eigen::Dynamic > SMatrix2Xd;
    typedef Eigen::Matrix< Sfloat, 2, Eigen::Dynamic > SMatrix2Xf;
    typedef Eigen::Matrix< std::complex< Sdouble >, 3, 3 > SMatrix3cd;
    typedef Eigen::Matrix< std::complex< Sfloat >, 3, 3 > SMatrix3cf;
    typedef Eigen::Matrix< Sdouble, 3, 3 > SMatrix3d;
    typedef Eigen::Matrix< Sfloat, 3, 3 > SMatrix3f;
    typedef Eigen::Matrix< std::complex< Sdouble >, 3, Eigen::Dynamic > SMatrix3Xcd;
    typedef Eigen::Matrix< std::complex< Sfloat >, 3, Eigen::Dynamic > SMatrix3Xcf;
    typedef Eigen::Matrix< Sdouble, 3, Eigen::Dynamic > SMatrix3Xd;
    typedef Eigen::Matrix< Sfloat, 3, Eigen::Dynamic > SMatrix3Xf;
    typedef Eigen::Matrix< std::complex< Sdouble >, 4, 4 > SMatrix4cd;
    typedef Eigen::Matrix< std::complex< Sfloat >, 4, 4 > SMatrix4cf;
    typedef Eigen::Matrix< Sdouble, 4, 4 > SMatrix4d;
    typedef Eigen::Matrix< Sfloat, 4, 4 > SMatrix4f;
    typedef Eigen::Matrix< std::complex< Sdouble >, 4, Eigen::Dynamic > SMatrix4Xcd;
    typedef Eigen::Matrix< std::complex< Sfloat >, 4, Eigen::Dynamic > SMatrix4Xcf;
    typedef Eigen::Matrix< Sdouble, 4, Eigen::Dynamic > SMatrix4Xd;
    typedef Eigen::Matrix< Sfloat, 4, Eigen::Dynamic > SMatrix4Xf;
    typedef Eigen::Matrix< std::complex< Sdouble >, Eigen::Dynamic, 2 > SMatrixX2cd;
    typedef Eigen::Matrix< std::complex< Sfloat >, Eigen::Dynamic, 2 > SMatrixX2cf;
    typedef Eigen::Matrix< Sdouble, Eigen::Dynamic, 2 > SMatrixX2d;
    typedef Eigen::Matrix< Sfloat, Eigen::Dynamic, 2 > SMatrixX2f;
    typedef Eigen::Matrix< std::complex< Sdouble >, Eigen::Dynamic, 3 > SMatrixX3cd;
    typedef Eigen::Matrix< std::complex< Sfloat >, Eigen::Dynamic, 3 > SMatrixX3cf;
    typedef Eigen::Matrix< Sdouble, Eigen::Dynamic, 3 > SMatrixX3d;
    typedef Eigen::Matrix< Sfloat, Eigen::Dynamic, 3 > SMatrixX3f;
    typedef Eigen::Matrix< std::complex< Sdouble >, Eigen::Dynamic, 4 > SMatrixX4cd;
    typedef Eigen::Matrix< std::complex< Sfloat >, Eigen::Dynamic, 4 > SMatrixX4cf;
    typedef Eigen::Matrix< Sdouble, Eigen::Dynamic, 4 > SMatrixX4d;
    typedef Eigen::Matrix< Sfloat, Eigen::Dynamic, 4 > SMatrixX4f;
    typedef Eigen::Matrix< std::complex< Sdouble >, Eigen::Dynamic, Eigen::Dynamic > SMatrixXcd;
    typedef Eigen::Matrix< std::complex< Sfloat >, Eigen::Dynamic, Eigen::Dynamic > SMatrixXcf;
    typedef Eigen::Matrix< Sdouble, Eigen::Dynamic, Eigen::Dynamic > SMatrixXd;
    typedef Eigen::Matrix< Sfloat, Eigen::Dynamic, Eigen::Dynamic > SMatrixXf;
    typedef Eigen::Matrix< std::complex< Sdouble >, 1, 2 > SRowVector2cd;
    typedef Eigen::Matrix< std::complex< Sfloat >, 1, 2 > SRowVector2cf;
    typedef Eigen::Matrix< Sdouble, 1, 2 > SRowVector2d;
    typedef Eigen::Matrix< Sfloat, 1, 2 > SRowVector2f;
    typedef Eigen::Matrix< std::complex< Sdouble >, 1, 3 > SRowVector3cd;
    typedef Eigen::Matrix< std::complex< Sfloat >, 1, 3 > SRowVector3cf;
    typedef Eigen::Matrix< Sdouble, 1, 3 > SRowVector3d;
    typedef Eigen::Matrix< Sfloat, 1, 3 > SRowVector3f;
    typedef Eigen::Matrix< std::complex< Sdouble >, 1, 4 > SRowVector4cd;
    typedef Eigen::Matrix< std::complex< Sfloat >, 1, 4 > SRowVector4cf;
    typedef Eigen::Matrix< Sdouble, 1, 4 > SRowVector4d;
    typedef Eigen::Matrix< Sfloat, 1, 4 > SRowVector4f;
    typedef Eigen::Matrix< std::complex< Sdouble >, 1, Eigen::Dynamic > SRowVectorXcd;
    typedef Eigen::Matrix< std::complex< Sfloat >, 1, Eigen::Dynamic > SRowVectorXcf;
    typedef Eigen::Matrix< Sdouble, 1, Eigen::Dynamic > SRowVectorXd;
    typedef Eigen::Matrix< Sfloat, 1, Eigen::Dynamic > SRowVectorXf;
    typedef Eigen::Matrix< std::complex< Sdouble >, 2, 1 > SVector2cd;
    typedef Eigen::Matrix< std::complex< Sfloat >, 2, 1 > SVector2cf;
    typedef Eigen::Matrix< Sdouble, 2, 1 > SVector2d;
    typedef Eigen::Matrix< Sfloat, 2, 1 > SVector2f;
    typedef Eigen::Matrix< std::complex< Sdouble >, 3, 1 > SVector3cd;
    typedef Eigen::Matrix< std::complex< Sfloat >, 3, 1 > SVector3cf;
    typedef Eigen::Matrix< Sdouble, 3, 1 > SVector3d;
    typedef Eigen::Matrix< Sfloat, 3, 1 > SVector3f;
    typedef Eigen::Matrix< std::complex< Sdouble >, 4, 1 > SVector4cd;
    typedef Eigen::Matrix< std::complex< Sfloat >, 4, 1 > SVector4cf;
    typedef Eigen::Matrix< Sdouble, 4, 1 > SVector4d;
    typedef Eigen::Matrix< Sfloat, 4, 1 > SVector4f;
    typedef Eigen::Matrix< std::complex< Sdouble >, Eigen::Dynamic, 1 > SVectorXcd;
    typedef Eigen::Matrix< std::complex< Sfloat >, Eigen::Dynamic, 1 > SVectorXcf;
    typedef Eigen::Matrix< Sdouble, Eigen::Dynamic, 1 > SVectorXd;
    typedef Eigen::Matrix< Sfloat, Eigen::Dynamic, 1 > SVectorXf;
}

/*
 * adds the needed functions to Sdouble
 */
inline const Sdouble& conj(const Sdouble& x)  { return x; }
inline const Sdouble& real(const Sdouble& x)  { return x; }
inline Sdouble imag(const Sdouble&)    { return 0.; }
inline Sdouble abs2(const Sdouble& x)  { return x*x; }

/*
 * adds the needed functions to Sfloat
 */
inline const Sfloat& conj(const Sfloat& x)  { return x; }
inline const Sfloat& real(const Sfloat& x)  { return x; }
inline Sfloat imag(const Sfloat&)    { return 0.f; }
inline Sfloat abs2(const Sfloat& x)  { return x*x; }

#endif //SHAMAN_DEMO_SHAMAN_EIGEN_H