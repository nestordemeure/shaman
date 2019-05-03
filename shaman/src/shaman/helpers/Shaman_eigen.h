//
// Created by demeuren on 06/03/18.
//

#pragma  once

#include <Eigen/Core>
#include <Eigen/Dense>

namespace Eigen
{
    /*
     * adds Slong_double to eigen
     * NumTraits<long double> give us the epsilon, dummy_precision, lowest and highest functions
     */
    template<>
    struct NumTraits<Slong_double> : NumTraits<long double>
    {
        typedef Slong_double Real;
        typedef Slong_double NonInteger;
        typedef Slong_double Nested;
        enum
        {
            IsComplex = 0,
            IsInteger = 0,
            IsSigned = 1,
            RequireInitialization = 0,  // 1 to call constructor
            ReadCost = 1,
            AddCost = 1,
            MulCost = 1
        };
    };

    /*
     * adds Sdouble to eigen
     * NumTraits<double> gives us the epsilon, dummy_precision, lowest and highest functions
     */
    template<>
    struct NumTraits<Sdouble> : NumTraits<double>
    {
        typedef Sdouble Real;
        typedef Sdouble NonInteger;
        typedef Sdouble Nested;
        enum
        {
            IsComplex = 0,
            IsInteger = 0,
            IsSigned = 1,
            RequireInitialization = 0,  // 1 to call constructor
            ReadCost = 1,
            AddCost = 1,
            MulCost = 1
        };
    };

    /*
     * adds Sfloat to eigen
     * NumTraits<float> gives us the epsilon, dummy_precision, lowest and highest functions
     */
    template<>
    struct NumTraits<Sfloat> : NumTraits<float>
    {
        typedef Sfloat Real;
        typedef Sfloat NonInteger;
        typedef Sfloat Nested;
        enum
        {
            IsComplex = 0,
            IsInteger = 0,
            IsSigned = 1,
            RequireInitialization = 0,  // 1 to call constructor
            ReadCost = 1,
            AddCost = 1,
            MulCost = 1
        };
    };

    /*
     * adds global matrix typedefs
     * see : https://eigen.tuxfamily.org/dox/group__matrixtypedefs.html
     */
    typedef Eigen::Matrix<std::complex<Sdouble>, 2, 2> SMatrix2cd;
    typedef Eigen::Matrix<std::complex<Sfloat>, 2, 2> SMatrix2cf;
    typedef Eigen::Matrix<Sdouble, 2, 2> SMatrix2d;
    typedef Eigen::Matrix<Sfloat, 2, 2> SMatrix2f;
    typedef Eigen::Matrix<std::complex<Sdouble>, 2, Eigen::Dynamic> SMatrix2Xcd;
    typedef Eigen::Matrix<std::complex<Sfloat>, 2, Eigen::Dynamic> SMatrix2Xcf;
    typedef Eigen::Matrix<Sdouble, 2, Eigen::Dynamic> SMatrix2Xd;
    typedef Eigen::Matrix<Sfloat, 2, Eigen::Dynamic> SMatrix2Xf;
    typedef Eigen::Matrix<std::complex<Sdouble>, 3, 3> SMatrix3cd;
    typedef Eigen::Matrix<std::complex<Sfloat>, 3, 3> SMatrix3cf;
    typedef Eigen::Matrix<Sdouble, 3, 3> SMatrix3d;
    typedef Eigen::Matrix<Sfloat, 3, 3> SMatrix3f;
    typedef Eigen::Matrix<std::complex<Sdouble>, 3, Eigen::Dynamic> SMatrix3Xcd;
    typedef Eigen::Matrix<std::complex<Sfloat>, 3, Eigen::Dynamic> SMatrix3Xcf;
    typedef Eigen::Matrix<Sdouble, 3, Eigen::Dynamic> SMatrix3Xd;
    typedef Eigen::Matrix<Sfloat, 3, Eigen::Dynamic> SMatrix3Xf;
    typedef Eigen::Matrix<std::complex<Sdouble>, 4, 4> SMatrix4cd;
    typedef Eigen::Matrix<std::complex<Sfloat>, 4, 4> SMatrix4cf;
    typedef Eigen::Matrix<Sdouble, 4, 4> SMatrix4d;
    typedef Eigen::Matrix<Sfloat, 4, 4> SMatrix4f;
    typedef Eigen::Matrix<std::complex<Sdouble>, 4, Eigen::Dynamic> SMatrix4Xcd;
    typedef Eigen::Matrix<std::complex<Sfloat>, 4, Eigen::Dynamic> SMatrix4Xcf;
    typedef Eigen::Matrix<Sdouble, 4, Eigen::Dynamic> SMatrix4Xd;
    typedef Eigen::Matrix<Sfloat, 4, Eigen::Dynamic> SMatrix4Xf;
    typedef Eigen::Matrix<std::complex<Sdouble>, Eigen::Dynamic, 2> SMatrixX2cd;
    typedef Eigen::Matrix<std::complex<Sfloat>, Eigen::Dynamic, 2> SMatrixX2cf;
    typedef Eigen::Matrix<Sdouble, Eigen::Dynamic, 2> SMatrixX2d;
    typedef Eigen::Matrix<Sfloat, Eigen::Dynamic, 2> SMatrixX2f;
    typedef Eigen::Matrix<std::complex<Sdouble>, Eigen::Dynamic, 3> SMatrixX3cd;
    typedef Eigen::Matrix<std::complex<Sfloat>, Eigen::Dynamic, 3> SMatrixX3cf;
    typedef Eigen::Matrix<Sdouble, Eigen::Dynamic, 3> SMatrixX3d;
    typedef Eigen::Matrix<Sfloat, Eigen::Dynamic, 3> SMatrixX3f;
    typedef Eigen::Matrix<std::complex<Sdouble>, Eigen::Dynamic, 4> SMatrixX4cd;
    typedef Eigen::Matrix<std::complex<Sfloat>, Eigen::Dynamic, 4> SMatrixX4cf;
    typedef Eigen::Matrix<Sdouble, Eigen::Dynamic, 4> SMatrixX4d;
    typedef Eigen::Matrix<Sfloat, Eigen::Dynamic, 4> SMatrixX4f;
    typedef Eigen::Matrix<std::complex<Sdouble>, Eigen::Dynamic, Eigen::Dynamic> SMatrixXcd;
    typedef Eigen::Matrix<std::complex<Sfloat>, Eigen::Dynamic, Eigen::Dynamic> SMatrixXcf;
    typedef Eigen::Matrix<Sdouble, Eigen::Dynamic, Eigen::Dynamic> SMatrixXd;
    typedef Eigen::Matrix<Sfloat, Eigen::Dynamic, Eigen::Dynamic> SMatrixXf;
    typedef Eigen::Matrix<std::complex<Sdouble>, 1, 2> SRowVector2cd;
    typedef Eigen::Matrix<std::complex<Sfloat>, 1, 2> SRowVector2cf;
    typedef Eigen::Matrix<Sdouble, 1, 2> SRowVector2d;
    typedef Eigen::Matrix<Sfloat, 1, 2> SRowVector2f;
    typedef Eigen::Matrix<std::complex<Sdouble>, 1, 3> SRowVector3cd;
    typedef Eigen::Matrix<std::complex<Sfloat>, 1, 3> SRowVector3cf;
    typedef Eigen::Matrix<Sdouble, 1, 3> SRowVector3d;
    typedef Eigen::Matrix<Sfloat, 1, 3> SRowVector3f;
    typedef Eigen::Matrix<std::complex<Sdouble>, 1, 4> SRowVector4cd;
    typedef Eigen::Matrix<std::complex<Sfloat>, 1, 4> SRowVector4cf;
    typedef Eigen::Matrix<Sdouble, 1, 4> SRowVector4d;
    typedef Eigen::Matrix<Sfloat, 1, 4> SRowVector4f;
    typedef Eigen::Matrix<std::complex<Sdouble>, 1, Eigen::Dynamic> SRowVectorXcd;
    typedef Eigen::Matrix<std::complex<Sfloat>, 1, Eigen::Dynamic> SRowVectorXcf;
    typedef Eigen::Matrix<Sdouble, 1, Eigen::Dynamic> SRowVectorXd;
    typedef Eigen::Matrix<Sfloat, 1, Eigen::Dynamic> SRowVectorXf;
    typedef Eigen::Matrix<std::complex<Sdouble>, 2, 1> SVector2cd;
    typedef Eigen::Matrix<std::complex<Sfloat>, 2, 1> SVector2cf;
    typedef Eigen::Matrix<Sdouble, 2, 1> SVector2d;
    typedef Eigen::Matrix<Sfloat, 2, 1> SVector2f;
    typedef Eigen::Matrix<std::complex<Sdouble>, 3, 1> SVector3cd;
    typedef Eigen::Matrix<std::complex<Sfloat>, 3, 1> SVector3cf;
    typedef Eigen::Matrix<Sdouble, 3, 1> SVector3d;
    typedef Eigen::Matrix<Sfloat, 3, 1> SVector3f;
    typedef Eigen::Matrix<std::complex<Sdouble>, 4, 1> SVector4cd;
    typedef Eigen::Matrix<std::complex<Sfloat>, 4, 1> SVector4cf;
    typedef Eigen::Matrix<Sdouble, 4, 1> SVector4d;
    typedef Eigen::Matrix<Sfloat, 4, 1> SVector4f;
    typedef Eigen::Matrix<std::complex<Sdouble>, Eigen::Dynamic, 1> SVectorXcd;
    typedef Eigen::Matrix<std::complex<Sfloat>, Eigen::Dynamic, 1> SVectorXcf;
    typedef Eigen::Matrix<Sdouble, Eigen::Dynamic, 1> SVectorXd;
    typedef Eigen::Matrix<Sfloat, Eigen::Dynamic, 1> SVectorXf;

    /*
     * adds global arrays typedefs
     * TODO add missing typedefs
     * see : https://eigen.tuxfamily.org/dox/Array_8h_source.html
     */
    typedef Eigen::Array<Sfloat, Eigen::Dynamic, Eigen::Dynamic> SArrayXXf;
    typedef Eigen::Array<Sdouble, Eigen::Dynamic, 1> SArrayXd;
    typedef Eigen::Array<Sfloat, 3, 3> SArray33f;
    typedef Eigen::Array<Sfloat, 4, 1> SArray4f;

    /*
     * overload functions from the unsupported section of Eigen
     * TODO define in a file dedicaced to eigen/unsupported ?
     */
    /*
    namespace internal
    {
        template<typename MatrixType>
        struct matrix_exp_computeUV<MatrixType, Sfloat>
        {
            static void run(const MatrixType &arg, MatrixType &U, MatrixType &V, int &squarings)
            {
                using std::frexp;
                using std::pow;
                const Sfloat l1norm = arg.cwiseAbs().colwise().sum().maxCoeff();
                squarings = 0;
                if (l1norm < 4.258730016922831e-001f)
                {
                    matrix_exp_pade3(arg, U, V);
                }
                else if (l1norm < 1.880152677804762e+000f)
                {
                    matrix_exp_pade5(arg, U, V);
                }
                else
                {
                    const float maxnorm = 3.925724783138660f;
                    frexp(l1norm / maxnorm, &squarings);
                    if (squarings < 0) squarings = 0;
                    MatrixType A = arg.unaryExpr(MatrixExponentialScalingOp<Sfloat>(squarings));
                    matrix_exp_pade7(A, U, V);
                }
            }
        };

        template<typename MatrixType>
        struct matrix_exp_computeUV<MatrixType, Sdouble>
        {
            static void run(const MatrixType &arg, MatrixType &U, MatrixType &V, int &squarings)
            {
                using std::frexp;
                using std::pow;
                const Sdouble l1norm = arg.cwiseAbs().colwise().sum().maxCoeff();
                squarings = 0;
                if (l1norm < 1.495585217958292e-002)
                {
                    matrix_exp_pade3(arg, U, V);
                }
                else if (l1norm < 2.539398330063230e-001)
                {
                    matrix_exp_pade5(arg, U, V);
                }
                else if (l1norm < 9.504178996162932e-001)
                {
                    matrix_exp_pade7(arg, U, V);
                }
                else if (l1norm < 2.097847961257068e+000)
                {
                    matrix_exp_pade9(arg, U, V);
                }
                else
                {
                    const double maxnorm = 5.371920351148152;
                    frexp(l1norm / maxnorm, &squarings);
                    if (squarings < 0) squarings = 0;
                    MatrixType A = arg.unaryExpr(MatrixExponentialScalingOp<Sdouble>(squarings));
                    matrix_exp_pade13(A, U, V);
                }
            }
        };
    }
     */
}

/*
 * adds the needed functions to Slong_double
 */
inline const Slong_double &conj(const Slong_double &x) { return x; }
inline const Slong_double &real(const Slong_double &x) { return x; }
inline Slong_double imag(const Slong_double &) { return 0.L; }
inline Slong_double abs2(const Slong_double &x) { return x * x; }

/*
 * adds the needed functions to Sdouble
 */
inline const Sdouble &conj(const Sdouble &x) { return x; }
inline const Sdouble &real(const Sdouble &x) { return x; }
inline Sdouble imag(const Sdouble &) { return 0.; }
inline Sdouble abs2(const Sdouble &x) { return x * x; }

/*
 * adds the needed functions to Sfloat
 */
inline const Sfloat &conj(const Sfloat &x) { return x; }
inline const Sfloat &real(const Sfloat &x) { return x; }
inline Sfloat imag(const Sfloat &) { return 0.f; }
inline Sfloat abs2(const Sfloat &x) { return x * x; }
