#ifndef WORKSPACE_HPP
#define WORKSPACE_HPP

#include <concepts>
#include <type_traits>
#include <vector>
#include <array>
#include <cmath>
#include <map>

#include "wires.hpp"

#define EPSILON 1e-9

template<NumericType N>
struct Workspace
{
    struct Grid
    {
        // Make it static for storage it
        // in the .data segment instead the stack
        static std::array<std::array<Point<N, 2>, Workspace<N>::height>, Workspace<N>::width> points;

        // Caching
        std::map<Point<N, 2>, Segment<N>> pointToSement;
        std::map<Segment<N>, Wire<N>> segmentToWire;
    };

    const float tolerance = 0.5f;

    static constexpr std::size_t height = 900; 
    static constexpr std::size_t width = 1200;

    bool gridOn { true };
}; 

#endif // WORKSPACE_HPP