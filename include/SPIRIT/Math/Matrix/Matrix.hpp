////////////////////////////////////////////////////////////
//
// Spirit
// Copyright (C) 2022 Matthieu Beauchamp-Boulay
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////


#ifndef SPIRIT_MATRIX_HPP
#define SPIRIT_MATRIX_HPP

#include "SPIRIT/Base.hpp"
#include "Eigen/Core"

namespace sp
{

namespace details
{

////////////////////////////////////////////////////////////
/// \brief Wraps the Matrix class from Eigen
///
/// The main purpose of wrapping this class is to allow swapping
/// implementation easily later on if needed.
/// Also Eigen's implementation is very complete (bloated), we hide some features.
///
/// Note the use of auto on the return types, Eigen uses Expression template
/// to optimize calculations.
////////////////////////////////////////////////////////////
template <class T, sp::Int32 mRows, sp::Int32 nCols>
using MatrixBase = Eigen::Matrix<
    T,
    mRows,
    nCols>;//, // condition avoids breaking on row vectors
    // Eigen::AutoAlign | (mRows == 1 ? Eigen::RowMajor : Eigen::ColMajor)>;

// Used methods
//     ////////////////////////////////////////////////////////////
//     // Construction and assignement
//     ////////////////////////////////////////////////////////////

//     using Base::Base;
//     using Base::operator=;

//     using Base::Identity;
//     using Base::Random;
//     using Base::Zero;

//     using Base::homogeneous; // for vects

//     using Base::Unit;
//     using Base::UnitW;
//     using Base::UnitX;
//     using Base::UnitY;
//     using Base::UnitZ;


//     ////////////////////////////////////////////////////////////
//     // Matrix operations
//     ////////////////////////////////////////////////////////////

//     auto
//     transposed() const
//     {
//         return Base::transpose();
//     }

//     auto
//     transpose()
//     {
//         return Base::transposeInPlace();
//     }

//     using Base::computeInverseAndDetWithCheck;
//     using Base::computeInverseWithCheck;

//     using Base::determinant;
//     using Base::inverse;

//     using Base::cross;
//     using Base::dot;

//     using Base::squaredNorm;

//     using Base::norm;
//     using Base::normalize;
//     using Base::normalized;

//     using Base::stableNorm;
//     using Base::stableNormalize;
//     using Base::stableNormalized;

//     ////////////////////////////////////////////////////////////
//     // Decompositions and solving
//     ////////////////////////////////////////////////////////////

//     // https://eigen.tuxfamily.org/dox/group__TopicLinearAlgebraDecompositions.html
//     using Base::colPivHouseholderQr;
//     using Base::fullPivLu;
//     using Base::householderQr;
//     using Base::partialPivLu;

//     template <class U, sp::Int32 mRowsOther, sp::Int32 nColsOther>
//     auto
//     solve(const MatrixBase<U, mRowsOther, nColsOther> & other) const
//     {
//         return this->householderQr().solve(other);
//     }

//     template <class U, sp::Int32 mRowsOther, sp::Int32 nColsOther>
//     auto
//     solveAccurate(const MatrixBase<U, mRowsOther, nColsOther> & other) const
//     {
//         return this->colPivHouseholderQr().solve(other);
//     }

//     ////////////////////////////////////////////////////////////
//     // Element access
//     ////////////////////////////////////////////////////////////

//     // using Base::data;
//     using Base::size;


//     using Base::operator[];
//     using Base::operator(); // indexed view

//     using Base::coeff;
//     using Base::col;
//     using Base::cols; // nCols

//     using Base::row;
//     using Base::rows; // mRows

//     // using Base::colStride;
//     // using Base::rowStride;

//     // using Base::block;
//     // bottom rows, corners, ...

//     // using Base::diagonal;

//     // not sure we want those
//     using Base::colwise;
//     using Base::rowwise;

//     // using Base::triangularView;

//     using Base::w;
//     using Base::x;
//     using Base::y;
//     using Base::z;


//     ////////////////////////////////////////////////////////////
//     // Component wise addition and substraction
//     ////////////////////////////////////////////////////////////

//     using Base::operator+;
//     using Base::operator+=;

//     using Base::operator-;
//     using Base::operator-=;

//     ////////////////////////////////////////////////////////////
//     // Matrix product and Scalar multiplication/division
//     ////////////////////////////////////////////////////////////

//     using Base::pow;

//     using Base::operator*;
//     using Base::operator*=;

//     using Base::operator/;
//     using Base::operator/=;


//     ////////////////////////////////////////////////////////////
//     // Comparison
//     ////////////////////////////////////////////////////////////

//     using Base::operator==;
//     using Base::operator!=;
//     using Base::isApprox;

//     ////////////////////////////////////////////////////////////
//     // Formatting / output
//     ////////////////////////////////////////////////////////////

// };

} // namespace details


template <class, sp::Int32>
class Transformation;


// Here we remove some of Eigen's optimization potential by converting results
// back to matrices. This is to avoid unexpected bugs due to aliasing.
// Furthermore, we don't expect any big equations in our usage.
//  any complex calculations can be made into a method using Eigen's Matrix
template <class T, sp::Int32 mRows, sp::Int32 nCols>
class Matrix
{
    constexpr static bool isColVector = nCols == 1;
    constexpr static bool isRowVector = mRows == 1 && !isColVector;
    constexpr static bool isValue     = isColVector && mRows == 1;
    constexpr static bool isVector    = isRowVector || isColVector;

public:

    ////////////////////////////////////////////////////////////
    // Construction and assignement
    ////////////////////////////////////////////////////////////

    // leaves the matrix uninitialized
    Matrix() {}

    Matrix(std::initializer_list<T> values)
    {
        static_assert(
            isVector,
            "Use initialization per row if this is not a vector"
        );
        for (sp::Int32 i = 0; i < nCols * mRows; ++i)
        {
            mat[i] = values.begin()[i];
        }
    }

    Matrix(const std::initializer_list<std::initializer_list<T>> & rows)
        : mat{rows}
    {
        static_assert(
            !isVector,
            "use coefficient initialization if this is a vector"
        );
    }

    Matrix(const Matrix &) = default;

    Matrix &
    operator=(const Matrix &)
        = default;


    ////////////////////////////////////////////////////////////
    // Static construction
    ////////////////////////////////////////////////////////////

    // Returns a matrix with the values of the identity matrix (not required to be square)
    static Matrix
    Identity()
    {
        return Matrix{Mat::Identity()};
    }

    // Filled with zeros
    static Matrix
    Zero()
    {
        return Matrix{Mat::Zero()};
    }

    // return a random matrix.
    // for float types, values are in [-1, 1].
    // for integer types are spread over their entire range.
    static Matrix
    Random()
    {
        return Matrix{Mat::Random()};
    }

    static Matrix
    Unit(sp::Int32 dimension)
    {
        static_assert(isVector, "Must be a Vector type");
        return Matrix{Mat::Unit(dimension)};
    }

    static Matrix
    UnitX()
    {
        static_assert(isVector, "Must be a Vector type");
        return Matrix{Mat::UnitX()};
    }

    static Matrix
    UnitY()
    {
        static_assert(isVector, "Must be a Vector type");
        return Matrix{Mat::UnitY()};
    }

    static Matrix
    UnitZ()
    {
        static_assert(isVector, "Must be a Vector type");
        return Matrix{Mat::UnitZ()};
    }

    static Matrix
    UnitW()
    {
        static_assert(isVector, "Must be a Vector type");
        return Matrix{Mat::UnitW()};
    }


    ////////////////////////////////////////////////////////////
    // Conversions / transformations
    ////////////////////////////////////////////////////////////

    typedef Matrix<T, isRowVector ? mRows + 1 : mRows, isColVector ? nCols + 1 : nCols>
        Homogeneous;
    Homogeneous
    homogeneous() const
    {
        static_assert(isVector, "Must be a Vector type");
        return Homogeneous{mat.homogeneous()};
    }

    /// \warning do not do A = A.transposed()
    [[nodiscard]] Matrix<T, nCols, mRows>
    transposed() const
    {
        return Matrix<T, nCols, mRows>{mat.transpose()};
    }

    void
    transpose()
    {
        static_assert(mRows == nCols, "Must be square");
        mat.transposeInPlace();
    }

    // returns true if the inversion was successful,
    // otherwise false (is the matrix still preserved?)
    bool
    inverse()
    {
        bool wasInversed;
        mat.computeInverseWithCheck(mat, wasInversed);
        return wasInversed;
    }

    [[nodiscard]] Matrix
    inversed(bool & wasInversed) const
    {
        Matrix inv;
        mat.computeInverseWithCheck(inv.mat, wasInversed);
        return inv;
    }

    ////////////////////////////////////////////////////////////
    // Misc
    ////////////////////////////////////////////////////////////

    T
    determinant() const
    {
        return mat.determinant();
    }

    template <class U, sp::Int32 mRowsOther, sp::Int32 nColsOther>
    T
    dot(const Matrix<U, mRowsOther, nColsOther> & other) const
    {
        return mat.dot(other.mat);
    }

    template <class U, sp::Int32 mRowsOther, sp::Int32 nColsOther>
    T
    cross(const Matrix<U, mRowsOther, nColsOther> & other) const
    {
        static_assert(mRows * nCols == 2, "Must be a 2D vector");
        static_assert(mRowsOther * nColsOther == 2, "Must be a 2D vector");

        return mat.cross(other);
    }

    // returns the matrix of the cross product vectors with vec.
    // TODO: Behavior is unclear
    template <class U, sp::Int32 mRowsOther, sp::Int32 nColsOther>
    Matrix
    cross(const Matrix<U, mRowsOther, nColsOther> & vec) const
    {
        static_assert(mRows == 3 || nCols == 3, "Must be made of 3D vectors");

        static_assert(
            vec.isVector && (mRowsOther == 3 || nColsOther == 3),
            "Must be a 3D vector"
        );

        return Matrix{mat.cross(vec)};
    }

    // returns the matrix of the cross product vectors with vec.
    // Does the cross product of 4D vectors as if they were 3D.
    // TODO: Behavior is unclear
    template <
        class U,
        sp::Int32 mRowsOther,
        sp::Int32 nColsOther,
        std::enable_if_t<
            sp::Matrix<U, mRowsOther, nColsOther>::isVector
                && (mRowsOther == 4 || nColsOther == 4),
            bool> = true>
    Matrix
    cross(const Matrix<U, mRowsOther, nColsOther> & vec) const
    {
        static_assert(mRows == 4 || nCols == 4, "Must be made of 4D vectors");

        return Matrix{mat.cross3(vec)};
    }


    T
    squaredNorm() const
    {
        static_assert(isVector, "Must be a vector type");
        return mat.squaredNorm();
    }
    T
    norm() const
    {
        static_assert(isVector, "Must be a vector type");
        return mat.norm();
    }

    // unchanged if norm == 0
    void
    normalize()
    {
        static_assert(isVector, "Must be a vector type");
        mat.normalize();
    }

    // returns *this if norm == 0
    Matrix
    normalized() const
    {
        static_assert(isVector, "Must be a vector type");
        return Matrix{mat.normalized()};
    }

    ////////////////////////////////////////////////////////////
    // Linear equations solving
    ////////////////////////////////////////////////////////////

    // if no good solution is found, returns junk
    // if many solutions exists, chooses one arbitrarily

    template <class U, sp::Int32 nColsOther>
    Matrix<T, nCols, nColsOther>
    solve(const Matrix<U, mRows, nColsOther> & other) const
    {
        return Matrix<T, nCols, nColsOther>{mat.householderQr().solve(other.mat)};
    }

    template <class U, sp::Int32 nColsOther>
    Matrix<T, nCols, nColsOther>
    solveAccurate(const Matrix<U, mRows, nColsOther> & other) const
    {
        return Matrix<T, nCols, nColsOther>{
            mat.colPivHouseholderQr().solve(other.mat)};
    }

    // Verifies that the solution to x = A.solve(b) of the system Ax=b is valid
    // todo: don't use eigen directly here
    template <class U, sp::Int32 nColsOther>
    bool
    isGoodSolution(
        const Matrix<T, nCols, nColsOther> & x,
        const Matrix<U, mRows, nColsOther> & b,
        T tolerance = Eigen::NumTraits<T>::dummy_precision()
    ) const
    {
        return (*this * x).isApprox(b, tolerance);
    }

    ////////////////////////////////////////////////////////////
    // Element access
    ////////////////////////////////////////////////////////////

    constexpr sp::Int32
    size() const
    {
        return mat.size();
    }

    constexpr sp::Int32
    cols() const
    {
        return mat.cols();
    }

    constexpr sp::Int32
    rows() const
    {
        return mat.rows();
    }

    // TODO: col and row should return an object that provides read/write access
    //  and from which a new matrix can be constructed
    Matrix<T, mRows, 1>
    col(sp::Int32 index) const
    {
        return Matrix<T, mRows, 1>{mat.col(index)};
    }

    Matrix<T, 1, nCols>
    row(sp::Int32 index) const
    {
        return Matrix<T, 1, nCols>{mat.row(index)};
    }

    // Allows applying operations column-wise
    // auto colwise() {return mat.colwise();}
    // auto rowwise() {return mat.rowwise();}

    T &
    operator[](sp::Int32 index)
    {
        static_assert(isVector, "Disabled for matrices, use matrix(row, column)");
        return mat[index];
    }

    const T &
    operator[](sp::Int32 index) const
    {
        static_assert(isVector, "Disabled for matrices, use matrix(row, column)");
        return mat[index];
    }

    T &
    operator()(sp::Int32 row, sp::Int32 col)
    {
        return mat(row, col);
    }

    const T &
    operator()(sp::Int32 row, sp::Int32 col) const
    {
        return mat(row, col);
    }

    // TODO: x, y, z, w ...

    ////////////////////////////////////////////////////////////
    // Matrix operators
    ////////////////////////////////////////////////////////////

    Matrix
    operator+(const Matrix & other) const
    {
        return Matrix{mat + other.mat};
    }

    Matrix &
    operator+=(const Matrix & other)
    {
        mat += other.mat;
        return *this;
    }

    Matrix
    operator-(const Matrix & other) const
    {
        return Matrix{mat - other.mat};
    }

    Matrix &
    operator-=(const Matrix & other)
    {
        mat -= other.mat;
        return *this;
    }

    template <class U, sp::Int32 nColsOther>
    Matrix<T, mRows, nColsOther>
    operator*(const Matrix<U, nCols, nColsOther> & other) const
    {
        return Matrix<T, mRows, nColsOther>{mat * other.mat};
    }

    template <class U>
    Matrix &
    operator*=(const Matrix<U, nCols, nCols> & other)
    {
        mat *= other.mat;
        return *this;
    }

    ////////////////////////////////////////////////////////////
    // Scalar operators
    ////////////////////////////////////////////////////////////

    Matrix
    operator*(T scalar) const
    {
        return Matrix{mat * scalar};
    }

    friend Matrix
    operator*(T scalar, const Matrix & mat)
    {
        return mat * scalar;
    }

    Matrix &
    operator*=(T scalar)
    {
        mat *= scalar;
        return *this;
    }

    Matrix
    operator/(T scalar) const
    {
        return Matrix{mat / scalar};
    }

    Matrix &
    operator/=(T scalar)
    {
        mat /= scalar;
        return *this;
    }


    ////////////////////////////////////////////////////////////
    // Comparison
    ////////////////////////////////////////////////////////////

    template <class U>
    bool
    operator==(const Matrix<U, mRows, nCols> & other) const
    {
        return mat == other.mat;
    }


    template <class U>
    bool
    operator!=(const Matrix<U, mRows, nCols> & other) const
    {
        return mat != other.mat;
    }

    template <class U>
    bool
    isApprox(
        const Matrix<U, mRows, nCols> & other,
        T tolerance = Eigen::NumTraits<T>::dummy_precision()
    ) const
    {
        return mat.isApprox(other.mat, tolerance);
    }


private:

    template <class U, sp::Int32 mRowsOther, sp::Int32 nColsOther>
    friend class Matrix;

    // Wants to construct matrices from Eigen matrices
    template <class U, sp::Int32 dim>
    friend class Transformation;

    typedef sp::details::MatrixBase<T, mRows, nCols> Mat;

    Matrix(const Mat & mat) : mat{mat} {}

    Mat mat;
};


template <class T, sp::Int32 dim>
using ColVector = sp::Matrix<T, dim, 1>;

template <class T, sp::Int32 dim>
using RowVector = sp::Matrix<T, 1, dim>;

template <class T, sp::Int32 dim>
using Vector = sp::ColVector<T, dim>;

template <sp::Int32 dim>
using Vec = sp::Vector<float, dim>;

template <sp::Int32 dim>
using VecI = sp::Vector<sp::Int32, dim>;


typedef sp::Vec<2> Vec2;
typedef sp::Vec<3> Vec3;
typedef sp::Vec<4> Vec4;

typedef sp::VecI<2> Vec2i;
typedef sp::VecI<3> Vec3i;
typedef sp::VecI<4> Vec4i;

typedef sp::Matrix<float, 2, 2> Mat2;
typedef sp::Matrix<float, 3, 3> Mat3;
typedef sp::Matrix<float, 4, 4> Mat4;


} // namespace sp


#endif // SPIRIT_MATRIX_HPP
