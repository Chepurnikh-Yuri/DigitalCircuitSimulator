#ifndef WIRES_HPP
#define WIRES_HPP

#include <utility>
#include <memory>
#include <vector>

#include "circuits.hpp"
#include "concepts.hpp"
#include "geometry.hpp"

using namespace Circuits;
using namespace Geometry;

template <
    NumericType N,
    template<typename, typename ...>
        class ContainerType = std::vector
    >
class Wire
{
    public:
    using Pins = ContainerType<Pin<N>>;
    using Segments = ContainerType<Segment<N>>;

    Wire() = default;
    Wire (const Segments& segments) : m_segments(std::move(segments)) { }

    // Copy operations
    Wire(const Wire& other) = default;
    Wire& operator=(const Wire& other) = default;

    // Move operations
    Wire(Wire&& other) noexcept = default;
    Wire& operator=(Wire&& other) noexcept = default;

    ~Wire() = default;

private:
    Pins m_pins;
    Segments m_segments;
};

#endif // WIRES_HPP