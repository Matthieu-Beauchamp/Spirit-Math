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


#ifndef SPIRIT_RANDOM_HPP
#define SPIRIT_RANDOM_HPP

#include <random>


namespace sp
{

//////////////////////////////////////////////////////////
///
/// \brief static class to return single Random numbers
///
//////////////////////////////////////////////////////////
class Random
{
public:

    //////////////////////////////////////////////////////////
    static void
    seed();


    //////////////////////////////////////////////////////////
    ///
    /// \brief Return a random number
    ///
    /// General case, (almost) all other functions make specialized calls
    /// to this template
    ///
    /// \tparam Type Type to be used / returned
    /// \tparam distrib One of the std:: distributions in <random> header
    /// \param a First distribution constructor parameter
    /// \param b Second distribution constructor parameter
    /// \return Random generated number
    ///
    //////////////////////////////////////////////////////////
    template <typename Type, class distrib>
    static Type
    random_impl(Type a, Type b);


    //////////////////////////////////////////////////////////
    ///
    /// \brief random float between 0 and 1.
    ///
    /// Shorthand function to return a Real (floating) type T
    /// value in the interval [0, 1) using uniform distribution
    ///
    /// \tparam T Real (floating point) type of the return value
    ///
    /// \return T value between [0, 1)
    ///
    //////////////////////////////////////////////////////////
    template <typename T = float>
    static T
    random();


    //////////////////////////////////////////////////////////
    ///
    /// \brief random integer between 0 and nChoices-1
    ///
    /// Shorthand function for picking an Integer type number
    /// between 0 and nChoices-1 using uniform distribution
    ///
    /// \tparam T Integer Type of the return value
    ///
    /// \param nChoices Range of the distribution (positive)
    ///
    /// \return random T value between 0 and nChoices-1
    ///
    //////////////////////////////////////////////////////////
    template <typename T = sp::Uint32>
    static T
    choose(T nChoices);


    //////////////////////////////////////////////////////////
    ///
    /// \brief returns a random boolean
    ///
    /// Returns a bool according to bernoulli's distribution
    /// with probability of p
    ///
    /// \param p probability of returning true, 1 means always true
    ///
    /// \return random true or false
    ///
    /////////////////////////////////////////////////////////////
    static bool
    coin(double p = 0.5);


    //////////////////////////////////////////////////////////
    ///
    /// \brief Uniform (flat) Distributions for random numbers
    ///
    //////////////////////////////////////////////////////////
    class Uni
    {
    public:

        //////////////////////////////////////////////////////////
        ///
        /// \brief returns a random integer between a and b
        ///
        /// Takes an Integer type T and returns a T value in the
        /// interval [a, b] using uniform distribution
        ///
        /// \tparam T integer Type to be used
        ///
        /// \param a minimal value
        /// \param b maximal value
        ///
        /// \return random T value
        ///
        //////////////////////////////////////////////////////////
        template <typename T = sp::Int32>
        static T
        randInt(T a, T b);


        //////////////////////////////////////////////////////////
        ///
        /// \brief returns a random float value between a and b
        ///
        /// Takes a Real (floating) type T and returns a T value in the
        /// interval [a, b) using uniform distribution
        ///
        /// \tparam T float Type to be used
        ///
        /// \param a minimal value
        /// \param b maximal value
        ///
        /// \return random T value
        ///
        //////////////////////////////////////////////////////////
        template <typename T = float>
        static T
        randFloat(T a, T b);
    };


    //////////////////////////////////////////////////////////
    ///
    /// \brief Gaussian Distributions for random numbers
    ///
    //////////////////////////////////////////////////////////
    class Gauss
    {
    public:

        //////////////////////////////////////////////////////////
        ///
        /// \brief returns a random integer
        ///
        /// Takes an Integer type T and returns a T value
        /// according to a Gaussian distribution.
        ///
        /// Uses floats internally
        ///
        /// \tparam T Integer type
        ///
        /// \param mean center of the distribution
        /// \param stdDev standard (average) deviation from the mean
        ///
        /// \return random T value
        ///
        //////////////////////////////////////////////////////////
        template <typename T = sp::Int32>
        static T
        randInt(T mean, T stdDev);


        //////////////////////////////////////////////////////////
        ///
        /// \brief returns a random float
        ///
        /// Takes a Real (floating) type T and returns a T value
        ///  according to a Gaussian distribution
        ///
        /// \tparam T Real (floating) type
        ///
        /// \param mean center of the distribution
        /// \param stdDev standard (average) deviation from the mean
        ///
        /// \return random T value
        ///
        //////////////////////////////////////////////////////////
        template <typename T = float>
        static T
        randFloat(T mean, T stdDev);
    };


    //////////////////////////////////////////////////////////
    ///
    /// \brief Weighted distribution for random integers
    ///
    //////////////////////////////////////////////////////////
    class Weighted
    {
    public:

        //////////////////////////////////////////////////////////
        ///
        /// \brief random number according to a weighted distribution
        ///
        /// Returns a random Integer value of returnType in the
        /// interval 0 to weights.size()-1. Probability is determined by
        /// the weights vector: probability = weight / (sum of weights).
        ///
        /// example:
        /// <code>
        /// int result = sp::Random::Weighted::randInt<int, float>({0.8, 0.01})
        /// // will almost always output 0, small possibility of 1
        /// </code>
        ///
        /// \tparam returnType Integer type to be returned
        /// \tparam weightType Type of the weights, must be
        ///                     convertible to double
        ///
        /// \param weights weights of the output, determines the probability
        ///                 of the return value.
        ///
        /// \return Random returnType integer
        ///
        //////////////////////////////////////////////////////////
        template <typename returnType = sp::Uint32, typename weightType = double>
        static returnType
        randInt(std::vector<weightType> weights);
    };


protected:

    //////////////////////////////////////////////////////////
    ///
    /// \brief Internal generator for all random number generations
    ///
    //////////////////////////////////////////////////////////
    static std::mt19937_64 generator;

    friend class RandList;
};


// ///////////////////////////////////////////////////////


//////////////////////////////////////////////////////////
// Note: Here we neglect the optimization of returning vectors by value
// with C++11 compilers which optimized this process.
//
// We ask for a reference to an external vector and populate it to accommodate
// older versions.
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
///
/// \brief static class for populating containers with random numbers
///
//////////////////////////////////////////////////////////
class RandList
{
public:

    //////////////////////////////////////////////////////////
    ///
    /// \brief populate a container with random numbers
    ///
    /// General case, (almost) all other functions make specialized calls
    /// to this template. <br><br>
    ///
    /// Populate a container with random values.
    /// The container must already be the right size! <br><br>
    ///
    /// The container must provide a begin() and end() and it's
    /// iterator must support operator++ incrementation <br><br>
    ///
    /// <code>
    /// std::vector<int> myVect{};\n
    /// myVect.resize(10);\n
    /// sp::RandList::random_impl\<int,
    ///     std::uniform_int_distribution\<int>> (0, 5, myVect);\n
    /// // myVect now contains 10 random values between 0 and 5
    /// </code>
    ///
    /// \tparam Type Type to be used to fill the vector
    /// \tparam distrib one of the std:: distributions in <random> header
    /// \tparam container any container that can be iterated over
    ///         with container.begin(), container.end() and iterator++
    ///
    /// \param a First parameter of the distribution constructor
    /// \param b First parameter of the distribution constructor
    /// \param receiver container to be populated
    ///
    //////////////////////////////////////////////////////////
    template <typename Type, class distrib, class container>
    static void
    random_Impl(Type a, Type b, container & receiver);


    //////////////////////////////////////////////////////////
    ///
    /// \brief populate a container with random numbers
    ///
    /// General case, (almost) all other functions make specialized calls
    /// to this template. <br><br>
    ///
    /// Populate a container / array with random values.
    ///
    /// <code>
    /// int arr[10];\n
    /// sp::RandList::random_impl\<int,
    ///     std::uniform_int_distribution\<int>> (0, 5, arr, arr+10);\n
    /// // arr now contains 10 random values between 0 and 5
    ///
    /// </code>
    ///
    /// \tparam Type Type to be used to fill the vector
    /// \tparam distrib one of the std:: distributions in <random> header
    ///
    /// \param a First parameter of the distribution constructor
    /// \param b First parameter of the distribution constructor
    /// \param begin Pointer to the first element
    /// \param end Pointer to the last element
    ///
    //////////////////////////////////////////////////////////
    template <typename Type, typename IterType, class distrib>
    static void
    random_Impl(Type a, Type b, IterType begin, IterType end);


    //////////////////////////////////////////////////////////
    ///
    /// \brief fill a container with random floats
    ///
    /// Shorthand function to populate a container with
    /// Real (floating) type T values in the interval [0, 1)
    /// using uniform distribution
    ///
    /// \tparam T Real value type
    /// \tparam container iterable container type
    ///         ( begin(), end(), iterator++ )
    ///
    /// \param receiver container to be populated
    ///
    //////////////////////////////////////////////////////////
    template <typename T = float, class container>
    static void
    random(container & receiver);


    //////////////////////////////////////////////////////////
    ///
    /// \brief fill a container with random floats
    ///
    /// Shorthand function to populate a container with
    /// Real (floating) type T values in the interval [0, 1)
    /// using uniform distribution
    ///
    ///
    /// \tparam T Real value type
    ///
    /// \param begin Pointer to the first element
    /// \param end Pointer to the last element
    ///
    //////////////////////////////////////////////////////////
    template <typename T = float, typename IterType>
    static void
    random(IterType begin, IterType end);


    //////////////////////////////////////////////////////////
    ///
    /// \brief fill a container with random integers
    ///
    /// Shorthand function to populate a container with
    /// Integer type numbers between 0 and nChoices-1
    /// using uniform distribution
    ///
    /// \tparam T Real value type
    /// \tparam container iterable container type
    ///         ( begin(), end(), iterator++ )
    ///
    /// \param receiver container to be populated
    ///
    //////////////////////////////////////////////////////////
    template <typename T = sp::Uint32, class container = std::vector<T>>
    static void
    choose(T nChoices, container & receiver);


    //////////////////////////////////////////////////////////
    ///
    /// \brief fill a container with random integers
    ///
    /// Shorthand function to populate a container with
    /// Integer type numbers between 0 and nChoices-1
    /// using uniform distribution
    ///
    /// \tparam T Real value type
    ///
    /// \param nChoices Number of choices
    /// \param begin Pointer to the first element
    /// \param end Pointer to the last element
    ///
    //////////////////////////////////////////////////////////
    template <typename T = sp::Uint32, typename IterType>
    static void
    choose(T nChoices, IterType begin, IterType end);


    //////////////////////////////////////////////////////////
    ///
    /// \brief fill a container with random booleans
    ///
    /// Populate a container with
    /// booleans according to bernoulli's distribution
    /// with probability p
    ///
    /// \tparam T Integer or bool type
    /// \tparam container iterable container type
    ///         ( begin(), end(), iterator++ )
    ///
    /// \param receiver container to be populated
    /// \param p probability that an output will be true
    ///
    //////////////////////////////////////////////////////////
    template <typename T, class container = std::vector<T>>
    static void
    coin(container & receiver, double p = 0.5);


    //////////////////////////////////////////////////////////
    ///
    /// \brief fill a container with random booleans
    ///
    /// Populate a container with
    /// booleans according to bernoulli's distribution
    /// with probability p

    /// \tparam T Integer or bool type
    /// \param begin Pointer to the first element
    /// \param end Pointer to the last element
    /// \param p probability that an output will be true
    ///
    //////////////////////////////////////////////////////////
    template <typename T, typename IterType>
    static void
    coin(IterType begin, IterType end, double p = 0.5);


    //////////////////////////////////////////////////////////
    ///
    /// \brief Uniform (flat) Distributions for random numbers
    ///
    //////////////////////////////////////////////////////////
    class Uni
    {
    public:

        //////////////////////////////////////////////////////////
        ///
        /// \brief fill a container with random ints
        ///
        /// Populate a container with integer type numbers in
        /// the interval [a, b] using uniform distribution
        ///
        /// \tparam T integer type
        /// \tparam container iterable container type
        ///         ( begin(), end(), iterator++ )
        ///
        /// \param a minimal value
        /// \param b maximal value
        /// \param receiver container to be populated
        ///
        //////////////////////////////////////////////////////////
        template <typename T = sp::Int32, class container = std::vector<T>>
        static void
        randInt(T a, T b, container & receiver);


        //////////////////////////////////////////////////////////
        ///
        /// \brief fill a container with random ints
        ///
        /// Populate a container with integer type numbers in
        /// the interval [a, b] using uniform distribution
        ///
        /// \tparam T integer type
        ///
        /// \param a minimal value
        /// \param b maximal value
        /// \param begin Pointer to the first element
        /// \param end Pointer to the last element
        ///
        //////////////////////////////////////////////////////////
        template <typename T = sp::Int32, typename IterType>
        static void
        randInt(T a, T b, IterType begin, IterType end);


        //////////////////////////////////////////////////////////
        ///
        /// \brief fill a container with random floats
        ///
        /// Populate a container with floating type numbers in
        /// the interval [a, b) using uniform distribution
        ///
        /// \tparam T Real (floating) type
        /// \tparam container iterable container type
        ///         ( begin(), end(), iterator++ )
        /// \param a minimal value
        /// \param b maximal value
        /// \param receiver container to be populated
        ///
        //////////////////////////////////////////////////////////
        template <typename T = float, class container = std::vector<T>>
        static void
        randFloat(T a, T b, container & receiver);


        //////////////////////////////////////////////////////////
        ///
        /// \brief fill a container with random floats
        ///
        /// Populate a container with floating type numbers in
        /// the interval [a, b) using uniform distribution
        ///
        /// \tparam T Real (floating) type
        ///
        /// \param a minimal value
        /// \param b maximal value
        /// \param begin Pointer to the first element
        /// \param end Pointer to the last element
        ///
        //////////////////////////////////////////////////////////
        template <typename T = float, typename IterType>
        static void
        randFloat(T a, T b, IterType begin, IterType end);
    };


    //////////////////////////////////////////////////////////
    ///
    /// \brief Gaussian Distributions for random numbers
    ///
    //////////////////////////////////////////////////////////
    class Gauss
    {
    public:

        //////////////////////////////////////////////////////////
        ///
        /// \brief fill a container with random integers
        ///
        /// Populate a container with integer type numbers
        /// using a Gaussian (Normal) distribution.
        ///
        /// \tparam T integer type
        /// \tparam container iterable container type
        ///         ( begin(), end(), iterator++ )
        /// \param mean average value of the distribution
        /// \param stdDev standard deviation of the distribution
        /// \param receiver container to be populated
        ///
        //////////////////////////////////////////////////////////
        template <typename T = sp::Int32, class container = std::vector<T>>
        static void
        randInt(T mean, T stdDev, container & receiver);


        //////////////////////////////////////////////////////////
        ///
        /// \brief fill a container with random integers
        ///
        /// Populate a container with integer type numbers
        /// using a Gaussian (Normal) distribution.
        ///
        /// \tparam T integer type
        ///
        /// \param mean average value of the distribution
        /// \param stdDev standard deviation of the distribution
        /// \param begin Pointer to the first element
        /// \param end Pointer to the last element
        ///
        //////////////////////////////////////////////////////////
        template <typename T = sp::Int32, typename IterType>
        static void
        randInt(T mean, T stdDev, IterType begin, IterType end);


        //////////////////////////////////////////////////////////
        ///
        /// \brief fill a container with random floats
        ///
        /// Populate a container with floating type numbers
        /// using a Gaussian (Normal) distribution.
        ///
        /// \tparam T Real (floating) type
        /// \tparam container iterable container type
        ///         ( begin(), end(), iterator++ )
        /// \param mean average value of the distribution
        /// \param stdDev standard deviation of the distribution
        /// \param receiver container to be populated
        ///
        //////////////////////////////////////////////////////////
        template <typename T = float, class container = std::vector<T>>
        static void
        randFloat(T mean, T stdDev, container & receiver);


        //////////////////////////////////////////////////////////
        ///
        /// \brief fill a container with random floats
        ///
        /// Populate a container with floating type numbers
        /// using a Gaussian (Normal) distribution.
        ///
        /// \tparam T Real (floating) type
        ///
        /// \param mean average value of the distribution
        /// \param stdDev standard deviation of the distribution
        /// \param begin Pointer to the first element
        /// \param end Pointer to the last element
        ///
        //////////////////////////////////////////////////////////
        template <typename T = float, typename IterType>
        static void
        randFloat(T mean, T stdDev, IterType begin, IterType end);
    };


    class Weighted
    {
    public:


        //////////////////////////////////////////////////////////
        ///
        /// \brief fill a container with random unsigned integers
        ///
        ///
        /// Fills the container with random Integer values of returnType
        /// in the interval 0 to weights.size()-1. <br>
        /// Probability is determined by
        /// the weights vector: probability = weight / (sum of weights).
        ///
        /// <code>
        /// example:\n
        /// std::vector<int> myVect;\n
        /// myVect.resize(10);\n
        /// sp::RandList::Weighted::randInt<int, float>({0.8, 0.2}, myVect);\n
        ///
        /// // will be mostly 0s with some 1s
        /// </code>
        ///
        /// \tparam returnType Integer type to be returned
        /// \tparam weightType Type of the weights, must be
        ///                     convertible to double
        /// \param weights weights of the output, determines the probability
        ///                 of the return value.
        /// \return Random returnType integer
        ///
        //////////////////////////////////////////////////////////
        template <
            typename valType    = sp::Uint32,
            typename weightType = double,
            class container     = std::vector<valType>>
        static void
        randInt(std::vector<weightType> weights, container & receiver);
    };
};


} // namespace sp

#include "Random_inl.hpp"

#endif // SPIRIT_RANDOM_HPP
