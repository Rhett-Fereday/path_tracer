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

        ray_caster(buffer_type buffer, unsigned int const num_samples) noexcept :
            image_buffer(std::move(buffer)),
            sample_count(num_samples)
        {}

        template<typename Camera>
        void render_scene(scene const& s, Camera const& camera) noexcept
        {
            for(auto x = 0; x < image_buffer.get_width(); ++x)
            {
                for(auto y = 0; y < image_buffer.get_height(); ++y)
                {
                    glm::vec3 accumulated{0.0f};

                    for(auto s = 0; s < sample_count; ++s)
                    {
                        auto const ray = camera.generate_ray(x, y, uv1, uv2, aspect_ratio, angle, width_inverse, height_inverse);

                        accumulated += evaluate(s, ray);
                    }
                }
            }
        }

        [[nodiscard]] inline constexpr auto get_image_buffer() const noexcept -> buffer_type const&
        {
            return image_buffer;
        }

        inline constexpr void clear_image_buffer() noexcept
        {
            image_buffer.clear();
        }

    private:
        buffer_type image_buffer;
        unsigned int sample_count;

        [[nodiscard]] glm::vec3 evaluate(scene const& s, ray const& r) const noexcept
        {
            auto const hit_optional = s.closest_hit(r);

            if(!hit_optional) return glm::vec3{0.0f};

            auto const& shape = s.shapes[hit_optional->shape_id];
            auto const& mat = s.materials[std::get<shapes::sphere>(shape).material_ID];

            return mat.albedo;
        }
    };
}