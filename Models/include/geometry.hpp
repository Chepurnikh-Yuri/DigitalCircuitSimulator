#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <array>
#include <utility>

#include "concepts.hpp"

namespace Geometry 
{
    template<NumericType N, std::size_t S>
    struct Point
    {
        std::array<N, S> coordinates;
    };

    // Specialization for two-dimensional point
    template<NumericType N>
    struct Point<N, 2>
    {
        N x, y;
    };

    template<NumericType N>
    using Segment = std::pair<Point<N, 2>, Point<N, 2>>;
}

#endif // GEOMETRY_HPP