#pragma once

#include <shapes/sphere.hpp>
#include <shapes/plane.hpp>
#include <ray.hpp>
#include <utils/math.hpp>

namespace pt
{
    class intersect_visitor
    {
    public:
        [[nodiscard]] constexpr auto operator()(shapes::sphere const& s, ray const& r) const noexcept
        {
            return math::ray_sphere_intersect(s.radius, r);
        }

        [[nodiscard]] constexpr auto operator()(shapes::plane const& p, ray const& r) const noexcept
        {
            return math::ray_plane_intersect(p.length, p.width, r);
        }
    };
}