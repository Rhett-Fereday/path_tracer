#pragma once

#include <shapes/sphere.hpp>
#include <shapes/plane.hpp>
#include <shapes/box.hpp>

namespace pt
{
    class shape_visitor
    {
    public:
        [[nodiscard]] constexpr auto operator()(shapes::sphere const& s) const noexcept -> unsigned int
        {
            return s.material_ID;
        }

        [[nodiscard]] constexpr auto operator()(shapes::plane const& p) const noexcept -> unsigned int
        {
            return p.material_ID;
        }

        [[nodiscard]] constexpr auto operator()(shapes::box const& b) const noexcept -> unsigned int
        {
            return b.material_ID;
        }
    };
}