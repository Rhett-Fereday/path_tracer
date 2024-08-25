#pragma once

namespace pt::lenses
{
    class pinhole
    {
    public:
        [[nodiscard]] glm::vec3 generate_lens_position(glm::vec2 const&) const noexcept
        {
            return glm::vec3{ 0.0f };
        }
    };
}