#pragma once

#include <scene.hpp>
#include <ray.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace pt
{
    template<typename Lens_Type>
    class camera
    {
    public:
        constexpr camera(Lens_Type lens) noexcept :
            lens{std::move(lens)}
        {}

        [[nodiscard]] ray Camera::generate_ray(unsigned int const x, unsigned int const y,
                                            glm::vec2 const& uv1, glm::vec2 const& uv2,
                                            float const aspect_ratio, float const angle,
                                            float const width_inverse, float const height_inverse) const noexcept
        {
            float const x_offset = -0.5f + uv1.x;
            float const y_offset = -0.5f + uv1.y;
            float const i = (2.0f * ((x + 0.5f + x_offset) * width_inverse) - 1.0f) * angle * aspect_ratio;
            float const j = (1.0f - 2.0f * ((y + 0.5f + y_offset) * height_inverse)) * angle;

            ray camera_ray { lens.generate_lens_position(uv2),  glm::normalize(glm::vec3(i, j, -1))};

            float distance = std::abs(1.0f / camera_ray.direction.z);//std::abs(settings.focalDistance / camera_ray.direction.z);
            glm::vec3 focus_plane_point = camera_ray.direction * distance;

            glm::vec3 focused_ray_direction = focus_plane_point - camera_ray.origin;

            // Transform the ray and origin from camera space into world space.
            glm::vec4 temp = to_world_transform * glm::vec4(focused_ray_direction, 0);
            camera_ray.direction = glm::normalize(glm::vec3(temp.x, temp.y, temp.z));

            temp = to_world_transform * glm::vec4(camera_ray.origin, 1);
            camera_ray.origin = { temp.x, temp.y, temp.z };

            return camera_ray;
        }

    private:
        glm::mat4 to_world_transform;
        Lens_Type lens;
    };
}