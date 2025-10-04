#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP

#include <concepts>

template<typename T>
concept IntegralType = std::integral<T>;

template<typename T>
concept NumericType = std::integral<T> || std::floating_point<T>;

#endif // CONCEPTS_HPP