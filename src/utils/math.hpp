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

    [[nodiscard]] constexpr auto ray_box_intersect(float const length, float const width, float const height, ray const& r) noexcept -> std::optional<hit_info>
    {
        hit_info info;

        glm::vec3 const min_dimensions{0.5f * -length, 0.5f * -width, 0.5f * -height};
        glm::vec3 const max_dimensions{0.5f * length, 0.5f * width, 0.5f * height};

        float tmin = (min_dimensions.x - r.origin.x) / r.direction.x;
        float tmax = (max_dimensions.x - r.origin.x) / r.direction.x;

        float val = -1.0f;
        unsigned int index = 0u;

        if (tmin > tmax)
        {
            std::swap(tmin, tmax);
            val = 1.0f;
        }

        float tymin = (min_dimensions.y - r.origin.y) / r.direction.y;
        float tymax = (max_dimensions.y - r.origin.y) / r.direction.y;
        float y_val = -1.0f;

        if (tymin > tymax)
        {
            std::swap(tymin, tymax);
            y_val = 1.0f;
        }

        if ((tmin > tymax) || (tymin > tmax)) return std::nullopt;

        if (tymin > tmin)
        {
            tmin = tymin;
            val = y_val;
            index = 1u;
        }

        tmax = std::min(tmax, tymax);

        float tzmin = (min_dimensions.z - r.origin.z) / r.direction.z;
        float tzmax = (max_dimensions.z - r.origin.z) / r.direction.z;

        float z_val = -1.0f;

        if (tzmin > tzmax)
        {
            std::swap(tzmin, tzmax);
            z_val = 1.0f;
        }

        if ((tmin > tzmax) || (tzmin > tmax)) return std::nullopt;

        if (tzmin > tmin)
        {
            tmin = tzmin;
            val = z_val;
            index = 2u;
        }

        tmax = std::min(tmax, tzmax);

        if ((tmin < 0.0001f) && (tmax < 0.0001f)) return std::nullopt;

        info.distance = tmin;

        if (tmin < 0.0001f)
        {
            info.distance = tmax;
            // insideObject = true;
        }

        info.normal = { 0.0f, 0.0f, 0.0f };
        info.normal[index] = val;
        info.position = r.origin + info.distance * r.direction;

        return std::optional(std::move(info));
    }
}