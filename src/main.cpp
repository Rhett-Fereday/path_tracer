#include <scene.hpp>
#include <camera.hpp>
#include <material.hpp>
#include <lenses/pinhole.hpp>
#include <integrators/ray_caster.hpp>

pt::scene create_scene()
{
    pt::scene s;
    material red_mat { {1.0f, 0.0f, 0.0f} };
    
    auto const red_ID = s.add_material(red_mat);

    s.add_shape(pt::shapes::sphere{1.0f, red_ID}, glm::identity<glm::mat4>());

    return s;
}

auto create_camera()
{
    return pt::camera(pt::lenses::pinhole());
}

auto create_integrator()
{
    return pt::integrators::ray_caster();
}

int main(int argc, char** argv)
{
    auto const s = create_scene();
    auto const c = create_camera();
    auto const i = create_integrator();

    return 0;
}