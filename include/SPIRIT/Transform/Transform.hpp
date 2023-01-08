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


#ifndef SPIRIT_TRANSFORM_HPP
#define SPIRIT_TRANSFORM_HPP

#include "SPIRIT/Base.hpp"
#include "Eigen/Geometry"
#include "SPIRIT/Matrix/Matrix.hpp"
#include <numbers>


namespace sp
{

template <class T>
constexpr T
radians(T degrees)
{
    return degrees * std::numbers::pi_v<T> / 180;
}

template <class T>
constexpr T
degrees(T radians)
{
    return radians * 180 * std::numbers::inv_pi_v<T>;
}


template <class T, sp::Int32 dim>
class Transformation
{
    typedef Matrix<T, dim + 1, dim + 1> Mat;

    typedef Eigen::Rotation2D<T> Rotation2D;
    typedef Eigen::AngleAxis<T> Rotation3D;
    typedef Eigen::Translation<T, dim> Translate;

    typedef Eigen::Transform<T, dim, Eigen::Affine, Eigen::AutoAlign | Eigen::ColMajor>
        Transform;

public:

    Transformation() { t.setIdentity(); }

    template <class U>
    Transformation(const sp::Matrix<U, dim + 1, dim + 1> & mat) : t{mat}
    {
    }

    template <class U>
    Transformation(const sp::Matrix<U, dim, dim> & mat)
    {
        *this = mat;
    }

    Transformation &
    operator=(const Transformation & other)
    {
        t = other.t;
        return *this;
    }

    template <class U>
    Transformation &
    operator=(const sp::Matrix<U, dim, dim> & other)
    {
        t.setIdentity();
        t.linear() = other.mat;
        return *this;
    }

    template <class U>
    Transformation &
    operator=(const sp::Matrix<U, dim + 1, dim + 1> & other)
    {
        t.matrix() = other;
        return *this;
    }


    Transformation
    inversed() const
    {
        return t.inverse();
    }

    Transformation &
    inverse()
    {
        t = t.inverse();
        return *this;
    }

    Transformation &
    scale(T scale)
    {
        t.prescale(scale);
        return *this;
    }

    Transformation &
    scale(const sp::Vec<dim> & scales)
    {
        t.prescale(scales);
        return *this;
    }

    Transformation &
    translate(const sp::Vec<dim> & offset)
    {
        t.pretranslate(offset.mat);
        return *this;
    }

    Transformation &
    rotate(T radians)
    {
        static_assert(dim == 2, "Must be a transformation in 2D");
        t.prerotate(Rotation2D{radians});
        return *this;
    }

    // axis is assumed to be normalized
    Transformation &
    rotate(T radians, sp::Vec3 axis)
    {
        static_assert(dim == 3, "Must be a transformation in 3D");
        t.prerotate(Rotation3D{radians, axis.mat});
        return *this;
    }

    Transformation &
    shear(T sx, T sy)
    {
        static_assert(dim == 2, "Must be a transformation in 2D");
        t.preshear(sx, sy);
        return *this;
    }

    sp::Vector<T, dim>
    translation() const
    {
        return sp::Vector<T, dim>{t.translation()};
    }

    sp::Matrix<T, dim, dim + 1>
    affine() const
    {
        return sp::Matrix<T, dim, dim + 1>{t.affine()};
    }

    sp::Matrix<T, dim, dim>
    linear() const
    {
        return sp::Matrix<T, dim, dim>{t.linear()};
    }

    Mat
    toMatrix() const
    {
        return Mat{t.matrix()};
    }

    // other is applied after all current transformations
    template <class U>
    Transformation &
    transform(const Transformation<U, dim> & other)
    {
        return *this = other * (*this);
    }


    template <class U>
    Transformation
    operator*(const Transformation<U, dim> & other) const
    {
        return t * other.t;
    }

    template <class U>
    Transformation &
    operator*=(const Transformation<U, dim> & other)
    {
        t = t * other.t; // TODO: Aliasing risk?
        return *this;
    }

    template <class U, sp::Int32 nCols>
    sp::Matrix<U, dim, nCols>
    applyTo(const sp::Matrix<U, dim, nCols> & vectors) const
    {
        return *this * vectors;
    }

    template <class U, sp::Int32 nCols>
    sp::Matrix<U, dim, nCols>
    applyTo(const sp::Matrix<U, dim + 1, nCols> & vectors) const
    {
        return *this * vectors;
    }

    template <class U, sp::Int32 nCols>
    sp::Matrix<U, dim, nCols>
    operator*(const sp::Matrix<U, dim, nCols> & other) const
    {
        return t * other.mat;
    }

    template <class U, sp::Int32 nCols>
    sp::Matrix<U, dim, nCols>
    operator*(const sp::Matrix<U, dim + 1, nCols> & other) const
    {
        return t * other.mat;
    }

    bool
    operator==(const Transformation & other) const
    {
        return t.matrix() == other.t.matrix();
    }

    bool
    operator!=(const Transformation & other) const
    {
        return t.matrix() != other.t.matrix();
    }

    bool
    isApprox(
        const Transformation & other,
        T tolerance = Eigen::NumTraits<T>::dummy_precision()
    ) const
    {
        return t.isApprox(other.t, tolerance);
    }


private:

    template <class, sp::Int32>
    friend class Transformation;

    Transformation(Transform t) : t{t} {}

    Transform t;
};

template <sp::Int32 dim>
using Transform = Transformation<float, dim>;

typedef Transform<2> Transform2D;
typedef Transform<3> Transform3D;

} // namespace sp


#endif // SPIRIT_TRANSFORM_HPP
