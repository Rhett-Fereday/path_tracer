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
        struct buffer_tile
        {
            static constexpr auto tile_dim = 16u;
            unsigned int x_begin, x_end, y_begin, y_end;
        };

        hdr_buffer(unsigned int const width, unsigned int const height) :
            data(width, std::vector(height, glm::vec3{0.0f})),
            width(width),
            height(height)
        {
            compute_tiles();
        }

        [[nodiscard]] constexpr auto get_width() const noexcept -> unsigned int { return width; };
        [[nodiscard]] constexpr auto get_height() const noexcept -> unsigned int { return height; };
        [[nodiscard]] constexpr auto get_data() const noexcept -> buffer const& { return data; };
        [[nodiscard]] constexpr auto get_data() noexcept -> buffer& { return data; };
        [[nodiscard]] constexpr auto get_tiles() const noexcept -> std::vector<buffer_tile> const& { return tiles; }

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
        std::vector<buffer_tile> tiles;
        unsigned int width, height;
        
        void compute_tiles()
        {
            auto const num_tiles_wide = std::ceil(width / buffer_tile::tile_dim);
            auto const num_tiles_high = std::ceil(height / buffer_tile::tile_dim);
            tiles.reserve(num_tiles_wide * num_tiles_high);

            for (auto j = 0; j < num_tiles_high; ++j)
            {
                auto const y_begin = j * buffer_tile::tile_dim;
                auto const y_end = (y_begin + buffer_tile::tile_dim >= height) ? height - 1 : y_begin + buffer_tile::tile_dim;

                for (auto i = 0; i < num_tiles_wide; ++i)
                {
                    auto const x_begin = i * buffer_tile::tile_dim;
                    auto const x_end = (x_begin + buffer_tile::tile_dim >= width) ? width - 1 : x_begin + buffer_tile::tile_dim;

                    tiles.emplace_back(buffer_tile{
                                        x_begin, x_end,
                                        y_begin, y_end
                                    });
                }
            }
        }
    };
}