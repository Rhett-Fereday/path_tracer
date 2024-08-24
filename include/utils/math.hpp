#pragma once

#include <ray.hpp>
#include <hit_info.hpp>

#include <optional>

namespace pt::math
{
    [[nodiscard]] constexpr auto ray_sphere_intersect(float radius, ray const& r) noexcept -> std::optional<hit_info>;
}