#pragma once

#include <image_buffers/hdr_buffer.hpp>

namespace pt::post_processes
{
    struct gamma_correction
    {
        [[nodiscard]] pt::image_buffers::hdr_buffer operator()(pt::image_buffers::hdr_buffer const& buffer) const
        {
            auto out_buffer = buffer;
            auto& data = out_buffer.get_data();

            auto const correction_vec = glm::vec3(1.0f / 2.2f);

            for(auto x = 0; x < buffer.get_width(); ++x)
            {
                for(auto y = 0; y < buffer.get_height(); ++y)
                {
                    auto& color = data[x][y];
                    color = glm::pow(color, correction_vec);
                }
            }

            return out_buffer;
        }
    };
}