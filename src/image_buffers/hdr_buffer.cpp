#include <image_buffers/hdr_buffer.hpp>

#include <algorithm>
#include <ranges>
#include <vector>

constexpr pt::image_buffers::hdr_buffer::hdr_buffer(unsigned int width, unsigned int height) :
    data(width, std::vector(height, glm::vec3{0.0f})),
    width(width),
    height(height)
{}

inline constexpr void pt::image_buffers::hdr_buffer::clear() noexcept
{
    std::ranges::for_each(data, [](auto& image_column){
        std::ranges::for_each(image_column, [](auto& rgb){
            rgb = glm::vec3(0.0f);
        });
    });
}