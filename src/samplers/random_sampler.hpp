#pragma once

#include <random>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>

namespace pt::samplers
{
    class random_sampler
    {
    public:
        random_sampler() :
            rd{},
            engine{rd()},
            distribution(std::uniform_real_distribution<float>(0.0, 1.0))
        {}

        [[nodiscard]] inline float generate_1D() noexcept
        {
            return distribution(engine);
        }

        [[nodiscard]] std::vector<float> generate_1D_array(unsigned int const count)
        {
            std::vector<float> values;
            values.reserve(count);

            std::generate_n(std::back_inserter(values), count, [this](){
                return generate_1D();
            });

            return values;
        }

        [[nodiscard]] inline glm::vec2 generate_2D() noexcept
        {
            return glm::vec2{generate_1D(), generate_1D()};
        }

        [[nodiscard]] inline std::vector<glm::vec2> generate_2D_array(unsigned int const count)
        {
            std::vector<glm::vec2> values;
            values.reserve(count);

            std::generate_n(std::back_inserter(values), count, [this](){
                return generate_2D();
            });

            return values;
        }

    private:
        std::random_device rd;
        std::default_random_engine engine;
        std::uniform_real_distribution<float> distribution;
    };
}