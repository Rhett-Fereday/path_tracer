#pragma once

#include <sphere.hpp>
#include <ray.hpp>
#include <hit_info.hpp>
#include <utils/math.hpp>

#include <optional>

namespace pt
{
    class intersect_visitor
    {
    public:
        [[nodiscard]] constexpr auto operator()(shapes::sphere const& s) const noexcept
        {
            return std::make_pair([](shapes::sphere const& s, ray const& r){
                        return math::ray_sphere_intersect(s.radius, r);
                    }, s);
        }
    };
}