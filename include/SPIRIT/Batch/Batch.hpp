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


#ifndef SPIRIT_BATCH_HPP
#define SPIRIT_BATCH_HPP

#include "xsimd/xsimd.hpp"

namespace sp
{

namespace details
{

// https://xsimd.readthedocs.io/en/latest/api/xsimd_batch.html
// TODO: Wrap into a class that defines all operations available, 
//  possibly emulating some behaviors.

// At first, deifing a vectoriazable type of any size n seemed like a good idea, 
//  but it adds complexity and favors inproper usage of simd.
// instead the recommended approach for writing simd will be:
//  use the batch type available at compile time and use buffers of size 
//  sp::Batch<T>::arch_type::size aligned at alignas(sp::Batch<T>::arch_type::alignement())

// Furthermore we will not write our own matrix class. Instead we will wrap
//  Eigen's matrix into a class whose implementation can be swapped later.
// (Eigen is not only vectorized for matrices of more than 4 elements, it uses 
//      expression templates and lazy evaluation to optimize equations into 
//      loops once an expression is "fully defined": M = a*A + b*B + c*C will be
//      unrolled into a single loop)

template <class T, class Arch = xsimd::default_arch>
using Batch = xsimd::batch<T, Arch>;

template <class T, std::size_t n>
using SizedBatch = Batch<T, xsimd::make_sized_batch_t<T, n>::arch_type>;

} // namespace details


} // namespace sp


#endif // SPIRIT_BATCH_HPP
