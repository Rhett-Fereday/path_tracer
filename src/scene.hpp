#pragma once

#include <shapes/sphere.hpp>
#include <shapes/plane.hpp>
#include <shapes/box.hpp>
#include <material.hpp>
#include <ray.hpp>
#include <hit_info.hpp>
#include <intersect_visitor.hpp>

#include <variant>
#include <optional>
#include <vector>
#include <glm/glm.hpp>

namespace pt
{
    class scene
    {
    public:
        using shape = std::variant<shapes::sphere, shapes::plane, shapes::box>;

        [[nodiscard]] constexpr auto add_material(material mat) -> unsigned int
        {
            materials.emplace_back(std::move(mat));
            return materials.size() - 1;
        }

        constexpr auto add_shape(shape s, glm::mat4 to_world) -> void
        {
            shapes.emplace_back(std::move(s));
            to_local_transforms.emplace_back(glm::inverse(to_world));
            normal_to_world_transforms.emplace_back(glm::transpose(glm::inverse(to_world)));
            to_world_transforms.emplace_back(std::move(to_world));
        }

        [[nodiscard]] constexpr auto closest_hit(ray const& r) const noexcept -> std::optional<hit_info>
        {
            std::optional<hit_info> info = std::nullopt;
            float min_distance = std::numeric_limits<float>::infinity();
            intersect_visitor visitor;

            for(auto i = 0; i < shapes.size(); ++i)
            {
                auto const& current_shape = shapes[i];
                auto const& to_local_transform = to_local_transforms[i];
                
                ray const local_ray { glm::vec3(to_local_transform * glm::vec4(r.origin, 1.0f)),
                                      glm::normalize(glm::vec3(to_local_transform * glm::vec4(r.direction, 0.0f)))
                                    };

                // auto const [intersect_evaluator, typed_shape] = std::visit(visitor, current_shape);
                // auto const intersect_evaluator = std::visit(visitor, current_shape, std::variant<ray>(local_ray));

                if(auto const hit_optional = std::visit(visitor, current_shape, std::variant<ray>(local_ray));
                    hit_optional && hit_optional->distance < min_distance)
                {
                    info = std::move(hit_optional);
                    info->shape_id = i;
                    min_distance = info->distance;
                }
            }

            auto const& to_world_transform = to_world_transforms[info->shape_id];
            auto const& normal_to_world_transform = normal_to_world_transforms[info->shape_id];
            info->position = glm::vec3(to_world_transform * glm::vec4(info->position, 1.0f));
            info->normal = glm::vec3(normal_to_world_transform * glm::vec4(info->normal, 0.0f));

            return info;
        }

    // private:
        std::vector<shape> shapes;
        std::vector<glm::mat4> to_world_transforms;
        std::vector<glm::mat4> normal_to_world_transforms;
        std::vector<glm::mat4> to_local_transforms;
        std::vector<material> materials;
    };
}