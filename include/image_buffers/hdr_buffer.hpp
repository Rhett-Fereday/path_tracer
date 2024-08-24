#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace pt::image_buffers
{
    class hdr_buffer
    {
        using buffer = std::vector<std::vector<glm::vec3>>;

    public:
        constexpr hdr_buffer(unsigned int width = 512u, unsigned int height = 512u);

        [[nodiscard]] inline constexpr auto get_width() const noexcept -> unsigned int { return width; };
        [[nodiscard]] inline constexpr auto get_height() const noexcept -> unsigned int { return height; };
        [[nodiscard]] inline constexpr auto get_data() const noexcept -> buffer const& { return data; };
        inline constexpr void clear() noexcept;

    private:
        buffer data;
        unsigned int width, height;
    };
}