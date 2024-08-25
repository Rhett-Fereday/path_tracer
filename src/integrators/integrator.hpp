#pragma once

#include <samplers/random_sampler.hpp>

namespace pt
{
    template<typename Policy>
    class integrator : public Policy
    {
    public:
        using buffer_type = typename Policy::buffer_type;

        struct render_properties
        {
            typename Policy::evaluation_properties eval_properties{};
            unsigned int num_samples{32u};
            struct resolution_pair
            {
                unsigned int x{512u}, y{512u};
            } resolution{};
        };

        struct tile
        {
            static constexpr unsigned int tile_dim = 16u;
            unsigned int x_begin, y_begin;
        };

        template<typename Camera>
        buffer_type render_scene(scene const& sc, Camera const& camera, render_properties const& properties = render_properties{}) const noexcept
        {
            buffer_type buffer(properties.resolution.x, properties.resolution.y);
            auto& data = buffer.get_data();
            float const inv_samples = 1.0f / properties.num_samples;
            float const aspect_ratio = properties.resolution.x / static_cast<float>(properties.resolution.y);
            float const width_inverse = 1.0f / properties.resolution.x;
            float const height_inverse = 1.0f / properties.resolution.y;

            pt::samplers::random_sampler sampler;

            for(auto x = 0; x < buffer.get_width(); ++x)
            {
                for(auto y = 0; y < buffer.get_height(); ++y)
                {
                    glm::vec3 accumulated{0.0f};

                    for(auto s = 0; s < properties.num_samples; ++s)
                    {
                        auto const ray = camera.generate_ray(x, y, sampler.generate_2D(), sampler.generate_2D(),
                                                                aspect_ratio, width_inverse, height_inverse);

                        accumulated += Policy::evaluate(sc, ray, properties.eval_properties);
                    }

                    data[x][y] = inv_samples * accumulated;
                }
            }

            return buffer;
        }
    };
}