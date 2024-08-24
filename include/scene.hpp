#pragma once

#include <shapes/sphere.hpp>
#include <material.hpp>
#include <ray.hpp>
#include <hit_info.hpp>

#include <variant>
#include <optional>
#include <vector>
#include <glm/gtx/transform.hpp>

namespace pt
{
    class scene
    {
    public:
        using shape = std::variant<shapes::sphere>;

        [[nodiscard]] inline constexpr auto add_material(material mat) -> unsigned int;
        inline constexpr auto add_shape(shape s, glm::mat4 to_local) -> void;
        [[nodiscard]] constexpr auto closest_hit(ray const& r) const noexcept -> std::optional<hit_info>;

    // private:
        std::vector<shape> shapes;
        std::vector<glm::mat4> to_world_transforms;
        std::vector<glm::mat4> to_local_transforms;
        std::vector<material> materials;
    };
}