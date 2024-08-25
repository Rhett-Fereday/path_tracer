#pragma once

#include <image_buffers/hdr_buffer.hpp>
#include <scene.hpp>
#include <camera.hpp>
#include <hit_info.hpp>

namespace pt::integrators
{
    class ray_caster
    {
    public:
        using buffer_type = image_buffers::hdr_buffer;

        struct evaluation_properties
        {};

        [[nodiscard]] glm::vec3 evaluate(scene const& s, ray const& r, evaluation_properties const& properties) const noexcept
        {
            auto const hit_optional = s.closest_hit(r);

            if(!hit_optional) return glm::vec3{0.0f};

            auto const& shape = s.shapes[hit_optional->shape_id];
            auto const& mat = s.materials[std::get<shapes::sphere>(shape).material_ID];

            return mat.albedo;
        }
    };
}