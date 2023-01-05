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


#ifndef SPIRIT_RANDOM_INL_HPP
#define SPIRIT_RANDOM_INL_HPP


#include <type_traits>

#include "Random.hpp"


namespace sp
{

std::mt19937_64 Random::generator = std::mt19937_64{std::random_device{}()};

void
Random::seed()
{
    generator.seed(std::random_device()());
}


bool
Random::coin(double p)
{
    std::bernoulli_distribution dist{p};
    return dist(generator);
}


namespace details
{

// for random distributions with integers,
// the standard says behavior is undefined for char types
// and MSVC throws error.
// => we promote to short
template <class T, bool charSized = sizeof(T) == 1, bool isSigned = std::is_signed<T>::value>
struct Integer
{
};

template <class T, bool isSigned>
struct Integer<T, false, isSigned>
{
    static_assert(std::numeric_limits<T>::is_integer, "Must be an integer");

    typedef T type;
};

template <class T>
struct Integer<T, true, true>
{
    static_assert(std::numeric_limits<T>::is_integer, "Must be an integer");

    typedef sp::Int16 type;
};

template <class T>
struct Integer<T, true, false>
{
    static_assert(std::numeric_limits<T>::is_integer, "Must be an integer");

    typedef sp::Uint16 type;
};

template <class T>
using Integer_t = typename Integer<T>::type;

} // namespace details


// ///////////////////////////////////////////////////////
template <typename T, class distrib>
T
Random::random_impl(T a, T b)
{
    distrib dist{a, b};
    return dist(generator);
}


// ///////////////////////////////////////////////////////
template <typename T>
T
Random::random()
{
    return Uni::randFloat<T>(0, 1);
}


// ///////////////////////////////////////////////////////
template <typename T>
T
Random::choose(T nChoices)
{
    typedef sp::priv::Integer_t<T> U;

    return random_impl<U, std::uniform_int_distribution<U>>(0, nChoices - 1);
}


// ///////////////////////////////////////////////////////
template <typename T>
T
Random::Uni::randInt(T a, T b)
{
    typedef sp::priv::Integer_t<T> U;

    return random_impl<U, std::uniform_int_distribution<U>>(a, b);
}


// ///////////////////////////////////////////////////////
template <typename T>
T
Random::Uni::randFloat(T a, T b)
{
    return random_impl<T, std::uniform_real_distribution<T>>(a, b);
}


// ///////////////////////////////////////////////////////
template <typename T>
T
Random::Gauss::randInt(T mean, T stdDev)
{
    return (T
    )std::round(Random::Gauss::randFloat<float>((float)mean, (float)stdDev));
}


// ///////////////////////////////////////////////////////
template <typename T>
T
Random::Gauss::randFloat(T mean, T stdDev)
{
    return random_impl<T, std::normal_distribution<T>>(mean, stdDev);
}


// ///////////////////////////////////////////////////////
template <typename returnType, typename weightType>
returnType
Random::Weighted::randInt(std::vector<weightType> weights)
{
    typedef sp::priv::Integer_t<returnType> U;
    SPIRIT_ASSERT(weights.size() + 1 <= std::numeric_limits<returnType>::max())

    std::discrete_distribution<U> dist{weights.begin(), weights.end()};
    return dist(generator);
}


// ///////////////////////////////////////////////////////
template <typename Type, class distrib, class container>
void
RandList::random_Impl(Type a, Type b, container & receiver)
{
    distrib dist{a, b};
    auto it = receiver.begin();

    while (it != receiver.end())
    {
        *it = dist(Random::generator);
        ++it;
    }
}


// ///////////////////////////////////////////////////////
template <typename Type, typename IterType, class distrib>
void
RandList::random_Impl(Type a, Type b, IterType begin, IterType end)
{
    distrib dist{a, b};

    auto it = begin;
    while (it != end)
    {
        *it = dist(Random::generator);
        ++it;
    }
}


// ///////////////////////////////////////////////////////
template <typename T, class container>
void
RandList::random(container & receiver)
{
    random_Impl<T, std::uniform_real_distribution<T>, container>(0, 1, receiver);
}


// ///////////////////////////////////////////////////////
template <typename T, typename IterType>
void
RandList::random(IterType begin, IterType end)
{
    random_Impl<T, IterType, std::uniform_real_distribution<T>>(0, 1, begin, end);
}


// ///////////////////////////////////////////////////////
template <typename T, class container>
void
RandList::choose(T nChoices, container & receiver)
{
    typedef sp::priv::Integer_t<T> U;

    random_Impl<U, std::uniform_int_distribution<U>, container>(
        0,
        nChoices - 1,
        receiver
    );
}


// ///////////////////////////////////////////////////////
template <typename T, typename IterType>
void
RandList::choose(T nChoices, IterType begin, IterType end)
{
    typedef sp::priv::Integer_t<T> U;

    random_Impl<U, IterType, std::uniform_int_distribution<U>>(
        0,
        nChoices - 1,
        begin,
        end
    );
}


// ///////////////////////////////////////////////////////
template <typename T, class container>
void
RandList::coin(container & receiver, double p)
{
    std::bernoulli_distribution dist{p};
    auto it = receiver.begin();

    while (it != receiver.end())
    {
        *it = dist(Random::generator);
        ++it;
    }
}


// ///////////////////////////////////////////////////////
template <typename T, typename IterType>
void
RandList::coin(IterType begin, IterType end, double p)
{
    std::bernoulli_distribution dist{p};
    auto it = begin;

    while (it != end)
    {
        *it = dist(Random::generator);
        ++it;
    }
}


// ///////////////////////////////////////////////////////
template <typename T, class container>
void
RandList::Uni::randInt(T a, T b, container & receiver)
{
    typedef sp::priv::Integer_t<T> U;

    random_Impl<U, std::uniform_int_distribution<U>, container>(a, b, receiver);
}


// ///////////////////////////////////////////////////////
template <typename T, typename IterType>
void
RandList::Uni::randInt(T a, T b, IterType begin, IterType end)
{
    typedef sp::priv::Integer_t<T> U;

    random_Impl<U, IterType, std::uniform_int_distribution<U>>(a, b, begin, end);
}


// ///////////////////////////////////////////////////////
template <typename T, class container>
void
RandList::Uni::randFloat(T a, T b, container & receiver)
{
    random_Impl<T, std::uniform_real_distribution<T>, container>(a, b, receiver);
}


// ///////////////////////////////////////////////////////
template <typename T, typename IterType>
void
RandList::Uni::randFloat(T a, T b, IterType begin, IterType end)
{
    random_Impl<T, IterType, std::uniform_real_distribution<T>>(a, b, begin, end);
}


// ///////////////////////////////////////////////////////
template <typename T, class container>
void
RandList::Gauss::randInt(T mean, T stdDev, container & receiver)
{
    std::normal_distribution<float> dist{(float)mean, (float)stdDev};
    auto it = receiver.begin();

    while (it != receiver.end())
    {
        *it = (T)round(dist(Random::generator));
        ++it;
    }
}


// ///////////////////////////////////////////////////////
template <typename T, typename IterType>
void
RandList::Gauss::randInt(T mean, T stdDev, IterType begin, IterType end)
{
    std::normal_distribution<float> dist{(float)mean, (float)stdDev};
    auto it = begin;

    while (it != end)
    {
        *it = (T)round(dist(Random::generator));
        ++it;
    }
}


// ///////////////////////////////////////////////////////
template <typename T, class container>
void
RandList::Gauss::randFloat(T mean, T stdDev, container & receiver)
{
    random_Impl<T, std::normal_distribution<T>, container>(mean, stdDev, receiver);
}


// ///////////////////////////////////////////////////////
template <typename T, typename IterType>
void
RandList::Gauss::randFloat(T mean, T stdDev, IterType begin, IterType end)
{
    random_Impl<T, IterType, std::normal_distribution<T>>(mean, stdDev, begin, end);
}


// ///////////////////////////////////////////////////////
template <typename valType, typename weightType, class container>
void
RandList::Weighted::randInt(std::vector<weightType> weights, container & receiver)
{
    typedef sp::priv::Integer_t<valType> U;
    SPIRIT_ASSERT(weights.size() + 1 <= std::numeric_limits<valType>::max())

    std::discrete_distribution<U> dist{weights.begin(), weights.end()};
    auto it = receiver.begin();

    while (it != receiver.end())
    {
        *it = dist(Random::generator);
        ++it;
    }
}

} // namespace sp

#endif // SPIRIT_RANDOM_INL_HPP
