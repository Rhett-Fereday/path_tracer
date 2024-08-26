#pragma once

#include <samplers/random_sampler.hpp>
#include <algorithm>
#include <execution>

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
                unsigned int x{640u}, y{480u};
            } resolution{};
        };

        template<typename Camera>
        buffer_type render_scene(scene const& sc, Camera const& camera, render_properties const& properties = render_properties{}) const noexcept
        {
            buffer_type buffer(properties.resolution.x, properties.resolution.y);

            std::for_each(std::execution::par, buffer.get_tiles().cbegin(), buffer.get_tiles().cend(), [&, this](auto const& tile){

                auto& data = buffer.get_data();
                float const inv_samples = 1.0f / properties.num_samples;
                float const aspect_ratio = properties.resolution.x / static_cast<float>(properties.resolution.y);
                float const width_inverse = 1.0f / properties.resolution.x;
                float const height_inverse = 1.0f / properties.resolution.y;

                pt::samplers::random_sampler sampler;

                for(auto y = tile.y_begin; y < tile.y_end; ++y)
                {
                    for(auto x = tile.x_begin; x < tile.x_end; ++x)
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
            });

            return buffer;
        }
    };
}