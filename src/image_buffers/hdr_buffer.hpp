#pragma once

#include <vector>
#include <algorithm>

#include <glm/glm.hpp>

namespace pt::image_buffers
{
    class hdr_buffer
    {
        using buffer = std::vector<std::vector<glm::vec3>>;

    public:
        constexpr hdr_buffer(unsigned int width = 512u, unsigned int height = 512u) :
            data(width, std::vector(height, glm::vec3{0.0f})),
            width(width),
            height(height)
        {}

        [[nodiscard]] constexpr auto get_width() const noexcept -> unsigned int { return width; };
        [[nodiscard]] constexpr auto get_height() const noexcept -> unsigned int { return height; };
        [[nodiscard]] constexpr auto get_data() const noexcept -> buffer const& { return data; };
        [[nodiscard]] constexpr auto get_data() noexcept -> buffer& { return data; };

        constexpr void clear() noexcept
        {
            std::ranges::for_each(data, [](auto& image_column){
                std::ranges::for_each(image_column, [](auto& rgb){
                    rgb = glm::vec3(0.0f);
                });
            });
        }

    private:
        buffer data;
        unsigned int width, height;
    };
}