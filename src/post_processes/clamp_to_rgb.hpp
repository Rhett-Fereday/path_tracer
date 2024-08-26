#pragma once

#include <image_buffers/rgb_buffer.hpp>
#include <image_buffers/hdr_buffer.hpp>

namespace pt::post_processes
{
    struct clamp_to_rgb
    {
        [[nodiscard]] pt::image_buffers::rgb_buffer operator()(pt::image_buffers::hdr_buffer const& buffer)
        {
            pt::image_buffers::rgb_buffer out_buffer(buffer.get_width(), buffer.get_height());
            auto& data = out_buffer.get_data();

            for(auto x = 0; x < buffer.get_width(); ++x)
            {
                for(auto y = 0; y < buffer.get_height(); ++y)
                {
                    auto& color = data[x][y];
                    
                    color.x = glm::clamp(static_cast<int>(buffer.get_data()[x][y].x * 255), 0, 255);
                    color.y = glm::clamp(static_cast<int>(buffer.get_data()[x][y].y * 255), 0, 255);
                    color.z = glm::clamp(static_cast<int>(buffer.get_data()[x][y].z * 255), 0, 255);
                }
            }

            return out_buffer;
        }
    };
}