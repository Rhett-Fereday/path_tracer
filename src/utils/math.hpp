#pragma once

#include <ray.hpp>
#include <hit_info.hpp>

#include <optional>

namespace pt::math
{
    [[nodiscard]] constexpr auto ray_sphere_intersect(float const radius, ray const& r) noexcept -> std::optional<hit_info>
    {
        hit_info info;

        bool insideObject = false;
        float const a = glm::dot(r.direction, r.direction);
        float const b = glm::dot(2.0f * r.direction, r.origin);
        float const c = glm::dot(r.origin, r.origin) - (radius * radius);

        float radicand = b * b - 4 * a * c;

        if (radicand < 0.0f) return std::nullopt;

        glm::vec3 intersect1, intersect2;
        float const root = glm::sqrt(radicand);
        float u1 = (-b + root) / (2.0f * a);
        float u2 = (-b - root) / (2.0f * a);

        if (u2 < u1) std::swap(u1, u2);

        if (u1 < 0.0001 && u2 < 0.0001) return std::nullopt;

        if (u1 < 0.0001)
        {
            info.position = r.origin + r.direction * u2;
            info.distance = u2;
            info.normal = glm::normalize(-info.position);
            insideObject = true;
        }
        else
        {
            info.position = r.origin + r.direction * u1;
            info.distance = u1;
            info.normal = glm::normalize(info.position);
        }

        return std::optional(std::move(info));
    }

    [[nodiscard]] constexpr auto ray_plane_intersect(float const length, float const width, ray const& r) noexcept -> std::optional<hit_info>
    {
        hit_info info;

        info.normal = glm::vec3{ 0.0f, 0.0f, 1.0f };

        // assuming vectors are all normalized
        float const denom = glm::dot(info.normal, -r.direction);

        if (denom <= 1e-6) return std::nullopt;

        info.distance = glm::dot(r.origin, info.normal) / denom;

        if (info.distance <= 0.0f) return std::nullopt;

        info.position = r.origin + info.distance * r.direction;

        float const x = glm::abs(info.position.x);
        float const y = glm::abs(info.position.y);

        if ((x > width * 0.5f) || (y > length * 0.5f)) return std::nullopt;

        return std::optional(std::move(info));
    }
}