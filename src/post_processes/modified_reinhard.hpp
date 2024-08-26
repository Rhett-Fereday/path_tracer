#pragma once

#include <image_buffers/hdr_buffer.hpp>

namespace pt::post_processes
{
    struct modified_reinhard
    {
        [[nodiscard]] pt::image_buffers::hdr_buffer operator()(pt::image_buffers::hdr_buffer const& buffer) const
        {
            auto out_buffer = buffer;
            auto& data = out_buffer.get_data();

            glm::vec3 const magic_vec{ 0.2126f, 0.7152f, 0.0722f };

            float max_luminance = 0.0f;

            for(auto x = 0; x < buffer.get_width(); ++x)
            {
                for(auto y = 0; y < buffer.get_height(); ++y)
                {
                    float const luminance = glm::dot(data[x][y], magic_vec);

                    max_luminance = std::max(max_luminance, luminance);
                }
            }

            float const inv_max_squared = 1.0f / (max_luminance * max_luminance);

            for(auto x = 0; x < buffer.get_width(); ++x)
            {
                for(auto y = 0; y < buffer.get_height(); ++y)
                {
                    auto& color = data[x][y];
                    float const luminance = glm::dot(color, magic_vec);
                    float const numerator = luminance * (1.0f + (luminance * inv_max_squared));
                    float const new_luminance = numerator / (1.0f + luminance);

                    color *= (new_luminance / luminance);
                }
            }

            return out_buffer;
        }
    };
}