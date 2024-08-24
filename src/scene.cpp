#include <scene.hpp>
#include <intersect_visitor.hpp>
#include <variant>

inline constexpr auto pt::scene::add_material(material mat) -> unsigned int
{
    materials.emplace_back(std::move(mat));
    return materials.size() - 1;
}

inline constexpr auto pt::scene::add_shape(shape s, glm::mat4 to_local) -> void
{
    shapes.emplace_back(std::move(s));
    to_world_transforms.emplace_back(glm::inverse(to_local));
    to_local_transforms.emplace_back(std::move(to_local));
}

constexpr auto pt::scene::closest_hit(ray const &r) const noexcept -> std::optional<hit_info>
{
    std::optional<hit_info> info = std::nullopt;
    float min_distance = std::numeric_limits<float>::infinity();
    intersect_visitor visitor;

    for(auto i = 0; i < shapes.size(); ++i)
    {
        auto const& current_shape = shapes[i];
        auto const& to_local_transform = to_local_transforms[i];

        ray const local_ray {};

        auto const [intersect_evaluator, typed_shape] = std::visit(visitor, current_shape);

        if(auto const hit_optional = intersect_evaluator(typed_shape, local_ray);
            hit_optional && hit_optional->distance < min_distance)
        {
            info = std::move(hit_optional);
            min_distance = info->distance;
        }
    }

    return info;
}