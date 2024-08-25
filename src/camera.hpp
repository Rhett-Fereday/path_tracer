#pragma once

#include <scene.hpp>
#include <ray.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>

namespace pt
{
    template<typename Lens_Type>
    class camera
    {
    public:
        struct camera_settings
        {
            glm::vec3 position{ 0.0f, 0.0f, 3.0f };
            glm::vec3 look_at = { 0.0f, 0.0f, 0.0f };
            glm::vec3 up_vector = { 0.0f, 1.0f, 0.0f };
            float focal_distance{ 1.0f };
            float fov{ 90.0f };
        };

        constexpr camera(Lens_Type lens, camera_settings settings = camera_settings{}) noexcept :
            settings(std::move(settings)),
            to_world_transform(glm::inverse(glm::lookAt(settings.position, settings.look_at, settings.up_vector))),
            lens{std::move(lens)},
            angle(glm::tan(glm::pi<float>() * 0.5f * settings.fov / 180.0f))
        {}

        [[nodiscard]] ray generate_ray(unsigned int const x, unsigned int const y,
                                            glm::vec2 const& uv1, glm::vec2 const& uv2,
                                            float const aspect_ratio, float const width_inverse,
                                            float const height_inverse) const noexcept
        {
            float const x_offset = -0.5f + uv1.x;
            float const y_offset = -0.5f + uv1.y;
            float const i = (2.0f * ((x + 0.5f + x_offset) * width_inverse) - 1.0f) * angle * aspect_ratio;
            float const j = (1.0f - 2.0f * ((y + 0.5f + y_offset) * height_inverse)) * angle;

            ray camera_ray { lens.generate_lens_position(uv2),  glm::normalize(glm::vec3(i, j, -1))};

            float distance = std::abs(settings.focal_distance / camera_ray.direction.z);//std::abs(settings.focalDistance / camera_ray.direction.z);
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
        camera_settings settings;
        glm::mat4 to_world_transform;
        Lens_Type lens;
        float angle;
    };
}